#include "path.h"
#include "world.h"
#include "../utils/vecTypes.h"
#include <vector>
#include <map>
#include <list>
#include <math.h>
#include <queue>
#include <set>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

struct CostWalkTile
{
	CostWalkTile() : walkTile(), cost(0.f) {}
	CostWalkTile(const WalkTile& parWalkTile, float parCost)
		: walkTile(parWalkTile),
		cost(parCost)
	{
	}

	bool operator==(const CostWalkTile& parRhs) const
	{
		return (walkTile.operator ==(parRhs.walkTile));
	}

	bool operator<(const CostWalkTile& parRhs) const
	{
		return (cost < parRhs.cost);
	}

	bool operator>(const CostWalkTile& parRhs) const
	{
		return (cost > parRhs.cost);
	}

	WalkTile walkTile;
	float cost;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Path::Path(const float2& parStartPos, const float2& parGoalPos, const int2& parEntityDimensions)
	: startPos_(parStartPos),
	goalPos_(parGoalPos),
	entityDimensions_(parEntityDimensions),
	hasPath_(false)
{
	assert(WalkTile(parStartPos) != WalkTile(parGoalPos));

	assert(World::Inst()->IsWalkable(WalkTile(startPos_), parEntityDimensions));
	assert(World::Inst()->IsWalkable(WalkTile(goalPos_), parEntityDimensions));

	ComputeShortestPath_();
}

// ============================================================================

Path::~Path()
{
}

// ============================================================================

// Breadth first search algorithm with priority queue
void Path::ComputeShortestPath_()
{
	const WalkTile startWalkTile(startPos_);
	const WalkTile goalWalkTile(goalPos_);
	assert(World::Inst()->IsWalkable(goalWalkTile, entityDimensions_));

	std::priority_queue<CostWalkTile, std::vector<CostWalkTile>, std::greater<CostWalkTile> > unvisitedTiles;
	std::map<WalkTile, WalkTile> parentsTile;

	unvisitedTiles.push(CostWalkTile(startWalkTile, 0.f));
	parentsTile[startWalkTile] = startWalkTile;
	while (!unvisitedTiles.empty())
	{
		CostWalkTile curTile = unvisitedTiles.top();
		unvisitedTiles.pop();

		if (curTile.walkTile == goalWalkTile)
		{
			hasPath_ = true;
			break;
		}

		for (int dir = 0; dir < MAX_DIRS; ++dir)
		{
			WalkTile curDir(int(dirs[dir].x), int(dirs[dir].y)); // FIXME: Shouldn't need to cast
			WalkTile nextWalkTile(curTile.walkTile + curDir);

			if (nextWalkTile.IsValid() &&
				 parentsTile.count(nextWalkTile) == 0 &&
				 World::Inst()->IsWalkable(nextWalkTile, entityDimensions_))
			{
				parentsTile[nextWalkTile] = curTile.walkTile;

				float cost = curTile.cost + 1.f;
				if (dir & 1) // WARNING: Every diagonal direction value should be *odd*
					cost += 0.2f;
				unvisitedTiles.push(CostWalkTile(nextWalkTile, cost));
			}
		}
	}

	RetrieveWalkTilePathFromParents_(parentsTile, startWalkTile, goalWalkTile);
}

// ============================================================================

void Path::RetrieveWalkTilePathFromParents_(std::map<WalkTile, WalkTile>& parParentOf,
														  const WalkTile& parStartWalkTile,
														  const WalkTile& parGoalWalkTile)
{
	fprintf(stdout, "[PATH] # of parents tile: %d\n", parParentOf.size());

	if (parParentOf.count(parGoalWalkTile) == 0)
	{
		assert(!hasPath_);
		fprintf(stdout, "[PATH] No valid path as been found between (%d, %d) and (%d, %d)\n",
			parStartWalkTile.x(), parStartWalkTile.y(), parGoalWalkTile.x(), parGoalWalkTile.y());
		return;
	}

	WalkTile curWalkTile(parGoalWalkTile);
	std::vector<WalkTile> reversePath;
	while (curWalkTile != parStartWalkTile)
	{
		reversePath.push_back(curWalkTile);
		assert(parParentOf.count(curWalkTile) > 0);
		curWalkTile = parParentOf[curWalkTile];
	}

	for (std::vector<WalkTile>::reverse_iterator tile = reversePath.rbegin();
		tile != reversePath.rend(); ++tile)
	{
		walkTilePath_.push_back(*tile);
	}

	fprintf(stdout, "[PATH] Shortest path between (%d, %d) and (%d, %d):\n",
		parStartWalkTile.x(), parStartWalkTile.y(), parGoalWalkTile.x(), parGoalWalkTile.y());
	DumpPath_(parParentOf);
}

// ============================================================================

void Path::DumpPath_(std::map<WalkTile, WalkTile>& parParentOf)
{
	assert(hasPath_);
	assert(walkTilePath_.size() >= 1);

	WalkTile startWalkTile(startPos_);
	WalkTile goalWalkTile(goalPos_);

	for (size_t y = 0; y < World::Inst()->Height() * (BUILD_TILE_SIZE / WALK_TILE_SIZE); ++y)
	{
		for (size_t x = 0; x < World::Inst()->Width() * (BUILD_TILE_SIZE / WALK_TILE_SIZE); ++x)
		{
			char c = '.';
			WalkTile tile(x, y);

			if (tile == startWalkTile)
				c = 'S';
			else if (tile == goalWalkTile)
				c = 'E';
			else if (!World::Inst()->IsWalkable(tile))
				c = '#';
			else
			{
				for (size_t waypoint = 0; waypoint < walkTilePath_.size(); ++waypoint)
					if (walkTilePath_[waypoint] == WalkTile(x, y))
					{
						c = '*';
						break;
					}
			}

			if (c == '.' && parParentOf.count(tile) > 0)
				c = 'o';

			fprintf(stdout, "%c", c);
		}
		fprintf(stdout, "\n");
	}
}

// ============================================================================

const WalkTile& Path::WalkTileFromWaypoint(size_t parWaypoint) const
{
	assert(hasPath_);
	assert(parWaypoint < walkTilePath_.size());

	return walkTilePath_[parWaypoint];
}

// ============================================================================

// FIXME: Should consider 'parSrcPos' in distance computation and should be in
// the same connected component
// TODO: Should use the PathFinding algorithms (code duplication)
WalkTile Path::NearestWalkableTileOf(const float2& parDstPos,
												  const float2& parSrcPos,
												  const int2& parDimensions)
{
	const WalkTile startWalkTile(parDstPos);

	std::priority_queue<CostWalkTile, std::vector<CostWalkTile>, std::greater<CostWalkTile> > unvisitedTiles;
	std::set<WalkTile> visitedTiles;
	unvisitedTiles.push(CostWalkTile(startWalkTile, 0.f));
	while (!unvisitedTiles.empty())
	{
		CostWalkTile curTile = unvisitedTiles.top();
		unvisitedTiles.pop();

		if (World::Inst()->IsWalkable(curTile.walkTile, parDimensions))
			return curTile.walkTile;

		for (int dir = 0; dir < MAX_DIRS; ++dir)
		{
			WalkTile curDir(int(dirs[dir].x), int(dirs[dir].y)); // FIXME: Shouldn't need to cast
			WalkTile nextWalkTile(curTile.walkTile + curDir);

			if (nextWalkTile.IsValid() && (visitedTiles.count(nextWalkTile) == 0))
			{
				visitedTiles.insert(nextWalkTile);

				float cost = curTile.cost + 1.f;
				if (dir & 1)
					cost += 0.2f;
				unvisitedTiles.push(CostWalkTile(nextWalkTile, cost));
			}
		}
	}

	return WalkTile(parSrcPos);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

PathCache::~PathCache()
{
	for (std::map<WalkTile, std::map<WalkTile, std::map<int2, Path*> > >::iterator srcIt = cache_.begin();
		srcIt != cache_.end(); ++srcIt)
	{
		for (std::map<WalkTile, std::map<int2, Path*> >::iterator dstIt = cache_[srcIt->first].begin();
			dstIt != cache_[srcIt->first].end(); ++dstIt)
		{
			for (std::map<int2, Path*>::iterator dimIt = cache_[srcIt->first][dstIt->first].begin();
				dimIt != cache_[srcIt->first][dstIt->first].end(); ++dimIt)
			{
				Path* path = dimIt->second;
				delete path;
			}
		}
	}
}

// ============================================================================

Path* PathCache::GetOrCreatePath(const WalkTile& parSrc,
											const WalkTile& parDst,
											const int2& parDimensions)
{
	if (cache_[parSrc][parDst].count(parDimensions) > 0)
		return cache_[parSrc][parDst][parDimensions];

	Path* newPath = new Path(parSrc.ToWorldPos(), parDst.ToWorldPos(), parDimensions);
	assert(newPath);

	cache_[parSrc][parDst][parDimensions] = newPath;

	return newPath;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================