#ifndef ORDER_H_
#define ORDER_H_


#include "../utils/util.h"
#include "../gameplay/unitDesc.h"
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
	virtual void OnCancel() {}

public:
	float CompletionStatus() const { return completionStatus_; }

protected:
	Unit* hostUnit_;
	float completionStatus_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif