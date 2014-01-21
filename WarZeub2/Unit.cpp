#include "Unit.h"
#include "Util.h"
#include "Order.h"
#include "UnitDesc.h"
#include <stdlib.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Unit::Unit(const Vec2& parPos, EUnitType parType)
:
pos_(parPos),
dir_(DIR_N),
type_(parType),
state_(EUS_IDLE),
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
	UpdateOrder_(parElapsedTime);
	UpdateAnimation_(parCurTime);
}

// ============================================================================

void Unit::UpdateOrder_(Uint32 parElapsedTime)
{
	if (!curOrder_)
		return;

	if (curOrder_->Update(parElapsedTime))
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

bool Unit::Move(const Vec2& parTargetPos)
{
	if (curOrder_ || !IsMovable_())
		return false;

	curOrder_ = new MoveOrder(this, parTargetPos);

	return true;
}

// ============================================================================

SDL_Rect Unit::BoundingBox() const
{
	const UnitDesc& unitDesc = unitTypeToUnitDesc[type_];

	SDL_Rect boundingBox = { 
		pos_.x - unitDesc.width / 2,
		pos_.y - unitDesc.height / 2, 
		unitDesc.width, 
		unitDesc.height 
	};

	return boundingBox;
}

// ============================================================================

bool Unit::IsMovable_() const
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
// ----------------------------------------------------------------------------
// ============================================================================