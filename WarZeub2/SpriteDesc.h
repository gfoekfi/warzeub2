#ifndef ANIM_DESC_
#define ANIM_DESC_


#include "Unit.h"
#include "UnitDesc.h"
#include <SDL.h>
#include <map>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

struct SpriteDesc
{
	SpriteDesc(int parOffsetX, int parOffsetY, int parWidth, int parHeight, int parMaxStep)
		: offsetX(parOffsetX),
		offsetY(parOffsetY),
		width(parWidth),
		height(parHeight),
		maxStep(parMaxStep)
	{
	}

	SpriteDesc()
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

void InitSpriteDesc();
void ReleaseSpriteDesc();

// ============================================================================

extern std::map<EUnitType, std::map<EUnitState, SpriteDesc> > unitTypeStateToSpriteDesc;
extern std::map<EUnitType, SDL_Surface*> unitTypeToImage;
extern std::map<EUnitType, SpriteDesc> unitTypeToIconSpriteDesc;
extern std::map<EOrder, SpriteDesc> orderToIconSpriteDesc;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

#endif