#ifndef PATH_H_
#define PATH_H_


#include "buildTile.h"
#include "walkTile.h"
#include "../utils/singleton.h"
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
	static WalkTile NearestWalkableTileOf(const float2& parDstPos,
													  const float2& parSrcPos,
													  const int2& parDimensions);

public:
	const WalkTile& WalkTileFromWaypoint(size_t parWaypoint) const;
	size_t PathSize() const { return walkTilePath_.size(); }
	bool HasPath() const { return hasPath_; }

private:
	void ComputeShortestPath_();
	void DumpPath_(std::map<WalkTile, WalkTile>& parParentOf);
	void RetrieveWalkTilePathFromParents_(std::map<WalkTile, WalkTile>& parParentOf, // TODO: should be const
														const WalkTile& parStartWalkTile,
														const WalkTile& parGoalWalkTile);

private:
	float2 startPos_;
	float2 goalPos_;
	int2 entityDimensions_;
	std::vector<WalkTile> walkTilePath_;
	bool hasPath_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class PathCache : public Singleton<PathCache>
{
public:
	PathCache() {}
	virtual ~PathCache();

public:
	Path* GetOrCreatePath(const WalkTile& parSrc,
								 const WalkTile& parDst,
								 const int2& parDimensions);

private:
	std::map<WalkTile, std::map<WalkTile, std::map<int2, Path*> > > cache_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif