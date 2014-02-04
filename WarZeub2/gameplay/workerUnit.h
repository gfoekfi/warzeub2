#ifndef WORKER_UNIT_H_
#define WORKER_UNIT_H_


#include "unit.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class WorkerUnit: public Unit
{
public:
	WorkerUnit(const float2& parPos, EUnitType parType);
	virtual ~WorkerUnit();

public:
	virtual void Render() const override;

	virtual void RightClick(Unit* parTargetUnit) override;
	virtual void RightClick(const float2& parTargetpos) override;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

#endif