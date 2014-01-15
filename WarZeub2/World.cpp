#include "World.h"
#include "Renderer.h" //only for SCREEN_WIDTH & SCREEN_HEIGHT

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Unit grunt =
{
	Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
	Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
	DIR_N,
	EUT_GRUNT,
	EUS_IDLE,
	0,
	0
};

Unit peon =
{
	Vec2(SCREEN_WIDTH/4, SCREEN_HEIGHT/4),
	Vec2(SCREEN_WIDTH/4, SCREEN_HEIGHT/4),
	DIR_N,
	EUT_PEON,
	EUS_IDLE,
	0,
	0
};

Unit mine = 
{
	Vec2(3*SCREEN_WIDTH/4, SCREEN_HEIGHT/4),
	Vec2(0, 0),
	DIR_N,
	EUT_MINE,
	EUS_IDLE,
	0,
	0
};

Unit townHall =
{
	Vec2(3*SCREEN_WIDTH/4, 3*SCREEN_HEIGHT/4),
	Vec2(0, 0),
	DIR_N,
	EUT_TOWN_HALL,
	EUS_IDLE,
	0,
	0
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================