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

bool DoesBBoxesCollide(const SDL_Rect& parBoxA, const SDL_Rect& parBoxB)
{
	return (((parBoxA.x >= parBoxB.x) && (parBoxA.x <= (parBoxB.x + parBoxB.w)) ||
		((parBoxA.x + parBoxA.w) >= parBoxB.x) && ((parBoxA.x + parBoxA.w) <= (parBoxB.x + parBoxB.w))) /*&&
		((parBoxA.y >= parBoxB.y) && (parBoxA.y <= (parBoxB.y + parBoxB.h)) ||
		((parBoxA.y + parBoxA.h) >= parBoxB.y) && ((parBoxA.y + parBoxA.h) <= (parBoxB.y + parBoxB.h)))*/);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================