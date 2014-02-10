#include "path.h"
#include "world.h"
#include "../utils/vecTypes.h"
#include <vector>
#include <map>
#include <list>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Path::Path(const float2& parStartPos, const float2& parGoalPos, const int2& parEntityDimensions)
	: startPos_(parStartPos),
	goalPos_(parGoalPos),
	entityDimensions_(parEntityDimensions),
	hasPath_(false)
{
	assert(parStartPos != parGoalPos); // FIXME: Warning with floating comparison!
	ComputeShortestPath_();
}

// ============================================================================

Path::~Path()
{
}

// ============================================================================

// Breadth first search algorithm
void Path::ComputeShortestPath_()
{
	const WalkTile startWalkTile(startPos_);
	const WalkTile goalWalkTile(goalPos_);

	std::list<WalkTile> unvisitedWalkTiles;
	std::map<WalkTile, WalkTile> parentTile;
	unvisitedWalkTiles.push_back(startWalkTile);
	parentTile[startWalkTile] = startWalkTile;
	while (!unvisitedWalkTiles.empty())
	{
		WalkTile curWalkTile = unvisitedWalkTiles.front();
		unvisitedWalkTiles.pop_front();

		if (curWalkTile == goalWalkTile)
		{
			hasPath_ = true;
			break; // Shortest path found
		}

		for (int dir = 0; dir < MAX_DIRS; ++dir)
		{
			// Hack: Ignore diagonal move (WARNING: EDir order must not change)
			if (dir & 1)
				continue;

			WalkTile curDir(int(dirs[dir].x), int(dirs[dir].y)); // FIXME: Shouldn't need to cast
			WalkTile nextWalkTile(curWalkTile + curDir);

			if (nextWalkTile.IsValid() &&
				 parentTile.count(nextWalkTile) == 0 &&
				 World::Inst()->IsWalkable(nextWalkTile, entityDimensions_))
			{
				parentTile[nextWalkTile] = curWalkTile;
				unvisitedWalkTiles.push_back(nextWalkTile);
			}
		}
	}

	RetrieveWalkTilePathFromParents_(parentTile, startWalkTile, goalWalkTile);
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
// ----------------------------------------------------------------------------
// ============================================================================