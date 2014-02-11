#ifndef WORLD_H_
#define WORLD_H_


#include "unit.h"
#include "buildTile.h"
#include "walkTile.h"
#include "../utils/singleton.h"
#include <vector>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

#define MAX_NB_TILES 64
#define MAX_WALK_TILES (MAX_NB_TILES * (BUILD_TILE_SIZE / WALK_TILE_SIZE))

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class World : public Singleton<World>
{
public:
	World();
	~World();

public:
	void Init();
	void Update(Uint32 parCurTime, Uint32 parElapsedTime);
	void AddUnit(Unit* parUnit); // Take the ownership
	void RemoveUnit(Unit* parUnit);

public:
	Unit* GetUnitAt(const float2& parPos) const;
	Unit* NearestUnitOf(const Unit* parUnit, EUnitType parUnitType) const;
	Unit* BuilderOf(const Unit* parUnit) const;
	WalkTile NearestWalkableTileOf(const float2& parDstPos,
											 const float2& parSrcPos,
											 const int2& parDimensions) const;

public:
	bool Collides(const Unit* parUnit, SDL_Rect& parDst) const; // unit parameter to ignore self collision

public:
	const std::vector<Unit*>& Units() const { return units_; }

	size_t Width() const { return width_; } // in BuildTile
	size_t Height() const { return height_; }

	bool IsBuildTileAccessible(const BuildTile& parBuildTile) { return accessibleTile_[parBuildTile.x()][parBuildTile.y()]; }
	// Is tile accessible in each point for an object with dimensions 'parDimensions' ?
	bool IsBuildTileAccessible(const BuildTile& parBuildTile, const int2& parDimensions) const;

	bool IsWalkable(const WalkTile& parWalkTile) const { return isWalkable_[parWalkTile.x()][parWalkTile.y()]; }
	bool IsWalkable(const WalkTile& parWalkTile, const int2& parDimensions) const;

#ifdef _DEBUG
	void RenderAccessibleTiles(EUnitType parUnitType) const;
	void GenerateAccessibleTileSurface(EUnitType parUnitType);

	void RenderWalkableTiles(EUnitType parUnitType) const;
	void GenerateWalkableTileSurface(EUnitType parUnitType);
#endif

private:
	void UpdateAccessibleTileFromUnit_(const Unit& parUnit, bool parAccessibleState);
	void UpdateWalkableStateFromUnit_(const Unit& parUnit, bool parWalkableState);
	void DumpAccessibleTile_() const;

private:
	std::vector<Unit*> units_;

	size_t width_; // in # of build tiles
	size_t height_; // in # of build tiles
	bool accessibleTile_[MAX_NB_TILES][MAX_NB_TILES]; // Use C-style for performance (security warning)
	bool isWalkable_[MAX_WALK_TILES][MAX_WALK_TILES]; // Use C-style for performance (security warning)
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif