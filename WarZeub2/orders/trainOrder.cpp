#include "trainOrder.h"
#include "../gameplay/world.h"
#include "../gameplay/path.h"
#include "../gameplay/workerUnit.h"
#include "../gameplay/player.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

TrainOrder::TrainOrder(Unit* parHostUnit, EUnitType parUnitTypeToTrain)
	: Order(parHostUnit),
	unitTypeToTrain_(parUnitTypeToTrain),
	trainedUnit_(0),
	startTime_(-1)
{
	assert(player.GoldAmount() >= unitTypeToUnitDesc[unitTypeToTrain_].goldPrice);
	player.IncreaseGoldAmount(-int(unitTypeToUnitDesc[unitTypeToTrain_].goldPrice));
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
		// Makes the unit pop at a suitable location
		const UnitDesc& trainedUnitDesc = unitTypeToUnitDesc[unitTypeToTrain_];
		int2 trainedUnitDimensions(trainedUnitDesc.width, trainedUnitDesc.height);
		float2 newUnitPos = Path::NearestWalkableTileOf(hostUnit_->Pos(),
			hostUnit_->Pos(), trainedUnitDimensions).ToWorldPos();

		// TODO: Use factory
		switch (unitTypeToTrain_)
		{
		case EUT_PEON: trainedUnit_ = new WorkerUnit(newUnitPos, unitTypeToTrain_); break;
		default: trainedUnit_ = new Unit(newUnitPos, unitTypeToTrain_); break;
		}

		World::Inst()->AddUnit(trainedUnit_);
	}

	hostUnit_->SetActionState(EUS_IDLE);

	return true;
}

// ============================================================================

void TrainOrder::OnCancel()
{
	assert(hostUnit_->ActionState() == EUS_TRAINING);
	assert(!trainedUnit_);

	player.IncreaseGoldAmount(unitTypeToUnitDesc[unitTypeToTrain_].goldPrice);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================