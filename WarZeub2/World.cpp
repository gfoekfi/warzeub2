#include "World.h"
#include "Renderer.h" //only for SCREEN_WIDTH & SCREEN_HEIGHT


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

std::vector<Unit*> units;
Map map;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitWorld()
{
	units.push_back(new Unit(Vec2(SCREEN_WIDTH/4, SCREEN_HEIGHT/4), EUT_PEON));
	units.push_back(new Unit(Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), EUT_GRUNT));
	units.push_back(new Unit(Vec2(3*SCREEN_WIDTH/4, SCREEN_HEIGHT/4), EUT_MINE));
	units.push_back(new Unit(Vec2(3*SCREEN_WIDTH/4, 3*SCREEN_HEIGHT/4), EUT_TOWN_HALL));

	map.width = 25;
	map.height = 19;
}

// ============================================================================

void UpdateWorld(Uint32 parCurTime, Uint32 parElapsedTime)
{
	for (size_t unit = 0; unit < units.size(); ++unit)
		Update(*units[unit], parCurTime, parElapsedTime);
}

// ============================================================================

void ReleaseWorld()
{
	for (size_t unit = 0; unit < units.size(); ++unit)
	{
		if (units[unit])
			delete units[unit];
	}

	units.clear();
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
