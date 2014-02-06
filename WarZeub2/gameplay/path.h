#ifndef PATH_H_
#define PATH_H_


#include "buildTile.h"
#include <vector>
#include <map>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class Path
{
public:
	Path(const float2& parStartPos, const float2& parGoalPos, const int2& parEntityDimensions = int2(0,0));
	~Path();

public:
	const BuildTile& BuildTileFromWaypoint(size_t parWaypoint) const;
	size_t PathSize() const { return buildTilePath_.size(); }
	bool HasPath() const { return hasPath_; }

private:
	void ComputeShortestPath_();
	void DumpPath_();
	void RetrieveBuildTilePathFromParents_(std::map<BuildTile, BuildTile>& parParentOf, // TODO: should be const
														const BuildTile& parStartBuildTile,
														const BuildTile& parGoalBuildTile);

private:
	float2 startPos_;
	float2 goalPos_;
	int2 entityDimensions_;
	std::vector<BuildTile> buildTilePath_;
	bool hasPath_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif