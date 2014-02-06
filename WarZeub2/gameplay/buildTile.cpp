#include "buildTile.h"
#include "world.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

bool BuildTile::IsValid() const
{
	return 
		data_.x >= 0 && data_.x < int(World::Inst()->Width()) &&
		data_.y >= 0 && data_.y < int(World::Inst()->Height());
}

// ============================================================================

float2 BuildTile::ToWorldPos() const
{
	float2 result;

	result.x = (0.5f + float(data_.x)) * float(MAP_BUILD_TILE_SIZE);
	result.y = (0.5f + float(data_.y)) * float(MAP_BUILD_TILE_SIZE);

	return result;
}

// ============================================================================

bool BuildTile::operator==(const BuildTile& parRhs) const
{
	return data_.operator==(parRhs.data_);
}

// ============================================================================

bool BuildTile::operator!=(const BuildTile& parRhs) const
{
	return data_.operator!=(parRhs.data_);
}

// ============================================================================

bool BuildTile::operator<(const BuildTile& parRhs) const
{
	return data_.operator<(parRhs.data_);
}

// ============================================================================

BuildTile BuildTile::operator+(const BuildTile& parRhs) const
{
	return BuildTile(data_.x + parRhs.data_.x, data_.y + parRhs.data_.y);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================