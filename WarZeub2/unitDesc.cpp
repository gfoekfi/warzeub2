#include "unitDesc.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

std::map<EUnitType, UnitDesc> unitTypeToUnitDesc;

// ============================================================================

UnitDesc peonUnitDesc(35, 45, 2000);
UnitDesc gruntUnitDesc(45, 50, 5000);
UnitDesc mineUnitDesc(110, 110, 0);
UnitDesc townHallUnitDesc(130, 130, 10000);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitUnitDesc()
{
	std::set<EOrder> moveOrderSet;
	moveOrderSet.insert(EO_MOVE);
	moveOrderSet.insert(EO_STOP);

	gruntUnitDesc.unitStateToOrderSet[EUS_IDLE] = moveOrderSet;
	gruntUnitDesc.unitStateToOrderSet[EUS_MOVE] = moveOrderSet;
	gruntUnitDesc.unitStateToOrderSet[EUS_CHOOSE_DESTINATION].insert(EO_CANCEL);

	peonUnitDesc.unitStateToOrderSet = gruntUnitDesc.unitStateToOrderSet;
	peonUnitDesc.unitStateToOrderSet[EUS_IDLE].insert(EO_BUILD);
	peonUnitDesc.unitStateToOrderSet[EUS_SELECT_BUILDING].insert(EO_CANCEL);
	peonUnitDesc.unitStateToOrderSet[EUS_SELECT_BUILDING].insert(EO_BUILD_TOWN_HALL);

	mineUnitDesc.unitStateToOrderSet[EUS_IDLE].insert(EO_NONE);

	townHallUnitDesc.unitStateToOrderSet[EUS_IDLE].insert(EO_TRAIN_PEON);
	townHallUnitDesc.unitStateToOrderSet[EUS_TRAINING].insert(EO_CANCEL);
	townHallUnitDesc.unitStateToOrderSet[EUS_BEING_BUILD_STATE0].insert(EO_CANCEL);
	townHallUnitDesc.unitStateToOrderSet[EUS_BEING_BUILD_STATE1].insert(EO_CANCEL);
	townHallUnitDesc.unitStateToOrderSet[EUS_BEING_BUILD_STATE2].insert(EO_CANCEL);

	// ---

	unitTypeToUnitDesc[EUT_PEON] = peonUnitDesc;
	unitTypeToUnitDesc[EUT_GRUNT] = gruntUnitDesc;
	unitTypeToUnitDesc[EUT_MINE] = mineUnitDesc;
	unitTypeToUnitDesc[EUT_TOWN_HALL] = townHallUnitDesc;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================