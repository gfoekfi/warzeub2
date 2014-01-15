#include "AnimDesc.h"
#include <SDL_Image.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

std::map<EUnitType, std::map<EUnitState, AnimDesc> > unitTypeStateToAnimation;
std::map<EUnitType, SDL_Surface*> unitTypeToImage;

// ============================================================================

const AnimDesc gruntIdleAnim(0, 0, 75, 54, 1);
const AnimDesc gruntMoveAnim(0, 0, 76, 54, 5);
const AnimDesc gruntAttackAnim(0, 5*54, 76, 56, 4);
const AnimDesc gruntDeadAnim(0, 11*51, 76, 54, 1);

const AnimDesc peonIdleAnim(0, 0, 51, 40, 1);
const AnimDesc peonMoveAnim(0, 0, 51, 40, 5);
const AnimDesc peonHarvestAnim(0, 40*5, 51, 50, 5);
const AnimDesc peonGatherGold(26, 40*5 + 50*7 - 20, 38, 43, 5);
const AnimDesc peonGatherWood(24, 40*5 + 50*7 + 5*43 - 20, 42, 46, 5);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitAnimDesc()
{
	unitTypeToImage[EUT_GRUNT] = IMG_Load("../Data/grunt.png");
	unitTypeToImage[EUT_PEON] = IMG_Load("../Data/peon.png");

	unitTypeStateToAnimation[EUT_GRUNT][EUS_IDLE] = gruntIdleAnim;
	unitTypeStateToAnimation[EUT_GRUNT][EUS_MOVE] = gruntMoveAnim;
	unitTypeStateToAnimation[EUT_GRUNT][EUS_ATTACK] = gruntAttackAnim;
	unitTypeStateToAnimation[EUT_GRUNT][EUS_DEAD] = gruntDeadAnim;

	unitTypeStateToAnimation[EUT_PEON][EUS_IDLE] = peonIdleAnim;
	unitTypeStateToAnimation[EUT_PEON][EUS_MOVE] = peonMoveAnim;
	unitTypeStateToAnimation[EUT_PEON][EUS_HARVEST] = peonHarvestAnim;
	unitTypeStateToAnimation[EUT_PEON][EUS_GATHER_GOLD] = peonGatherGold;
	unitTypeStateToAnimation[EUT_PEON][EUS_GATHER_WOOD] = peonGatherWood;
}

// ============================================================================

void ReleaseAnimDesc()
{
	SDL_FreeSurface(unitTypeToImage[EUT_GRUNT]);
	SDL_FreeSurface(unitTypeToImage[EUT_PEON]);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================