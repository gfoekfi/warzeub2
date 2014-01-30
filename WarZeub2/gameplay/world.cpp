#include "world.h"
#include "../graphic/renderer.h" //only for SCREEN_WIDTH & SCREEN_HEIGHT
#include <algorithm> // std::find
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

World::World()
{
	map_ = new Map();
	map_->width = 30;
	map_->height = 30;

	units_.push_back(new Unit(float2(SCREEN_WIDTH/4, SCREEN_HEIGHT/4), EUT_PEON));
	units_.push_back(new Unit(float2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), EUT_GRUNT));
	units_.push_back(new Unit(float2(3*SCREEN_WIDTH/4, SCREEN_HEIGHT/4), EUT_MINE));
	units_.push_back(new Unit(float2(3*SCREEN_WIDTH/4, 3*SCREEN_HEIGHT/4), EUT_TOWN_HALL));

#if 0
	// used to kill performance
	for (int i = 0; i < 40; ++i)
		units_.push_back(new Unit(float2(3*SCREEN_WIDTH/4 - i*50, 3*SCREEN_HEIGHT/4), EUT_TOWN_HALL));
#endif
}

// ============================================================================

World::~World()
{
	for (size_t unit = 0; unit < units_.size(); ++unit)
	{
		if (units_[unit])
			delete units_[unit];
	}

	units_.clear();
}

// ============================================================================

void World::Update(Uint32 parCurTime, Uint32 parElapsedTime)
{
	for (size_t unit = 0; unit < units_.size(); ++unit)
		units_[unit]->Update(parCurTime, parElapsedTime);
}

// ============================================================================

void World::AddUnit(Unit* parUnit)
{
	units_.push_back(parUnit);
}

// ============================================================================

void World::RemoveUnit(Unit* parUnit)
{
	if (parUnit->IsBeingConstructed())
	{
		Unit* builder = BuilderOf(parUnit);
		builder->CancelOrder();
	}

	units_.erase(std::find(units_.begin(), units_.end(), parUnit));

	delete parUnit;
}

// ============================================================================

Unit* World::BuilderOf(Unit* parUnit)
{
	assert(parUnit);
	assert(parUnit->IsBeingConstructed());

	for (size_t unit = 0; unit < units_.size(); ++unit)
		if (units_[unit]->IsBuilding(parUnit))
			return units_[unit];

	return 0;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================