#include "world.h"
#include "workerUnit.h"
#include "../graphic/renderer.h" //only for SCREEN_WIDTH & SCREEN_HEIGHT
#include <algorithm> // std::find
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

World::World()
	: width_(30),
	height_(20)
{
	for (size_t x = 0; x < width_; ++x)
		for (size_t y = 0; y < height_; ++y)
			accessibleTile_[x][y] = true;

	AddUnit(new WorkerUnit(float2(SCREEN_WIDTH/4, SCREEN_HEIGHT/4), EUT_PEON));
	AddUnit(new Unit(float2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2), EUT_GRUNT));

	float2 minePos(3*SCREEN_WIDTH/4, SCREEN_HEIGHT/4);
	TileAlign(minePos);
	AddUnit(new Unit(minePos, EUT_MINE));

	float2 townhallPos(3*SCREEN_WIDTH/4, 3*SCREEN_HEIGHT/4);
	TileAlign(townhallPos);
	AddUnit(new Unit(townhallPos, EUT_TOWN_HALL));

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

void World::TileAlign(float2& parPos)
{
	parPos.x = float(size_t(parPos.x) / MAP_TILE_SIZE) * MAP_TILE_SIZE + 0.5f * MAP_TILE_SIZE;
	parPos.y = float(size_t(parPos.y) / MAP_TILE_SIZE) * MAP_TILE_SIZE + 0.5f * MAP_TILE_SIZE;
}

// ============================================================================

void World::AddUnit(Unit* parUnit)
{
	fprintf(stdout, "[WORLD] Adding unit\n");

	units_.push_back(parUnit);

	if (parUnit->CanMove())
		return; // no collision with movable unit right now

	// mark inaccessible all tiles where unit bounding box collides
	SDL_Rect bbox = parUnit->BoundingBox();
	for (size_t x = (bbox.x / MAP_TILE_SIZE); x < ((bbox.x + bbox.w) / MAP_TILE_SIZE); ++x)
	{
		for (size_t y = (bbox.y / MAP_TILE_SIZE); y < ((bbox.y + bbox.h) / MAP_TILE_SIZE); ++y)
		{
			assert(accessibleTile_[x][y]);
			accessibleTile_[x][y] = false;
		}
	}

#if 0
	DumpAccessibleTile_();
#endif
}

// ============================================================================

void World::DumpAccessibleTile_() const
{
	fprintf(stdout, "[WORLD] accessibleTile_:\n");
	for (size_t y = 0; y < height_; ++y)
	{
		for (size_t x = 0; x < width_; ++x)
			fprintf(stdout, "%d", accessibleTile_[x][y] ? 0 : 1);
		fprintf(stdout, "\n");
	}
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

Unit* World::BuilderOf(const Unit* parUnit) const
{
	assert(parUnit);
	assert(parUnit->IsBeingConstructed());

	for (size_t unit = 0; unit < units_.size(); ++unit)
		if (units_[unit]->IsBuilding(parUnit))
			return units_[unit];

	return 0;
}

// ============================================================================

// TODO: Optimize search with Tree
Unit* World::NearestUnitOf(const Unit* parUnit, EUnitType parUnitType)
{
	assert(parUnit);

	// FIXME: Get the nearest with manhattan distance

	for (size_t unit = 0; unit < units_.size(); ++unit)
		if (units_[unit]->Type() == parUnitType)
			return units_[unit];

	return 0;
}

// ============================================================================

// TODO: Optimize search with Tree
Unit* World::GetUnitAt(const float2& parPos)
{
	// TODO: assert on 'map dimension' and 'parPos'

	Unit* dstUnit = 0;
	for (size_t unit = 0; unit < units_.size(); ++unit)
	{
		Unit* otherUnit = units_[unit];
		SDL_Rect dstBoundingBox = { Sint16(parPos.x) - 1, Sint16(parPos.y) - 1, 3, 3 };
		if (DoesBBoxesCollide(&dstBoundingBox, &otherUnit->BoundingBox()))
		{
			assert(dstUnit == 0); // units should never be stacked
			dstUnit = otherUnit;
		}
	}

	return dstUnit;
}

// ============================================================================

// TODO: Optimize search with Tree
bool World::Collides(const Unit* parUnit, SDL_Rect& parDst) const
{
	assert(parUnit);

	for (size_t unit = 0; unit < units_.size(); ++unit)
	{
		const Unit* otherUnit = units_[unit];
		if (otherUnit == parUnit) // no self collision
			continue;

		// no collision on units ATM
		if (otherUnit->Type() == EUT_PEON ||otherUnit->Type() == EUT_GRUNT)
			continue;

		if (DoesBBoxesCollide(&parDst, &otherUnit->BoundingBox()))
			return true;
	}

	return false;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================