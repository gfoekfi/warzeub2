#include "util.h"
#include "../graphic/renderer.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

float2 dirs[MAX_DIRS] =
{
	float2(0.f, -1.f), // N
	float2(1.f, -1.f),
	float2(1.f, 0.f),	// E
	float2(1.f, 1.f),
	float2(0.f, 1.f),	// S
	float2(-1.f, 1.f),
	float2(-1.f, 0.f),	// W
	float2(-1.f, -1.f)
};

// ============================================================================

EDir DirectionToTarget(const float2& parSrc, const float2& parDst)
{
	float2 delta(parDst.x - parSrc.x, parDst.y - parSrc.y);
	const float MOVE_STEP_TRESHOLD = 1.f;

	if (delta.y >= MOVE_STEP_TRESHOLD)
	{
		if (delta.x <= -MOVE_STEP_TRESHOLD)
			return DIR_SW;
		else if (delta.x >= MOVE_STEP_TRESHOLD)
			return DIR_SE;
		else
			return DIR_S;
	}
	else if (delta.y <= -MOVE_STEP_TRESHOLD)
	{
		if (delta.x <= -MOVE_STEP_TRESHOLD)
			return DIR_NW;
		else if (delta.x >= MOVE_STEP_TRESHOLD)
			return DIR_NE;
		else
			return DIR_N;
	}
	else if (delta.x <= -MOVE_STEP_TRESHOLD)
		return DIR_W;
	else if (delta.x >= MOVE_STEP_TRESHOLD)
		return DIR_E;

	return DIR_N;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

template <typename T>
inline bool IsPointInSegment(T parValue, T parSrcValue, T parDstValue)
{
	return (parValue >= parSrcValue) && (parValue <= parDstValue);
}

// ============================================================================

bool DoesBBoxesCollide(const SDL_Rect* parBoxA, const SDL_Rect* parBoxB)
{
	assert(parBoxA->w >= 1 && parBoxA->h >= 1);
	assert(parBoxB->w >= 1 && parBoxB->h >= 1);

	const SDL_Rect* left = (parBoxA->x <= parBoxB->x) ? parBoxA : parBoxB;
	const SDL_Rect* right = (left == parBoxA) ? parBoxB : parBoxA;
	const SDL_Rect* top = (parBoxA->y <= parBoxB->y) ? parBoxA : parBoxB;
	const SDL_Rect* bottom = (top == parBoxA) ? parBoxB : parBoxA;

	return
		(IsPointInSegment<Sint16>(left->x, right->x, right->x + right->w) ||
		((left->x + left->w) >= right->x)) &&
		(IsPointInSegment<Sint16>(top->y, bottom->y, bottom->y + bottom->h) ||
		((top->y + top->h) >= bottom->y));
}

// ============================================================================

void TransformToScreenCoordinate(SDL_Rect& parRect, const float2& parCameraPos)
{
	parRect.x += viewport.x - Sint16(parCameraPos.x);
	parRect.y += viewport.y - Sint16(parCameraPos.y);
}

// ============================================================================

void TransformToScreenCoordinate(float2& parPos, const float2& parCameraPos)
{
	parPos.x += viewport.x - parCameraPos.x;
	parPos.y += viewport.y - parCameraPos.y;
}

// ============================================================================

void TransformToWorldCoordinate(SDL_Rect& parRect, const float2& parCameraPos)
{
	parRect.x -= viewport.x - Sint16(parCameraPos.x);
	parRect.y -= viewport.y - Sint16(parCameraPos.y);
}

// ============================================================================

void TransformToWorldCoordinate(float2& parPos, const float2& parCameraPos)
{
	parPos.x -= viewport.x - parCameraPos.x;
	parPos.y -= viewport.y - parCameraPos.y;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================