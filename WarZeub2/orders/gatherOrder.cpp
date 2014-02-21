#include "gatherOrder.h"
#include "moveOrder.h"
#include "../gameplay/player.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

GatherOrder::GatherOrder(Unit* parHostUnit, Unit* parReceiverUnit, Unit* parDstUnit)
	: Order(parHostUnit),
	receiverUnit_(parReceiverUnit),
	dstUnit_(parDstUnit),
	curTargetUnit_(0)
{
	assert(parHostUnit && parHostUnit->Type() == EUT_PEON);
	assert(parReceiverUnit && parReceiverUnit->Type() == EUT_TOWN_HALL);
	assert(parDstUnit && parDstUnit->Type() == EUT_MINE);

	curTargetUnit_ = hostUnit_->IsHoldingGold() ? receiverUnit_ : dstUnit_;
	assert(curTargetUnit_);
	moveOrder_ = new MoveOrder(hostUnit_, curTargetUnit_->Pos());
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
	Sint16 COLLISION_THRESHOLD = 8;
	hostBoundingBox.x -= COLLISION_THRESHOLD;
	hostBoundingBox.y -= COLLISION_THRESHOLD;
	hostBoundingBox.w += 2 * COLLISION_THRESHOLD;
	hostBoundingBox.h += 2 * COLLISION_THRESHOLD;

	if (DoesBBoxesCollide(&hostBoundingBox, &curTargetUnit_->BoundingBox()))
	{
		if (hostUnit_->IsHoldingGold())
#ifdef _DEBUG
			player.IncreaseGoldAmount(25);
#else
			player.IncreaseGoldAmount(10);
#endif

		hostUnit_->ToggleHoldingGold();
	}

	Unit* nextTargetUnit = hostUnit_->IsHoldingGold() ? receiverUnit_ : dstUnit_;
	if (nextTargetUnit != curTargetUnit_)
	{
		curTargetUnit_ = nextTargetUnit;
		moveOrder_->SetTargetPos(curTargetUnit_->Pos());
	}

	return false; // never ends (until destination is consumed)
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================