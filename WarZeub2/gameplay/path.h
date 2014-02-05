#ifndef PATH_H_
#define PATH_H_


#include "../utils/vecTypes.h"
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
	const int2& TileFromWaypoint(size_t parWaypoint) const;
	size_t PathSize() const { return tilePath_.size(); }
	bool HasPath() const { return hasPath_; }

private:
	void ComputeShortestPath_();
	void DumpPath_();
	void RetrieveTilePathFromParents_(std::map<int2, int2>& parParentOf, // TODO: should be const
												 const int2& parStartTile,
												 const int2& parGoalTile);

private:
	float2 startPos_;
	float2 goalPos_;
	int2 entityDimensions_;
	std::vector<int2> tilePath_;
	bool hasPath_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif