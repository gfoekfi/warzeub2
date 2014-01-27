#include "order.h"
#include "unit.h"
#include "world.h"
#include "util.h"
#include "unitDesc.h"
#include "userInput.h"
#include "player.h"


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

	if ((parCurTime - startTime_) < (Uint32)unitTypeToUnitDesc[unitTypeToTrain_].buildTime)
	{
		hostUnit_->SetActionState(EUS_TRAINING);
		return false;
	}

	if (!trainedUnit_)
	{
		Vec2 newUnitPos(hostUnit_->Pos());
		newUnitPos.y += unitTypeToUnitDesc[hostUnit_->Type()].height / 2 + 
			unitTypeToUnitDesc[unitTypeToTrain_].height / 2;
		trainedUnit_ = new Unit(newUnitPos, unitTypeToTrain_);

		World::Inst()->AddUnit(trainedUnit_);
	}

	hostUnit_->SetActionState(EUS_IDLE);

	return true;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

MoveOrder::MoveOrder(Unit* parHostUnit, const Vec2& parTargetPos)
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
	int deltaPosX = targetPos_.x - hostUnit_->Pos().x;
	int deltaPosY = targetPos_.y - hostUnit_->Pos().y;

	// Hack to force the convergence to target
	if (abs(deltaPosX) <= 3)
		targetPos_.x = hostUnit_->Pos().x;
	if (abs(deltaPosY) <= 3)
		targetPos_.y = hostUnit_->Pos().y;

	// Temporary hack that allows grunt and peon only to move
	if (hostUnit_->Type() == EUT_PEON || hostUnit_->Type() == EUT_GRUNT)
		hostUnit_->SetMoveState((abs(deltaPosX) > 3 || abs(deltaPosY) > 3) ? EUS_MOVE : EUS_IDLE);
	else
		hostUnit_->SetMoveState(EUS_IDLE);

	if (hostUnit_->MoveState() == EUS_MOVE)
	{
		hostUnit_->SetDir(DirectionToTarget(hostUnit_->Pos(), targetPos_));

		int velX = dirs[hostUnit_->Dir()].x * (parElapsedTime / 3);
		int velY = dirs[hostUnit_->Dir()].y * (parElapsedTime / 3);
		if (abs(velX) > abs(deltaPosX))
			velX = deltaPosX;
		if (abs(velY) > abs(deltaPosY))
			velY = deltaPosY;
		
		Vec2 newPos = hostUnit_->Pos();
		newPos.x += velX;
		newPos.y += velY;
		hostUnit_->SetPos(newPos);
	}

	return (hostUnit_->Pos() == targetPos_);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

BuildOrder::BuildOrder(Unit *parHostUnit, EUnitType parUnitTypeToBuild, const Vec2 &parPos)
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

		buildingUnit_ = new Unit(buildingPos_, unitTypeToBuild_);
		buildingUnit_->SetMoveState(EUS_BEING_BUILD_STATE0);
		buildingUnit_->SetActionState(EUS_BEING_BUILD_STATE0);

		World::Inst()->AddUnit(buildingUnit_);

		hostUnit_->SetActionState(EUS_BUILDING);
		player.selectedUnit = buildingUnit_; // force the selection change
	}
	else
	{
		Uint32 totalBuildTime = unitTypeToUnitDesc[unitTypeToBuild_].buildTime;
		Uint32 deltaTime = parCurTime - buildingStartTime_;
		if (deltaTime < (totalBuildTime / 3))
		{
			buildingUnit_->SetMoveState(EUS_BEING_BUILD_STATE0); // FIXME: Bad double setter
			buildingUnit_->SetActionState(EUS_BEING_BUILD_STATE0);
		}
		else if (deltaTime < (2 * totalBuildTime / 3))
		{
			buildingUnit_->SetMoveState(EUS_BEING_BUILD_STATE1); // FIXME: Bad double setter
			buildingUnit_->SetActionState(EUS_BEING_BUILD_STATE1);
		}
		else if (deltaTime < totalBuildTime)
		{
			buildingUnit_->SetMoveState(EUS_BEING_BUILD_STATE2); // FIXME: Bad double setter
			buildingUnit_->SetActionState(EUS_BEING_BUILD_STATE2);
		}
		else
		{
			buildingUnit_->SetMoveState(EUS_IDLE); 
			buildingUnit_->SetActionState(EUS_IDLE);
			hostUnit_->SetActionState(EUS_IDLE);

			// Make the building unit pop at the bottom of the created building
			Vec2 newUnitPos(hostUnit_->Pos());
			newUnitPos.y += unitTypeToUnitDesc[unitTypeToBuild_].height / 2 +
				unitTypeToUnitDesc[hostUnit_->Type()].height / 2;
			hostUnit_->SetPos(newUnitPos);

			return true;
		}
	}

	return false;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================