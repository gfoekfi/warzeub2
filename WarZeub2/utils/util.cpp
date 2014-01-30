#include "util.h"
#include "../graphic/renderer.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

int2 dirs[MAX_DIRS] =
{
	int2(0, -1), // N
	int2(1, -1),
	int2(1, 0),	// E
	int2(1, 1),
	int2(0, 1),	// S
	int2(-1, 1),
	int2(-1, 0),	// W
	int2(-1, -1)
};

// ============================================================================

EDir DirectionToTarget(const int2& parSrc, const int2& parDst)
{
	int deltaPosX = parDst.x - parSrc.x;
	int deltaPosY = parDst.y - parSrc.y;
	const int MOVE_STEP_TRESHOLD = 1;

	if (deltaPosY >= MOVE_STEP_TRESHOLD)
	{
		if (deltaPosX <= -MOVE_STEP_TRESHOLD)
			return DIR_SW;
		else if (deltaPosX >= MOVE_STEP_TRESHOLD)
			return DIR_SE;
		else
			return DIR_S;
	}
	else if (deltaPosY <= -MOVE_STEP_TRESHOLD)
	{
		if (deltaPosX <= -MOVE_STEP_TRESHOLD)
			return DIR_NW;
		else if (deltaPosX >= MOVE_STEP_TRESHOLD)
			return DIR_NE;
		else
			return DIR_N;
	}
	else if (deltaPosX <= -MOVE_STEP_TRESHOLD)
		return DIR_W;
	else if (deltaPosX >= MOVE_STEP_TRESHOLD)
		return DIR_E;

	return DIR_N;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

inline bool IsPointInSegment(int parValue, int parSrcValue, int parDstValue)
{
	return (parValue >= parSrcValue) && (parValue <= parDstValue);
}

// ============================================================================

bool DoesBBoxesCollide(const SDL_Rect* parBoxA, const SDL_Rect* parBoxB)
{
	const SDL_Rect* left = (parBoxA->x <= parBoxB->x) ? parBoxA : parBoxB;
	const SDL_Rect* right = (left == parBoxA) ? parBoxB : parBoxA;
	const SDL_Rect* top = (parBoxA->y <= parBoxB->y) ? parBoxA : parBoxB;
	const SDL_Rect* bottom = (top == parBoxA) ? parBoxB : parBoxA;

	return
		(IsPointInSegment(left->x, right->x, right->x + right->w) ||
		((left->x + left->w) >= right->x)) &&
		(IsPointInSegment(top->y, bottom->y, bottom->y + bottom->h) ||
		((top->y + top->h) >= bottom->y));
}

// ============================================================================

void TransformToScreenCoordinate(SDL_Rect& parRect, const int2& parCameraPos)
{
	parRect.x += viewport.x - parCameraPos.x;
	parRect.y += viewport.y - parCameraPos.y;
}

// ============================================================================

void TransformToScreenCoordinate(int2& parPos, const int2& parCameraPos)
{
	parPos.x += viewport.x - parCameraPos.x;
	parPos.y += viewport.y - parCameraPos.y;
}

// ============================================================================

void TransformToWorldCoordinate(SDL_Rect& parRect, const int2& parCameraPos)
{
	parRect.x -= viewport.x - parCameraPos.x;
	parRect.y -= viewport.y - parCameraPos.y;
}

// ============================================================================

void TransformToWorldCoordinate(int2& parPos, const int2& parCameraPos)
{
	parPos.x -= viewport.x - parCameraPos.x;
	parPos.y -= viewport.y - parCameraPos.y;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================