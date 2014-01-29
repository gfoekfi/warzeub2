#include "util.h"
#include "graphic/renderer.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Vec2 dirs[MAX_DIRS] =
{
	Vec2(0, -1), // N
	Vec2(1, -1),
	Vec2(1, 0),	// E
	Vec2(1, 1),
	Vec2(0, 1),	// S
	Vec2(-1, 1),
	Vec2(-1, 0),	// W
	Vec2(-1, -1)
};

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

EDir DirectionToTarget(const Vec2& parSrc, const Vec2& parDst)
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

void TransformToScreenCoordinate(SDL_Rect& parRect, const Vec2& parCameraPos)
{
	parRect.x += viewport.x - parCameraPos.x;
	parRect.y += viewport.y - parCameraPos.y;
}

// ============================================================================

void TransformToScreenCoordinate(Vec2& parPos, const Vec2& parCameraPos)
{
	parPos.x += viewport.x - parCameraPos.x;
	parPos.y += viewport.y - parCameraPos.y;
}

// ============================================================================

void TransformToWorldCoordinate(SDL_Rect& parRect, const Vec2& parCameraPos)
{
	parRect.x -= viewport.x - parCameraPos.x;
	parRect.y -= viewport.y - parCameraPos.y;
}

// ============================================================================

void TransformToWorldCoordinate(Vec2& parPos, const Vec2& parCameraPos)
{
	parPos.x -= viewport.x - parCameraPos.x;
	parPos.y -= viewport.y - parCameraPos.y;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================