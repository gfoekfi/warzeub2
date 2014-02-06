#ifndef WORLD_H_
#define WORLD_H_


#include "unit.h"
#include "buildTile.h"
#include "../utils/singleton.h"
#include <vector>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

#define MAX_NB_TILES 64

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class World : public Singleton<World>
{
public:
	World();
	~World();

public:
	static void BuildTileAlign(float2& parPos, EUnitType parUnitType); // center pos to current build tile

public:
	void Update(Uint32 parCurTime, Uint32 parElapsedTime);
	void AddUnit(Unit* parUnit); // Take the ownership
	void RemoveUnit(Unit* parUnit);

public:
	Unit* GetUnitAt(const float2& parPos);
	Unit* NearestUnitOf(const Unit* parUnit, EUnitType parUnitType);
	Unit* BuilderOf(const Unit* parUnit) const;

public:
	bool Collides(const Unit* parUnit, SDL_Rect& parDst) const; // unit parameter to ignore self collision

public:
	const std::vector<Unit*>& Units() const { return units_; }

	size_t Width() const { return width_; } // in BuildTile
	size_t Height() const { return height_; }

	bool IsBuildTileAccessible(const BuildTile& parBuildTile) { return accessibleTile_[parBuildTile.x()][parBuildTile.y()]; }
	// Is tile accessible in each point for an object with dimensions 'parDimensions' ?
	bool IsBuildTileAccessible(const BuildTile& parBuildTile, const int2& parDimensions) const;

#ifdef _DEBUG
	void RenderAccessibleTiles(EUnitType parUnitType) const;
	void GenerateAccessibleTileSurface(EUnitType parUnitType);
#endif

private:
	void UpdateAccessibleTileFromUnit_(const Unit& parUnit, bool parAccessibleState);
	void DumpAccessibleTile_() const;

private:
	std::vector<Unit*> units_;

	size_t width_; // in # of tiles
	size_t height_; // in # of tiles
	bool accessibleTile_[MAX_NB_TILES][MAX_NB_TILES]; // Use C-style for performance (security warning)
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif