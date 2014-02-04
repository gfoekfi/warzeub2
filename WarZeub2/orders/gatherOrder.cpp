#include "gatherOrder.h"
#include "moveOrder.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

GatherOrder::GatherOrder(Unit* parHostUnit, Unit* parReceiverUnit, Unit* parDstUnit)
	: Order(parHostUnit),
	receiverUnit_(parReceiverUnit),
	dstUnit_(parDstUnit)
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

	Unit* targetUnit = hostUnit_->IsHoldingGold() ? receiverUnit_ : dstUnit_;
	moveOrder_->SetTargetPos(targetUnit->Pos());
	if (DoesBBoxesCollide(&hostBoundingBox, &targetUnit->BoundingBox()))
		hostUnit_->SetHoldingGold(!hostUnit_->IsHoldingGold());

	return false; // never ends (until destination is consumed)
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================