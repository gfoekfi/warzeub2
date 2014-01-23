#include "order.h"
#include "unit.h"
#include "world.h"
#include "util.h"
#include "unitDesc.h"
#include "userInput.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

std::map<EOrder, int> orderToGridPos;
std::map<int, std::set<EOrder> > gridPosToOrders;

void InitOrderGridPosMapping()
{
	orderToGridPos[EO_MOVE] = 0;
	orderToGridPos[EO_STOP] = 1;
	orderToGridPos[EO_CANCEL] = 8;
	orderToGridPos[EO_TRAIN_PEON] = 6;

	for (std::map<EOrder, int>::iterator it = orderToGridPos.begin();
			it != orderToGridPos.end(); ++it)
	{
		gridPosToOrders[it->second].insert(it->first);
	}
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

TrainOrder::TrainOrder(Unit* parHostUnit, EUnitType parUnitTypeToTrain)
	: Order(parHostUnit),
	unitTypeToTrain_(parUnitTypeToTrain),
	trainedUnit_(0)
{
}

// ============================================================================

TrainOrder::~TrainOrder()
{
}

// ============================================================================

bool TrainOrder::Update(Uint32 parElapsedTime)
{
	if (!trainedUnit_)
	{
		Vec2 newUnitPos(hostUnit_->Pos());
		newUnitPos.y += unitTypeToUnitDesc[hostUnit_->Type()].height / 2 + 
			unitTypeToUnitDesc[unitTypeToTrain_].height / 2;
		trainedUnit_ = new Unit(newUnitPos, unitTypeToTrain_);

		World::Inst()->AddUnit(trainedUnit_);
	}

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

bool MoveOrder::Update(Uint32 parElapsedTime)
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