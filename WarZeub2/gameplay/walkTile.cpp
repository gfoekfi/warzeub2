#include "walkTile.h"
#include "buildTile.h"
#include "world.h"
#include "unitDesc.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void WalkTile::Align(float2& parPos)
{
	parPos = WalkTile(parPos).ToWorldPos();
}

// ============================================================================

bool WalkTile::IsValid() const
{
	assert(BUILD_TILE_SIZE == (WALK_TILE_SIZE * 4));

	int worldWidth = World::Inst()->Width() * (BUILD_TILE_SIZE / WALK_TILE_SIZE);
	int worldHeight = World::Inst()->Height() * (BUILD_TILE_SIZE / WALK_TILE_SIZE);

	return 
		data_.x >= 0 && data_.x < worldWidth &&
		data_.y >= 0 && data_.y < worldHeight;
}

// ============================================================================

float2 WalkTile::ToWorldPos() const
{
	float2 result;

	result.x = (0.5f + float(data_.x)) * float(WALK_TILE_SIZE);
	result.y = (0.5f + float(data_.y)) * float(WALK_TILE_SIZE);

	return result;
}

// ============================================================================

bool WalkTile::operator==(const WalkTile& parRhs) const
{
	return data_.operator==(parRhs.data_);
}

// ============================================================================

bool WalkTile::operator!=(const WalkTile& parRhs) const
{
	return data_.operator!=(parRhs.data_);
}

// ============================================================================

bool WalkTile::operator<(const WalkTile& parRhs) const
{
	return data_.operator<(parRhs.data_);
}

// ============================================================================

WalkTile WalkTile::operator+(const WalkTile& parRhs) const
{
	return WalkTile(data_.x + parRhs.data_.x, data_.y + parRhs.data_.y);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================