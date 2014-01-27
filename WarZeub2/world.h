#ifndef WORLD_H_
#define WORLD_H_


#include "unit.h"
#include "singleton.h"
#include <vector>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

const size_t MAP_TILE_SIZE = 32;

struct Map
{
	int width; // in number of tiles
	int height;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class World : public Singleton<World>
{
public:
	World();
	~World();

public:
	void Update(Uint32 parCurTime, Uint32 parElapsedTime);
	void AddUnit(Unit* parUnit); // Take the ownership
	void RemoveUnit(Unit* parUnit);

public:
	const std::vector<Unit*>& Units() const { return units_; }
	const Map& GetMap() const { return *map_; }

private:
	std::vector<Unit*> units_;
	Map* map_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif