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
	const int2 startBuildTile(int(startPos_.x / MAP_BUILD_TILE_SIZE), int(startPos_.y / MAP_BUILD_TILE_SIZE));
	const int2 goalBuildTile(int(goalPos_.x / MAP_BUILD_TILE_SIZE), int(goalPos_.y / MAP_BUILD_TILE_SIZE));

	std::list<int2> unvisitedBuildTiles;
	std::map<int2, int2> parentTile;
	unvisitedBuildTiles.push_back(startBuildTile);
	parentTile[startBuildTile] = startBuildTile;
	while (!unvisitedBuildTiles.empty())
	{
		int2 curBuildTile = unvisitedBuildTiles.front();
		unvisitedBuildTiles.pop_front();

		if (curBuildTile == goalBuildTile)
		{
			hasPath_ = true;
			break; // Shortest path found
		}

		for (int dir = 0; dir < MAX_DIRS; ++dir)
		{
			// Hack: Ignore diagonal move (WARNING: EDir order must not change)
			if (dir & 1)
				continue;

			int2 curDir(int(dirs[dir].x), int(dirs[dir].y)); // FIXME: Shouldn't need to cast
			int2 nextBuildTile = curBuildTile + curDir;

			if (nextBuildTile.x >= 0 && nextBuildTile.x < int(World::Inst()->Width()) &&
				nextBuildTile.y >= 0 && nextBuildTile.y < int(World::Inst()->Height()) &&
				World::Inst()->IsBuildTileAccessible(nextBuildTile, entityDimensions_) &&
				(parentTile.count(nextBuildTile) == 0))
			{
				parentTile[nextBuildTile] = curBuildTile;
				unvisitedBuildTiles.push_back(nextBuildTile);
			}
		}
	}

	RetrieveBuildTilePathFromParents_(parentTile, startBuildTile, goalBuildTile);
}

// =======================================================================

void Path::RetrieveBuildTilePathFromParents_(std::map<int2, int2>& parParentOf,
															const int2& parStartBuildTile,
															const int2& parGoalBuildTile)
{
	if (parParentOf.count(parGoalBuildTile) == 0)
	{
		assert(!hasPath_);
		fprintf(stdout, "[PATH] No valid path as been found between (%d, %d) and (%d, %d)\n",
			parStartBuildTile.x, parStartBuildTile.y, parGoalBuildTile.x, parGoalBuildTile.y);
		return;
	}

	int2 curBuildTile = parGoalBuildTile;
	std::vector<int2> reversePath;
	while (curBuildTile != parStartBuildTile)
	{
		reversePath.push_back(curBuildTile);
		assert(parParentOf.count(curBuildTile) > 0);
		curBuildTile = parParentOf[curBuildTile];
	}

	for (std::vector<int2>::reverse_iterator tile = reversePath.rbegin();
		tile != reversePath.rend(); ++tile)
	{
		tilePath_.push_back(*tile);
	}

	fprintf(stdout, "[PATH] Shortest path between (%d, %d) and (%d, %d):\n",
		parStartBuildTile.x, parStartBuildTile.y, parGoalBuildTile.x, parGoalBuildTile.y);
	DumpPath_();
}

// ============================================================================

void Path::DumpPath_()
{
	assert(hasPath_);
	assert(tilePath_.size() >= 1);

	for (size_t y = 0; y < World::Inst()->Height(); ++y)
	{
		for (size_t x = 0; x < World::Inst()->Width(); ++x)
		{
			char c = '.';
			int2 tile(x, y);

			if (tile == int2(int(startPos_.x / MAP_BUILD_TILE_SIZE), int(startPos_.y / MAP_BUILD_TILE_SIZE)))
				c = 'S';
			else if (tile == int2(int(goalPos_.x / MAP_BUILD_TILE_SIZE), int(goalPos_.y / MAP_BUILD_TILE_SIZE)))
				c = 'E';
			else if (!World::Inst()->IsBuildTileAccessible(tile))
				c = '#';
			else
			{
				for (size_t waypoint = 0; waypoint < tilePath_.size(); ++waypoint)
					if (tilePath_[waypoint] == int2(x, y))
					{
						c = '*';
						break;
					}
			}

			fprintf(stdout, "%c", c);
		}
		fprintf(stdout, "\n");
	}
}

// ============================================================================

const int2& Path::TileFromWaypoint(size_t parWaypoint) const
{
	assert(hasPath_);
	assert(parWaypoint < tilePath_.size());

	return tilePath_[parWaypoint];
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================