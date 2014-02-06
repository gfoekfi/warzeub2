#ifndef BUILD_TILE_H_
#define BUILD_TILE_H_


#include "../utils/vecTypes.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

const size_t MAP_BUILD_TILE_SIZE = 32; // in pixel

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

// This is basically an int2 wrapper. We can't inherit from int2 because of C++
// restriction. This type exist to help maintaining consistency between different
// kind of coordinates.
class BuildTile
{
public:
	BuildTile() : data_() { }
	BuildTile(int parX, int parY) : data_(parX, parY) { } // should be BuildTile aligned
	BuildTile(const BuildTile& parRhs) : data_(parRhs.data_) { }
	BuildTile(const float2& parPos)
		: data_(int(parPos.x) / MAP_BUILD_TILE_SIZE, int(parPos.y) / MAP_BUILD_TILE_SIZE)
	{
	}

public:
	bool operator==(const BuildTile& parRhs) const;
	bool operator!=(const BuildTile& parRhs) const;
	BuildTile operator+(const BuildTile& parRhs) const;
	bool operator<(const BuildTile& parRhs) const;

public:
	bool IsValid() const;
	float2 ToWorldPos() const;

public:
	int x() const { return data_.x; }
	int y() const { return data_.y; }
	int w() const { return data_.w; }
	int h() const { return data_.h; }

private:
	int2 data_; // Note: C++ can't inherit from union
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif