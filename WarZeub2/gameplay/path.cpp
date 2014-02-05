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
	const int2 startTile(int(startPos_.x / MAP_TILE_SIZE), int(startPos_.y / MAP_TILE_SIZE));
	const int2 goalTile(int(goalPos_.x / MAP_TILE_SIZE), int(goalPos_.y / MAP_TILE_SIZE));

	std::list<int2> unvisitedTiles;
	std::map<int2, int2> parentTile;
	unvisitedTiles.push_back(startTile);
	parentTile[startTile] = startTile;
	while (!unvisitedTiles.empty())
	{
		int2 curTile = unvisitedTiles.front();
		unvisitedTiles.pop_front();

		if (curTile == goalTile)
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
			int2 nextTile = curTile + curDir;

			if (nextTile.x >= 0 && nextTile.x < int(World::Inst()->Width()) &&
				nextTile.y >= 0 && nextTile.y < int(World::Inst()->Height()) &&
				World::Inst()->IsTileAccessible(nextTile, entityDimensions_) &&
				(parentTile.count(nextTile) == 0))
			{
				parentTile[nextTile] = curTile;
				unvisitedTiles.push_back(nextTile);
			}
		}
	}

	RetrieveTilePathFromParents_(parentTile, startTile, goalTile);
}

// =======================================================================

void Path::RetrieveTilePathFromParents_(std::map<int2, int2>& parParentOf,
													 const int2& parStartTile,
													 const int2& parGoalTile)
{
	if (parParentOf.count(parGoalTile) == 0)
	{
		assert(!hasPath_);
		fprintf(stdout, "[PATH] No valid path as been found between (%d, %d) and (%d, %d)\n",
			parStartTile.x, parStartTile.y, parGoalTile.x, parGoalTile.y);
		return;
	}

	int2 curTile = parGoalTile;
	std::vector<int2> reversePath;
	while (curTile != parStartTile)
	{
		reversePath.push_back(curTile);
		assert(parParentOf.count(curTile) > 0);
		curTile = parParentOf[curTile];
	}

	for (std::vector<int2>::reverse_iterator tile = reversePath.rbegin();
		tile != reversePath.rend(); ++tile)
	{
		tilePath_.push_back(*tile);
	}

	fprintf(stdout, "[PATH] Shortest path between (%d, %d) and (%d, %d):\n",
		parStartTile.x, parStartTile.y, parGoalTile.x, parGoalTile.y);
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

			if (tile == int2(int(startPos_.x / MAP_TILE_SIZE), int(startPos_.y / MAP_TILE_SIZE)))
				c = 'S';
			else if (tile == int2(int(goalPos_.x / MAP_TILE_SIZE), int(goalPos_.y / MAP_TILE_SIZE)))
				c = 'E';
			else if (!World::Inst()->IsTileAccessible(tile))
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