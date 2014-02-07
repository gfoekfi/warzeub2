#ifndef WALK_TILE_H_
#define WALK_TILE_H_


#include "../utils/vecTypes.h"
#include "unit.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

const size_t WALK_TILE_SIZE = 8; // in pixel (basically BUILD_TILE_SIZE / 4)

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

// This is basically an int2 wrapper. We can't inherit from int2 because of C++
// restriction. This type exist to help maintaining consistency between different
// kind of coordinates.
class WalkTile
{
public:
	WalkTile() : data_() { }
	WalkTile(int parX, int parY) : data_(parX, parY) { } // should be WalkTile aligned
	WalkTile(const WalkTile& parRhs) : data_(parRhs.data_) { }
	WalkTile(const float2& parPos)
		: data_(int(parPos.x) / WALK_TILE_SIZE, int(parPos.y) / WALK_TILE_SIZE)
	{
	}

public:
	static void Align(float2& parPos); // to  the center of walk tile

public:
	bool operator==(const WalkTile& parRhs) const;
	bool operator!=(const WalkTile& parRhs) const;
	WalkTile operator+(const WalkTile& parRhs) const;
	bool operator<(const WalkTile& parRhs) const;

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