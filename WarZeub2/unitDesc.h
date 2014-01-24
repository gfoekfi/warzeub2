#ifndef UNIT_DESC_H_
#define UNIT_DESC_H_


#include "unit.h"
#include <map>
#include <set>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

enum EOrder
{
	EO_NONE			= 0,
	EO_STOP			= 1 << 0,
	EO_CANCEL		= 1 << 1,
	EO_TRAIN_PEON	= 1 << 2,
	EO_MOVE			= 1 << 3,
	EO_BUILD			= 1 << 4,
	EO_BUILD_TOWN_HALL = 1 << 5,
};

// ============================================================================

struct UnitDesc
{
	UnitDesc(int parWidth, int parHeight) :
		width(parWidth),
		height(parHeight)
	{
	}

	UnitDesc() :
		width(0),
		height(0)
	{
	}

	int width;
	int height;

	std::map<EUnitState, std::set<EOrder> > unitStateToOrderSet;
};

// ============================================================================

void InitUnitDesc();

// ============================================================================

extern std::map<EUnitType, UnitDesc> unitTypeToUnitDesc;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif