#ifndef UNIT_DESC_H_
#define UNIT_DESC_H_


#include <map>
#include <set>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

// TODO: Remove this, unused bit mask

enum ECommand
{
	EC_NONE			= 0,
	EC_STOP			= 1 << 0,
	EC_CANCEL		= 1 << 1,
	EC_TRAIN_PEON	= 1 << 2,
	EC_MOVE			= 1 << 3,
	EC_BUILD			= 1 << 4,
	EC_BUILD_TOWN_HALL = 1 << 5,
	EC_BUILD_FARM = 1 << 6,
	EC_BUILD_BARRACK = 1 << 7,
	EC_TRAIN_GRUNT = 1 << 8,
};

// ============================================================================

enum EUnitType
{
	EUT_PEON = 0,
	EUT_GRUNT,
	EUT_MINE,
	EUT_TOWN_HALL,
	EUT_FARM,
	EUT_BARRACK,
};

// ============================================================================

enum EUnitState
{
	EUS_IDLE = 0,
	EUS_IDLE_WITH_GOLD,
	EUS_MOVING,
	EUS_MOVING_WITH_GOLD,

	EUS_ATTACK,
	EUS_HARVEST,

	EUS_GATHER_WOOD,
	EUS_DEAD,
	EUS_TRAINING,
	EUS_BUILDING,

	EUS_BEING_BUILD_STATE0,
	EUS_BEING_BUILD_STATE1,
	EUS_BEING_BUILD_STATE2,

	EUS_SELECT_BUILDING, // TODO: Should be a 'hud state'
	EUS_CHOOSE_DESTINATION, // TODO: Should be a 'hud state'
};

// ============================================================================

struct UnitDesc
{
	UnitDesc(int parWidth, int parHeight, int parBuildTime, int parMoveSpeed) :
		width(parWidth),
		height(parHeight),
		buildTime(parBuildTime),
		moveSpeed(parMoveSpeed)
	{
	}

	UnitDesc() :
		width(0),
		height(0),
		buildTime(0),
		moveSpeed(0)
	{
	}

	int width;
	int height;
	int buildTime; // in ms
	int moveSpeed;

	std::map<EUnitState, std::set<ECommand> > unitStateToCommandSet;
};

// ============================================================================

void InitUnitDesc();

// ============================================================================

extern std::map<EUnitType, UnitDesc> unitTypeToUnitDesc;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif