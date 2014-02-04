#include "trainOrder.h"
#include "../gameplay/world.h"


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