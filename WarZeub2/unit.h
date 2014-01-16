#ifndef UNIT_H_
#define UNIT_H_


#include "Util.h"
#include <SDL.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

enum EUnitType
{
	EUT_PEON = 0,
	EUT_GRUNT,
	EUT_AXE_THROWER,
	EUT_MINE,
	EUT_TOWN_HALL,
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

enum EOrder
{
	EO_NONE		= 0,
	EO_STOP		= 1,
	EO_CANCEL	= 1 << 1
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
	Vec2 pos;
	Vec2 targetPos;

	EDir dir;
	EUnitType type;
	EUnitState state;

	int spriteStep;
	Uint32 spriteLastTime;
};

// ============================================================================

void Update(Unit& parUnit, Uint32 parCurTime, Uint32 parElapsedTime);

SDL_Rect BoundingBoxFromUnit(const Unit& parUnit);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif