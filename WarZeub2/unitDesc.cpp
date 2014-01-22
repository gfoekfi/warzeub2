#include "unitDesc.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

std::map<EUnitType, UnitDesc> unitTypeToUnitDesc;

// ============================================================================

const UnitDesc peonUnitDesc(35, 45, EO_MOVE | EO_STOP);
const UnitDesc gruntUnitDesc(45, 50, EO_MOVE | EO_STOP);
const UnitDesc mineUnitDesc(110, 110, EO_NONE);
const UnitDesc townHallUnitDesc(130, 130, EO_TRAIN_PEON);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitUnitDesc()
{
	unitTypeToUnitDesc[EUT_PEON] = peonUnitDesc;
	unitTypeToUnitDesc[EUT_GRUNT] = gruntUnitDesc;
	unitTypeToUnitDesc[EUT_MINE] = mineUnitDesc;
	unitTypeToUnitDesc[EUT_TOWN_HALL] = townHallUnitDesc;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================