#ifndef ORDER_H_
#define ORDER_H_


#include "util.h"
#include "unitDesc.h"
#include <SDL.h>
#include <map>
#include <set>


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
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsedTime) = 0; // Return true if the order is complete

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
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsedTime) override;

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
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsedTime) override;

private:
	EUnitType unitTypeToTrain_;
	Unit* trainedUnit_; // only 1 unit is train by train order
	Uint32 startTime_;
};

// ============================================================================

class BuildOrder : public Order
{
public:
	BuildOrder(Unit* parHostUnit, EUnitType parUnitTypeToBuild, const Vec2& parPos);
	virtual ~BuildOrder();

public:
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsedTime) override;

private:
	EUnitType unitTypeToBuild_;
	Vec2 buildingPos_;
	Unit* buildingUnit_;
	Uint32 buildingStartTime_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif