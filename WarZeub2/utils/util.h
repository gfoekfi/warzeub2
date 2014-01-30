#ifndef UTIL_H_
#define UTIL_H_


#include "vecTypes.h"
#include <assert.h>
#include <SDL.h>


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

extern int2 dirs[MAX_DIRS];

// ============================================================================

EDir DirectionToTarget(const int2& parSrc, const int2& parDst);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

template <typename T>
T Clamp(T parValue, T parMin, T parMax)
{
	assert(parMin <= parMax);

	if (parValue < parMin)
		return parMin;
	if (parValue > parMax)
		return parMax;
	return parValue;
}

// ============================================================================

bool DoesBBoxesCollide(const SDL_Rect* parBoxA, const SDL_Rect* parBoxB);

void TransformToScreenCoordinate(SDL_Rect& parRect, const int2& parCameraPos);
void TransformToScreenCoordinate(int2& parPos, const int2& parCameraPos);
void TransformToWorldCoordinate(SDL_Rect& parRect, const int2& parCameraPos);
void TransformToWorldCoordinate(int2& parRect, const int2& parCameraPos);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif