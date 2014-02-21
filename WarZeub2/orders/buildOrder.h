#ifndef BUILD_ORDER_H_
#define BUILD_ORDER_H_


#include "order.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class MoveOrder;
class BuildOrder : public Order
{
public:
	BuildOrder(Unit* parHostUnit, EUnitType parUnitTypeToBuild, const float2& parPos);
	virtual ~BuildOrder();

public:
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsedTime) override;

public:
	virtual void OnCancel() override;

public:
	const Unit* BuildingUnit() const { return buildingUnit_; }

private:
	EUnitType unitTypeToBuild_;
	float2 buildingPos_;
	Unit* buildingUnit_;
	Uint32 buildingStartTime_;
	MoveOrder* moveOrder_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif