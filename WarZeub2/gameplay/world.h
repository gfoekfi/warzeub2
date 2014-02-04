#ifndef WORLD_H_
#define WORLD_H_


#include "unit.h"
#include "../utils/singleton.h"
#include <vector>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class Map;
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
	Unit* GetUnitAt(const float2& parPos);
	Unit* NearestUnitOf(const Unit* parUnit, EUnitType parUnitType);
	Unit* BuilderOf(const Unit* parUnit) const;
	bool Collides(const Unit* parUnit, SDL_Rect& parDst) const; // unit parameter to ignore self collision

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