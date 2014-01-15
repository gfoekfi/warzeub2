#ifndef UNIT_H_
#define UNIT_H_


#include <SDL.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

enum EUnitType
{
	EUT_PEON = 0,
	EUT_GRUNT,
	EUT_AXE_THROWER,
};

// ============================================================================

enum EUnitState
{
	EUS_IDLE = 0,
	EUS_MOVE,
	EUS_ATTACK,
	EUS_HARVEST,
	EUS_GATHER_GOLD,
	EUS_GATHER_WOOD,
	EUS_DEAD
};

// ============================================================================

enum EDir
{
	DIR_N = 0,
	DIR_NE,
	DIR_E,
	DIR_SE,
	DIR_S,
	DIR_SW,
	DIR_W,
	DIR_NW,
	MAX_DIRS
};

// ============================================================================

struct Unit
{
	int posX;
	int posY;
	
	int targetPosX;
	int targetPosY;

	EDir dir;
	EUnitType type;
	EUnitState state;

	int spriteStep;
	Uint32 spriteLastTime;
};

// ============================================================================

void Update(Unit& parUnit, Uint32 parCurTime, Uint32 parElapsedTime);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif