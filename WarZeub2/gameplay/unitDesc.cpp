#include "unitDesc.h"
#include "world.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

std::map<EUnitType, UnitDesc> unitTypeToUnitDesc;

// ============================================================================

#ifdef _DEBUG
UnitDesc peonUnitDesc(4 * WALK_TILE_SIZE - 1, 4 * WALK_TILE_SIZE - 1, 2000, 8);
UnitDesc gruntUnitDesc(5 * WALK_TILE_SIZE - 1, 5 * WALK_TILE_SIZE - 1, 5000, 12); // 45, 50
#else
UnitDesc peonUnitDesc(4 * WALK_TILE_SIZE - 1, 4 * WALK_TILE_SIZE - 1, 2000, 4);
UnitDesc gruntUnitDesc(5 * WALK_TILE_SIZE - 1, 5 * WALK_TILE_SIZE - 1, 5000, 6);

#endif
UnitDesc mineUnitDesc(3 * BUILD_TILE_SIZE, 3 * BUILD_TILE_SIZE, 0, 0);
UnitDesc townHallUnitDesc(4 * BUILD_TILE_SIZE, 4 * BUILD_TILE_SIZE, 10000, 0);

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