#include "order.h"
#include "unit.h"
#include "world.h"
#include "unitDesc.h"
#include "player.h"
#include "../userInput.h"
#include <math.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

TrainOrder::TrainOrder(Unit* parHostUnit, EUnitType parUnitTypeToTrain)
	: Order(parHostUnit),
	unitTypeToTrain_(parUnitTypeToTrain),
	trainedUnit_(0),
	startTime_(-1)
{
}

// ============================================================================

TrainOrder::~TrainOrder()
{
}

// ============================================================================

bool TrainOrder::Update(Uint32 parCurTime, Uint32 parElapsedTime)
{
	if (startTime_ == -1)
		startTime_ = parCurTime;

	completionStatus_ = float(parCurTime - startTime_) / float(unitTypeToUnitDesc[unitTypeToTrain_].buildTime);
	completionStatus_ = Clamp(completionStatus_, 0.f, 1.f);

	if (completionStatus_ < 1.f)
	{
		hostUnit_->SetActionState(EUS_TRAINING);
		return false;
	}

	if (!trainedUnit_)
	{
		float2 newUnitPos(hostUnit_->Pos());
		newUnitPos.y += unitTypeToUnitDesc[hostUnit_->Type()].height / 2 + 
			unitTypeToUnitDesc[unitTypeToTrain_].height / 2 + 1; // +1 => to not stay stuck
		trainedUnit_ = new Unit(newUnitPos, unitTypeToTrain_);

		World::Inst()->AddUnit(trainedUnit_);
	}

	hostUnit_->SetActionState(EUS_IDLE);

	return true;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

MoveOrder::MoveOrder(Unit* parHostUnit, const float2& parTargetPos)
	: Order(parHostUnit),
	targetPos_(parTargetPos)
{
}

// ============================================================================

MoveOrder::~MoveOrder()
{
}

// ============================================================================

bool MoveOrder::Update(Uint32 parCurTime, Uint32 parElapsedTime)
{
	assert(hostUnit_->Type() == EUT_PEON || hostUnit_->Type() == EUT_GRUNT);

	float2 deltaPos(
		targetPos_.x - hostUnit_->Pos().x,
		targetPos_.y - hostUnit_->Pos().y);
	if (fabs(deltaPos.x) <= 1.f && fabs(deltaPos.y) <= 1.f)
	{
		targetPos_ = hostUnit_->Pos();
		hostUnit_->SetMoveState(EUS_IDLE);
		return true;
	}

	hostUnit_->SetMoveState(EUS_MOVE);
	hostUnit_->SetDir(DirectionToTarget(hostUnit_->Pos(), targetPos_));

	const float moveSpeed = 0.02f * float(unitTypeToUnitDesc[hostUnit_->Type()].moveSpeed);
	float2 velocity(
		moveSpeed * float(parElapsedTime) * dirs[hostUnit_->Dir()].x,
		moveSpeed * float(parElapsedTime) * dirs[hostUnit_->Dir()].y);
	if (fabs(deltaPos.x) < fabs(velocity.x))
		velocity.x = deltaPos.x;
	if (fabs(deltaPos.y) < fabs(velocity.y))
		velocity.y = deltaPos.y;

	float2 nextPos(hostUnit_->Pos().x + velocity.x, hostUnit_->Pos().y + velocity.y);

	SDL_Rect nextBoundingBox = hostUnit_->BoundingBox();
	nextBoundingBox.x = Sint16(nextPos.x) - nextBoundingBox.w / 2;
	nextBoundingBox.y = Sint16(nextPos.y) - nextBoundingBox.h / 2;
	if (World::Inst()->Collides(hostUnit_, nextBoundingBox))
		hostUnit_->SetMoveState(EUS_IDLE);
	else
		hostUnit_->SetPos(nextPos);

	return false;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

BuildOrder::BuildOrder(Unit *parHostUnit, EUnitType parUnitTypeToBuild, const float2& parPos)
	: Order(parHostUnit),
	unitTypeToBuild_(parUnitTypeToBuild),
	buildingUnit_(0),
	buildingStartTime_(-1),
	buildingPos_(parPos),
	moveOrder_(0)
{
}

// ============================================================================

BuildOrder::~BuildOrder()
{
	if (moveOrder_)
		delete moveOrder_;

	if (player.selectedUnit && player.selectedUnit == buildingUnit_)
		player.selectedUnit = hostUnit_;

	hostUnit_->SetActionState(EUS_IDLE);
}

// ============================================================================

bool BuildOrder::Update(Uint32 parCurTime, Uint32 parElapsedTime)
{
	if (!moveOrder_)
		moveOrder_ = new MoveOrder(hostUnit_, buildingPos_);
	if (!moveOrder_->Update(parCurTime, parElapsedTime))
		return false;

	if (!buildingUnit_)
	{
		buildingStartTime_ = parCurTime;
		completionStatus_ = 0.0f;

		buildingUnit_ = new Unit(buildingPos_, unitTypeToBuild_);
		buildingUnit_->SetActionState(EUS_BEING_BUILD_STATE0);

		World::Inst()->AddUnit(buildingUnit_);

		hostUnit_->SetActionState(EUS_BUILDING);

		// TODO: Move it to another place
		if (player.selectedUnit == hostUnit_)
			player.selectedUnit = buildingUnit_; // force the selection change if necessary
	}
	else
	{
		completionStatus_ = float(parCurTime - buildingStartTime_) / float(unitTypeToUnitDesc[unitTypeToBuild_].buildTime);
		completionStatus_ = Clamp(completionStatus_, 0.f, 1.f);

		if (completionStatus_ < 1.f)
		{
			assert(EUS_BEING_BUILD_STATE1 == (EUS_BEING_BUILD_STATE0 + 1));
			assert(EUS_BEING_BUILD_STATE2 == (EUS_BEING_BUILD_STATE1 + 1));

			EUnitState beingBuildState = EUnitState(EUS_BEING_BUILD_STATE0 +
				int(completionStatus_ * (EUS_BEING_BUILD_STATE2 - EUS_BEING_BUILD_STATE0 + 1)));

			buildingUnit_->SetActionState(beingBuildState);
		}
		else
		{
			buildingUnit_->SetActionState(EUS_IDLE);
			hostUnit_->SetActionState(EUS_IDLE);

			// Make the building unit pop at the bottom of the created building
			float2 newUnitPos(hostUnit_->Pos());
			newUnitPos.y += unitTypeToUnitDesc[unitTypeToBuild_].height / 2 +
				unitTypeToUnitDesc[hostUnit_->Type()].height / 2 + 1; // +1 => to not stay stuck
			hostUnit_->SetPos(newUnitPos);

			return true;
		}
	}

	return false;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

GatherOrder::GatherOrder(Unit* parHostUnit, Unit* parReceiverUnit, Unit* parDstUnit)
	: Order(parHostUnit),
	receiverUnit_(parReceiverUnit),
	dstUnit_(parDstUnit),
	hasResource_(false)
{
	assert(parHostUnit && parHostUnit->Type() == EUT_PEON);
	assert(parReceiverUnit && parReceiverUnit->Type() == EUT_TOWN_HALL);
	assert(parDstUnit && parDstUnit->Type() == EUT_MINE);

	moveOrder_ = new MoveOrder(hostUnit_, hostUnit_->Pos());
}

// ============================================================================

GatherOrder::~GatherOrder()
{
	if (moveOrder_)
		delete moveOrder_;
}

// ============================================================================

bool GatherOrder::Update(Uint32 parCurTime, Uint32 parElapsed)
{
	assert(moveOrder_);

	moveOrder_->Update(parCurTime, parElapsed);

	SDL_Rect hostBoundingBox = hostUnit_->BoundingBox();
	// Hack: increase host unit boundingbox size to check collision with destination
	// unit. Currently, no unit can actually be in collision with another (except
	// unit popping at the wrong place).
	// TODO: refactor
	Sint16 COLLISION_THRESHOLD = 3;
	hostBoundingBox.x -= COLLISION_THRESHOLD;
	hostBoundingBox.y -= COLLISION_THRESHOLD;
	hostBoundingBox.w += 2 * COLLISION_THRESHOLD;
	hostBoundingBox.h += 2 * COLLISION_THRESHOLD;

	// TODO: Factorize me (repetition)
	if (!hasResource_)
	{
		moveOrder_->SetTargetPos(dstUnit_->Pos());
		if (DoesBBoxesCollide(&hostBoundingBox, &dstUnit_->BoundingBox()))
			hasResource_ = true;
	}

	// TODO: Factorize me (repetition)
	if (hasResource_)
	{
		moveOrder_->SetTargetPos(receiverUnit_->Pos());
		if (DoesBBoxesCollide(&hostBoundingBox, &receiverUnit_->BoundingBox()))
			hasResource_ = false;
	}

	return false; // never ends (until destination is consumed)
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================