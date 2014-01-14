#include "AnimDesc.h"
#include <SDL_Image.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

std::map<EUnitType, std::map<EUnitState, AnimDesc> > unitTypeStateToAnimation;
std::map<EUnitType, SDL_Surface*> unitTypeToImage;

// ============================================================================

const AnimDesc gruntIdleAnim(0, 0, 76, 54, 1);
const AnimDesc gruntMoveAnim(0, 0, 76, 54, 5);
const AnimDesc gruntAttackAnim(0, 5*54, 76, 56, 4);
const AnimDesc gruntDeadAnim(0, 11*51, 76, 54, 1);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitAnimDesc()
{
	unitTypeToImage[EUT_GRUNT] = IMG_Load("../Data/grunt.png");

	unitTypeStateToAnimation[EUT_GRUNT][EUS_IDLE] = gruntIdleAnim;
	unitTypeStateToAnimation[EUT_GRUNT][EUS_MOVE] = gruntMoveAnim;
	unitTypeStateToAnimation[EUT_GRUNT][EUS_ATTACK] = gruntAttackAnim;
	unitTypeStateToAnimation[EUT_GRUNT][EUS_DEAD] = gruntDeadAnim;
}

// ============================================================================

void ReleaseAnimDesc()
{
	SDL_FreeSurface(unitTypeToImage[EUT_GRUNT]);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================