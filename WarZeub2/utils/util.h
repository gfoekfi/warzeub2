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

extern float2 dirs[MAX_DIRS];

// ============================================================================

EDir DirectionToTarget(const float2& parSrc, const float2& parDst);

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

bool DoesBBoxesCollide(const SDL_Rect* parBoxA, const SDL_Rect* parBoxB); // FIXME: rename it

void TransformToScreenCoordinate(SDL_Rect& parRect, const float2& parCameraPos);
void TransformToScreenCoordinate(float2& parPos, const float2& parCameraPos);
void TransformToWorldCoordinate(SDL_Rect& parRect, const float2& parCameraPos);
void TransformToWorldCoordinate(float2& parRect, const float2& parCameraPos);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif