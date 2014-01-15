#include "util.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

int Clamp(int parValue, int parMin, int parMax)
{
	if (parValue < parMin)
		return parMin;
	if (parValue > parMax)
		return parMax;
	return parValue;
}

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
// ----------------------------------------------------------------------------
// ============================================================================