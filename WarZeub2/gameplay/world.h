#ifndef WORLD_H_
#define WORLD_H_


#include "unit.h"
#include "../utils/singleton.h"
#include <vector>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

const size_t MAX_NB_TILES = 64;
const size_t MAP_TILE_SIZE = 32;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class World : public Singleton<World>
{
public:
	World();
	~World();

public:
	static void TileAlign(float2& parPos); // center pos to current tile

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
	size_t Width() const { return width_; }
	size_t Height() const { return height_; }

private:
	std::vector<Unit*> units_;

	size_t width_; // in # of tiles
	size_t height_; // in # of tiles
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif