#ifndef ANIM_DESC_
#define ANIM_DESC_


#include "Unit.h"
#include <SDL.h>
#include <map>

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

struct AnimDesc
{
	AnimDesc(int parOffsetX, int parOffsetY, int parWidth, int parHeight, int parMaxStep)
		: offsetX(parOffsetX),
		offsetY(parOffsetY),
		width(parWidth),
		height(parHeight),
		maxStep(parMaxStep)
	{
	}

	AnimDesc()
		: offsetX(0),
		offsetY(0),
		width(0),
		height(0),
		maxStep(0)
	{
	}

	int offsetX;
	int offsetY;
	int width;
	int height;
	int maxStep;
};

// ============================================================================

void InitAnimDesc();
void ReleaseAnimDesc();

// ============================================================================

extern std::map<EUnitType, std::map<EUnitState, AnimDesc> > unitTypeStateToAnimation;
extern std::map<EUnitType, SDL_Surface*> unitTypeToImage;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

#endif