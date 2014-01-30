#include "unit.h"
#include "order.h"
#include "unitDesc.h"
#include <stdlib.h>
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Unit::Unit(const float2& parPos, EUnitType parType)
	: pos_(parPos),
	dir_(DIR_N),
	type_(parType),
	moveState_(EUS_IDLE),
	actionState_(EUS_IDLE),
	spriteStep_(0),
	spriteLastTime_(0),
	curOrder_(0)
{
}

// ============================================================================

Unit::~Unit()
{
	if (curOrder_)
		delete curOrder_;
}

// ============================================================================

void Unit::Update(Uint32 parCurTime, Uint32 parElapsedTime)
{
	UpdateOrder_(parCurTime, parElapsedTime);
	UpdateAnimation_(parCurTime);
}

// ============================================================================

void Unit::UpdateOrder_(Uint32 parCurTime, Uint32 parElapsedTime)
{
	if (!curOrder_)
		return;

	if (curOrder_->Update(parCurTime, parElapsedTime))
	{
		delete curOrder_;
		curOrder_ = 0;
	}
}

// ============================================================================

void Unit::UpdateAnimation_(Uint32 parCurTime)
{
	Uint32 deltaTime = parCurTime - spriteLastTime_;
	if (deltaTime > 180) // 180 = grunt
	{
		spriteStep_++;
		spriteLastTime_ = parCurTime;
	}
}

// ============================================================================

bool Unit::Train(EUnitType parUnitTypeToTrain)
{
	if (curOrder_)
		return false;

	curOrder_ = new TrainOrder(this, parUnitTypeToTrain);

	return true;
}

// ============================================================================

bool Unit::Move(const float2& parTargetPos)
{
	assert(IsMovable());

	if (curOrder_)
		delete curOrder_;

	curOrder_ = new MoveOrder(this, parTargetPos);

	return true;
}

// ============================================================================

bool Unit::Build(EUnitType parUnitTypeToBuild, const float2& parPos)
{
	if (curOrder_)
		delete curOrder_;

	curOrder_ = new BuildOrder(this, parUnitTypeToBuild, parPos);

	return true;
}

// ============================================================================

bool Unit::CancelOrder()
{
	if (curOrder_)
	{
		delete curOrder_;
		curOrder_ = 0;
	}

	moveState_ = EUS_IDLE;
	actionState_ = EUS_IDLE;

	spriteStep_ = 0;

	return true;
}

// ============================================================================

float Unit::OrderCompletionStatus() const
{
	if (curOrder_)
		return curOrder_->CompletionStatus();
	return -1.0f;
}

// ============================================================================

SDL_Rect Unit::BoundingBox() const
{
	const UnitDesc& unitDesc = unitTypeToUnitDesc[type_];

	SDL_Rect boundingBox = { 
		Sint16(pos_.x) - unitDesc.width / 2,	// FIXME: Sint16 might be too small in future
		Sint16(pos_.y) - unitDesc.height / 2,
		unitDesc.width, 
		unitDesc.height 
	};

	return boundingBox;
}

// ============================================================================

bool Unit::IsMovable() const
{
	switch (type_)
	{
	case EUT_PEON:
	case EUT_GRUNT:
		return true;

	default:
		return false;
	};

	return false;
}

// ============================================================================

bool Unit::IsBeingConstructed() const
{
	return (actionState_ == EUS_BEING_BUILD_STATE0 ||
		actionState_ == EUS_BEING_BUILD_STATE1 ||
		actionState_ == EUS_BEING_BUILD_STATE2);
}

// ============================================================================

bool Unit::IsBuilding(const Unit* parUnit) const
{
	assert(parUnit);

	if (!curOrder_ || actionState_ != EUS_BUILDING)
		return false;

	const BuildOrder* buildOrder = dynamic_cast<const BuildOrder*>(curOrder_);
	return (buildOrder && buildOrder->BuildingUnit() == parUnit);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================