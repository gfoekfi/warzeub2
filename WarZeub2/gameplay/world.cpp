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
	BuildTileAlign(minePos, EUT_MINE);
	AddUnit(new Unit(minePos, EUT_MINE));

	float2 townhallPos(3*SCREEN_WIDTH/4, 3*SCREEN_HEIGHT/4);
	BuildTileAlign(townhallPos, EUT_TOWN_HALL);
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

void World::BuildTileAlign(float2& parPos, EUnitType parUnitType)
{
	parPos.x = float(size_t(parPos.x) / MAP_BUILD_TILE_SIZE) * MAP_BUILD_TILE_SIZE;
	parPos.y = float(size_t(parPos.y) / MAP_BUILD_TILE_SIZE) * MAP_BUILD_TILE_SIZE;

	const UnitDesc& unitDesc = unitTypeToUnitDesc[parUnitType];
	float2 dimensions(float(unitDesc.width), float(unitDesc.height));
	BuildTile buildTile = ToBuildTile(dimensions);

	if (buildTile.x & 1)
		parPos.x += 0.5f * MAP_BUILD_TILE_SIZE;
	if (buildTile.y & 1)
		parPos.y += 0.5f * MAP_BUILD_TILE_SIZE;
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

#ifdef _DEBUG
	GenerateAccessibleTileSurface(EUT_PEON);
	GenerateAccessibleTileSurface(EUT_GRUNT);
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

#ifdef _DEBUG
	GenerateAccessibleTileSurface(EUT_PEON);
	GenerateAccessibleTileSurface(EUT_GRUNT);
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

bool World::IsBuildTileAccessible(const int2& parBuildTilePos, const int2& parDimensions) const
{
	assert(parBuildTilePos.x >= 0 && parBuildTilePos.x < int(width_));
	assert(parBuildTilePos.y >= 0 && parBuildTilePos.y < int(height_));

	if (!accessibleTile_[parBuildTilePos.x][parBuildTilePos.y])
		return false;

	BuildTile dimensionInBuildTile(World::ToBuildTile(parDimensions));
	const int2& minTile = int2(
		parBuildTilePos.x - dimensionInBuildTile.w,
		parBuildTilePos.y - dimensionInBuildTile.h);
	const int2& maxTile = int2(
		parBuildTilePos.x + dimensionInBuildTile.w,
		parBuildTilePos.y + dimensionInBuildTile.h);

	for (int x = minTile.x; x <= maxTile.x; ++x)
		for (int y = minTile.y; y <= maxTile.y; ++y)
			if (x < 0 || x >= int(width_) || y < 0 || y >= int(height_) || !accessibleTile_[x][y])
				return false;

	return true;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

static std::map<EUnitType, SDL_Surface*> unitTypeToAccessibleSurface;

// ============================================================================

#ifdef _DEBUG
void World::RenderAccessibleTiles(EUnitType parUnitType) const
{
	assert(unitTypeToAccessibleSurface.count(parUnitType) == 1);

	SDL_Rect dst = { 0, 0, 0, 0 };
	TransformToScreenCoordinate(dst, gCamera->Pos());
	SDL_BlitSurface(unitTypeToAccessibleSurface[parUnitType], 0, screen, &dst);
}
#endif

// ============================================================================

#ifdef _DEBUG
void World::GenerateAccessibleTileSurface(EUnitType parUnitType)
{
	if (unitTypeToAccessibleSurface.count(parUnitType) > 0)
		SDL_FreeSurface(unitTypeToAccessibleSurface[parUnitType]);

	unitTypeToAccessibleSurface[parUnitType] = SDL_CreateRGBSurface(SDL_HWSURFACE,
		width_ * MAP_BUILD_TILE_SIZE, height_ * MAP_BUILD_TILE_SIZE,
		screen->format->BitsPerPixel,
		screen->format->Rmask, screen->format->Gmask,
		screen->format->Bmask, screen->format->Amask);
	assert(unitTypeToAccessibleSurface[parUnitType]);

	SDL_Rect dst = { 0, 0, MAP_BUILD_TILE_SIZE, MAP_BUILD_TILE_SIZE };
	int2 unitDimension(unitTypeToUnitDesc[parUnitType].width, unitTypeToUnitDesc[parUnitType].height);
	for (size_t x = 0; x < width_; ++x)
		for (size_t y = 0; y < height_; ++y)
		{
			if (!IsBuildTileAccessible(BuildTile(x, y), unitDimension))
			{
				dst.x = x * MAP_BUILD_TILE_SIZE;
				dst.y = y * MAP_BUILD_TILE_SIZE;
				SDL_FillRect(unitTypeToAccessibleSurface[parUnitType], &dst, 0x00ff0000);
			}
			else
			{
				SDL_Rect buildTileRect = { x * MAP_BUILD_TILE_SIZE, y * MAP_BUILD_TILE_SIZE, MAP_BUILD_TILE_SIZE, MAP_BUILD_TILE_SIZE};
				SDL_FillRect(unitTypeToAccessibleSurface[parUnitType], &buildTileRect, ((x + y) % 2) ? 0x000000ff : 0x00ffffff);
			}
		}
}
#endif

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================