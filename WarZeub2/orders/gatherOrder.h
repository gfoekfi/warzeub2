#ifndef GATHER_ORDER_H_
#define GATHER_ORDER_H_


#include "order.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class MoveOrder;
class GatherOrder : public Order
{
public:
	GatherOrder(Unit* parHostUnit, Unit* parReceiverUnit, Unit* parDstUnit);
	virtual ~GatherOrder();

public:
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsed) override;

private:
	Unit* receiverUnit_; // townhall only ATM
	Unit* dstUnit_; // mine only ATM
	Unit* curTargetUnit_; // for convenience only
	MoveOrder* moveOrder_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif