#ifndef ORDER_H_
#define ORDER_H_


#include "Util.h"
#include <SDL.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class Unit;
enum EUnitType;

class Order
{
public:
	Order(Unit* parHostUnit) : hostUnit_(parHostUnit) {}
	virtual ~Order() {}

public:
	virtual bool Update(Uint32 parElapsedTime) = 0; // Return true if the order is complete

protected:
	Unit* hostUnit_;
};

// ============================================================================

class MoveOrder : public Order
{
public:
	MoveOrder(Unit* parHostUnit, const Vec2& parTargetPos);
	virtual ~MoveOrder();

public:
	virtual bool Update(Uint32 parElapsedTime) override;

private:
	Vec2 targetPos_;
};

// ============================================================================

class TrainOrder : public Order
{
public:
	TrainOrder(Unit* parHostUnit, EUnitType parUnitTypeToTrain);
	virtual ~TrainOrder();

public:
	virtual bool Update(Uint32 parElapsedTime) override;

private:
	EUnitType unitTypeToTrain_;
	Unit* trainedUnit_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif