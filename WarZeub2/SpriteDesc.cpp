#include "SpriteDesc.h"
#include <SDL_Image.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

std::map<EUnitType, std::map<EUnitState, SpriteDesc> > unitTypeStateToSpriteDesc;
std::map<EUnitType, SDL_Surface*> unitTypeToImage;

// ============================================================================

const SpriteDesc gruntIdleAnim(0, 0, 75, 54, 1);
const SpriteDesc gruntMoveAnim(0, 0, 76, 54, 5);
const SpriteDesc gruntAttackAnim(0, 5*54, 76, 56, 4);
const SpriteDesc gruntDeadAnim(0, 11*51, 76, 54, 1);

const SpriteDesc peonIdleAnim(0, 0, 51, 40, 1);
const SpriteDesc peonMoveAnim(0, 0, 51, 40, 5);
const SpriteDesc peonHarvestAnim(0, 40*5, 51, 50, 5);
const SpriteDesc peonGatherGold(26, 40*5 + 50*7 - 20, 38, 43, 5);
const SpriteDesc peonGatherWood(24, 40*5 + 50*7 + 5*43 - 20, 42, 46, 5);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitSpriteDesc()
{
	unitTypeToImage[EUT_GRUNT] = IMG_Load("../Data/grunt.png");
	unitTypeToImage[EUT_PEON] = IMG_Load("../Data/peon.png");

	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_IDLE] = gruntIdleAnim;
	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_MOVE] = gruntMoveAnim;
	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_ATTACK] = gruntAttackAnim;
	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_DEAD] = gruntDeadAnim;

	unitTypeStateToSpriteDesc[EUT_PEON][EUS_IDLE] = peonIdleAnim;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_MOVE] = peonMoveAnim;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_HARVEST] = peonHarvestAnim;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_GATHER_GOLD] = peonGatherGold;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_GATHER_WOOD] = peonGatherWood;
}

// ============================================================================

void ReleaseSpriteDesc()
{
	SDL_FreeSurface(unitTypeToImage[EUT_GRUNT]);
	SDL_FreeSurface(unitTypeToImage[EUT_PEON]);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================