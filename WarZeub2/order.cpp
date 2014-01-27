#include "order.h"
#include "unit.h"
#include "world.h"
#include "util.h"
#include "unitDesc.h"
#include "userInput.h"


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

	if (moveOrder_->Update(parCurTime, parElapsedTime))
	{
		if (!buildingUnit_)
		{
			buildingUnit_ = new Unit(buildingPos_, unitTypeToBuild_);
			World::Inst()->AddUnit(buildingUnit_);

			return true;
		}
	}

	return false;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================