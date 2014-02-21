#ifndef TRAIN_ORDER_H_
#define TRAIN_ORDER_H_


#include "order.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class TrainOrder : public Order
{
public:
	TrainOrder(Unit* parHostUnit, EUnitType parUnitTypeToTrain);
	virtual ~TrainOrder();

public:
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsedTime) override;

public:
	virtual void OnCancel() override;

private:
	EUnitType unitTypeToTrain_;
	Unit* trainedUnit_; // only 1 unit is train per train order
	Uint32 startTime_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif