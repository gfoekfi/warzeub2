#ifndef MOVE_ORDER_H_
#define MOVE_ORDER_H_


#include "order.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class MoveOrder : public Order
{
public:
	MoveOrder(Unit* parHostUnit, const float2& parTargetPos);
	virtual ~MoveOrder();

public:
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsedTime) override;

public:
	void SetTargetPos(const float2& parTargetPos) { targetPos_ = parTargetPos; }

private:
	float2 targetPos_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif