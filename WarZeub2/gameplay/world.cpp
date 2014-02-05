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
	BuildTileAlign(minePos);
	AddUnit(new Unit(minePos, EUT_MINE));

	float2 townhallPos(3*SCREEN_WIDTH/4, 3*SCREEN_HEIGHT/4);
	BuildTileAlign(townhallPos);
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

void World::BuildTileAlign(float2& parPos)
{
	parPos.x = float(size_t(parPos.x) / MAP_BUILD_TILE_SIZE) * MAP_BUILD_TILE_SIZE + 0.5f * MAP_BUILD_TILE_SIZE;
	parPos.y = float(size_t(parPos.y) / MAP_BUILD_TILE_SIZE) * MAP_BUILD_TILE_SIZE + 0.5f * MAP_BUILD_TILE_SIZE;
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
	for (size_t x = (bbox.x / MAP_BUILD_TILE_SIZE); x < ((bbox.x + bbox.w) / MAP_BUILD_TILE_SIZE); ++x)
	{
		for (size_t y = (bbox.y / MAP_BUILD_TILE_SIZE); y < ((bbox.y + bbox.h) / MAP_BUILD_TILE_SIZE); ++y)
		{
			// FIXME: Bound checking (security warning)
			assert(accessibleTile_[x][y]);
			accessibleTile_[x][y] = false;
		}
	}

#if 1
	DumpAccessibleTile_();
#endif
}

// ============================================================================

void World::RemoveUnit(Unit* parUnit)
{
	assert(parUnit);

	fprintf(stdout, "[WORLD] Removing unit\n");

	if (parUnit->IsBeingConstructed())
	{
		Unit* builder = BuilderOf(parUnit);
		builder->CancelOrder();
	}

	if (!parUnit->CanMove())
	{
		SDL_Rect bbox = parUnit->BoundingBox();
		for (size_t x = (bbox.x / MAP_BUILD_TILE_SIZE); x < ((bbox.x + bbox.w) / MAP_BUILD_TILE_SIZE); ++x)
		{
			for (size_t y = (bbox.y / MAP_BUILD_TILE_SIZE); y < ((bbox.y + bbox.h) / MAP_BUILD_TILE_SIZE); ++y)
			{
				// FIXME: Bound checking (security warning)
				assert(!accessibleTile_[x][y]);
				accessibleTile_[x][y] = true;
			}
		}
	}

#if 1
	DumpAccessibleTile_();
#endif

	units_.erase(std::find(units_.begin(), units_.end(), parUnit));
	delete parUnit;
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

bool World::IsBuildTileAccessible(const int2& parTilePos, const int2& parDimensions) const
{
	assert(parTilePos.x >= 0 && parTilePos.x < int(width_));
	assert(parTilePos.y >= 0 && parTilePos.y < int(height_));

	if (!accessibleTile_[parTilePos.x][parTilePos.y])
		return false;

	const int2& minTile = int2(
		parTilePos.x - (parDimensions.w / MAP_BUILD_TILE_SIZE),
		parTilePos.y - (parDimensions.h / MAP_BUILD_TILE_SIZE));
	const int2& maxTile = int2(
		parTilePos.x + (parDimensions.w / MAP_BUILD_TILE_SIZE),
		parTilePos.y + (parDimensions.h / MAP_BUILD_TILE_SIZE));
	for (int x = minTile.x; x <= maxTile.x; ++x)
		for (int y = minTile.y; y <= maxTile.y; ++y)
			if (x < 0 || x >= int(width_) || y < 0 || y >= int(height_) || !accessibleTile_[x][y])
				return false;

	return true;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================