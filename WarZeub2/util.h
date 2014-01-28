#ifndef UTIL_H_
#define UTIL_H_


#include <SDL.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

struct Vec2
{
	Vec2(int parX, int parY) :
		x(parX),
		y(parY)
	{
	}

	Vec2(const Vec2& parRhs) :
		x(parRhs.x),
		y(parRhs.y)
	{
	}

	Vec2() :
		x(0),
		y(0)
	{
	}

	Vec2& operator=(const Vec2& parRhs)
	{
		x = parRhs.x;
		y = parRhs.y;

		return *this;
	}

	bool operator==(const Vec2& parRhs) const
	{
		return (x == parRhs.x && y == parRhs.y);
	}

	int x;
	int y;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

enum EDir
{
	DIR_N = 0,
	DIR_NE,
	DIR_E,
	DIR_SE,
	DIR_S,
	DIR_SW,
	DIR_W,
	DIR_NW,
	MAX_DIRS
};

// ============================================================================

extern Vec2 dirs[MAX_DIRS];

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

template <typename T>
T Clamp(T parValue, T parMin, T parMax)
{
	if (parValue < parMin)
		return parMin;
	if (parValue > parMax)
		return parMax;
	return parValue;
}

// ============================================================================

bool DoesBBoxesCollide(const SDL_Rect* parBoxA, const SDL_Rect* parBoxB);
EDir DirectionToTarget(const Vec2& parSrc, const Vec2& parDst);
void TransformToScreenCoordinate(SDL_Rect& parRect);
void TransformToScreenCoordinate(Vec2& parPos);
void TransformToWorldCoordinate(SDL_Rect& parRect);
void TransformToWorldCoordinate(Vec2& parRect);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif