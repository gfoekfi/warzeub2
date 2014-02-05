#include "workerUnit.h"
#include "../graphic/spriteDesc.h"
#include "../graphic/renderer.h"
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

WorkerUnit::WorkerUnit(const float2& parPos, EUnitType parType)
	: Unit(parPos, parType)
{
	assert(parType == EUT_PEON);
}

// ============================================================================

WorkerUnit::~WorkerUnit()
{
}

// ============================================================================

void WorkerUnit::Render() const
{
	if (actionState_ == EUS_BUILDING)  // don't render unit that are building
		return;

	EUnitState animState = EUS_IDLE;
	if (moving_)
		animState = holdingGold_ ? EUS_MOVING_WITH_GOLD : EUS_MOVING;
	else
		animState = holdingGold_ ? EUS_IDLE_WITH_GOLD : EUS_IDLE;

	const SpriteDesc& spriteDesc = unitTypeStateToSpriteDesc[type_][animState];
	::Render(unitTypeToImage[type_], spriteDesc, pos_, dir_, spriteStep_);
}

// ============================================================================

void WorkerUnit::RightClick(Unit* parTargetUnit)
{
	if (parTargetUnit->Type() == EUT_MINE)
		Gather(parTargetUnit);
	else
	{
		assert(CanMove());
		fprintf(stdout, "[WORKER_UNIT] RightClick (target)\n");
		Move(parTargetUnit->Pos());
	}
}

// ============================================================================

void WorkerUnit::RightClick(const float2& parTargetpos)
{
	assert(CanMove());
	fprintf(stdout, "[WORKER_UNIT] RightClick (pos)\n");
	Move(parTargetpos);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================