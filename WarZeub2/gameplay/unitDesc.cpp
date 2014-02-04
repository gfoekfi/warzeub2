#include "unitDesc.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

std::map<EUnitType, UnitDesc> unitTypeToUnitDesc;

// ============================================================================

UnitDesc peonUnitDesc(35, 45, 2000, 4); // 4
UnitDesc gruntUnitDesc(45, 50, 5000, 12); // 6
UnitDesc mineUnitDesc(110, 110, 0, 0);
UnitDesc townHallUnitDesc(130, 130, 10000, 0);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitUnitDesc()
{
	std::set<ECommand> moveCommandSet;
	moveCommandSet.insert(EC_MOVE);
	moveCommandSet.insert(EC_STOP);

	gruntUnitDesc.unitStateToCommandSet[EUS_IDLE] = moveCommandSet;
	gruntUnitDesc.unitStateToCommandSet[EUS_MOVING] = moveCommandSet;
	gruntUnitDesc.unitStateToCommandSet[EUS_CHOOSE_DESTINATION].insert(EC_CANCEL);

	peonUnitDesc.unitStateToCommandSet = gruntUnitDesc.unitStateToCommandSet;
	peonUnitDesc.unitStateToCommandSet[EUS_IDLE].insert(EC_BUILD);
	peonUnitDesc.unitStateToCommandSet[EUS_SELECT_BUILDING].insert(EC_CANCEL);
	peonUnitDesc.unitStateToCommandSet[EUS_SELECT_BUILDING].insert(EC_BUILD_TOWN_HALL);

	// ---

	mineUnitDesc.unitStateToCommandSet[EUS_IDLE].insert(EC_NONE);

	// ---

	std::map<EUnitState, std::set<ECommand> > buildingStateToCommandSet;
	buildingStateToCommandSet[EUS_BEING_BUILD_STATE0].insert(EC_CANCEL);
	buildingStateToCommandSet[EUS_BEING_BUILD_STATE1].insert(EC_CANCEL);
	buildingStateToCommandSet[EUS_BEING_BUILD_STATE2].insert(EC_CANCEL);

	townHallUnitDesc.unitStateToCommandSet = buildingStateToCommandSet;
	townHallUnitDesc.unitStateToCommandSet[EUS_IDLE].insert(EC_TRAIN_PEON);
	townHallUnitDesc.unitStateToCommandSet[EUS_TRAINING].insert(EC_CANCEL);

	// ---

	unitTypeToUnitDesc[EUT_PEON] = peonUnitDesc;
	unitTypeToUnitDesc[EUT_GRUNT] = gruntUnitDesc;
	unitTypeToUnitDesc[EUT_MINE] = mineUnitDesc;
	unitTypeToUnitDesc[EUT_TOWN_HALL] = townHallUnitDesc;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================