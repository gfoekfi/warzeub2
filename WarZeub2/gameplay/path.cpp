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
	const BuildTile startBuildTile(startPos_);
	const BuildTile goalBuildTile(goalPos_);

	std::list<BuildTile> unvisitedBuildTiles;
	std::map<BuildTile, BuildTile> parentTile;
	unvisitedBuildTiles.push_back(startBuildTile);
	parentTile[startBuildTile] = startBuildTile;
	while (!unvisitedBuildTiles.empty())
	{
		BuildTile curBuildTile = unvisitedBuildTiles.front();
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

			BuildTile curDir(int(dirs[dir].x), int(dirs[dir].y)); // FIXME: Shouldn't need to cast
			BuildTile nextBuildTile(curBuildTile + curDir);

			if (nextBuildTile.IsValid() &&
				 World::Inst()->IsBuildTileAccessible(nextBuildTile, entityDimensions_) &&
				 parentTile.count(nextBuildTile) == 0)
			{
				parentTile[nextBuildTile] = curBuildTile;
				unvisitedBuildTiles.push_back(nextBuildTile);
			}
		}
	}

	RetrieveBuildTilePathFromParents_(parentTile, startBuildTile, goalBuildTile);
}

// =======================================================================

void Path::RetrieveBuildTilePathFromParents_(std::map<BuildTile, BuildTile>& parParentOf,
															const BuildTile& parStartBuildTile,
															const BuildTile& parGoalBuildTile)
{
	if (parParentOf.count(parGoalBuildTile) == 0)
	{
		assert(!hasPath_);
		fprintf(stdout, "[PATH] No valid path as been found between (%d, %d) and (%d, %d)\n",
			parStartBuildTile.x(), parStartBuildTile.y(), parGoalBuildTile.x(), parGoalBuildTile.y());
		return;
	}

	BuildTile curBuildTile(parGoalBuildTile);
	std::vector<BuildTile> reversePath;
	while (curBuildTile != parStartBuildTile)
	{
		reversePath.push_back(curBuildTile);
		assert(parParentOf.count(curBuildTile) > 0);
		curBuildTile = parParentOf[curBuildTile];
	}

	for (std::vector<BuildTile>::reverse_iterator tile = reversePath.rbegin();
		tile != reversePath.rend(); ++tile)
	{
		buildTilePath_.push_back(*tile);
	}

	fprintf(stdout, "[PATH] Shortest path between (%d, %d) and (%d, %d):\n",
		parStartBuildTile.x(), parStartBuildTile.y(), parGoalBuildTile.x(), parGoalBuildTile.y());
	DumpPath_();
}

// ============================================================================

void Path::DumpPath_()
{
	assert(hasPath_);
	assert(buildTilePath_.size() >= 1);

	BuildTile startBuildTile(startPos_);
	BuildTile goalBuildTile(goalPos_);

	for (size_t y = 0; y < World::Inst()->Height(); ++y)
	{
		for (size_t x = 0; x < World::Inst()->Width(); ++x)
		{
			char c = '.';
			BuildTile tile(x, y);

			if (tile == startBuildTile)
				c = 'S';
			else if (tile == goalBuildTile)
				c = 'E';
			else if (!World::Inst()->IsBuildTileAccessible(tile))
				c = '#';
			else
			{
				for (size_t waypoint = 0; waypoint < buildTilePath_.size(); ++waypoint)
					if (buildTilePath_[waypoint] == BuildTile(x, y))
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

const BuildTile& Path::BuildTileFromWaypoint(size_t parWaypoint) const
{
	assert(hasPath_);
	assert(parWaypoint < buildTilePath_.size());

	return buildTilePath_[parWaypoint];
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================