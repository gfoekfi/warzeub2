#ifndef ORDER_H_
#define ORDER_H_


#include "../utils/util.h"
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
	Order(Unit* parHostUnit) : hostUnit_(parHostUnit), completionStatus_(-1.0f) {}
	virtual ~Order() {}

public:
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsedTime) = 0; // Return true if the order is complete

public:
	float CompletionStatus() const { return completionStatus_; }

protected:
	Unit* hostUnit_;
	float completionStatus_;
};

// ============================================================================

class MoveOrder : public Order
{
public:
	MoveOrder(Unit* parHostUnit, const int2& parTargetPos);
	virtual ~MoveOrder();

public:
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsedTime) override;

private:
	int2 targetPos_;
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
	BuildOrder(Unit* parHostUnit, EUnitType parUnitTypeToBuild, const int2& parPos);
	virtual ~BuildOrder();

public:
	virtual bool Update(Uint32 parCurTime, Uint32 parElapsedTime) override;

public:
	const Unit* BuildingUnit() const { return buildingUnit_; }

private:
	EUnitType unitTypeToBuild_;
	int2 buildingPos_;
	Unit* buildingUnit_;
	Uint32 buildingStartTime_;
	MoveOrder* moveOrder_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif