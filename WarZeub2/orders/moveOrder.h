#ifndef MOVE_ORDER_H_
#define MOVE_ORDER_H_


#include "order.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class Path;
class MoveOrder : public Order
{
public:
	MoveOrder(Unit* parHostUnit, const float2& parTargetPos);
	virtual ~MoveOrder();

public:
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsedTime) override;

public:
	void SetTargetPos(const float2& parTargetPos); // Warning: might call pathfinding computation

private:
	void RecomputePathIFN_();

private:
	float2 targetPos_;
	Path* path_;
	size_t curWaypoint_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif