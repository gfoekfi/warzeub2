#include "spriteDesc.h"
#include <SDL_Image.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

std::map<EUnitType, std::map<EUnitState, SpriteDesc> > unitTypeStateToSpriteDesc;
std::map<EUnitType, SDL_Surface*> unitTypeToImage;
std::map<EUnitType, SpriteDesc> unitTypeToIconSpriteDesc;
std::map<EOrder, SpriteDesc> orderToIconSpriteDesc;

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

const SpriteDesc mineIdleAnim(10, 5, 100, 100, 1);
const SpriteDesc townHallIdleAnim(140, 540, 130, 130, 1);

// ============================================================================


#define SPRITE_DESC_PARAM_FROM_ICON(col, line)\
	6 + col*(46 + 3), 5 + line*(38 + 3), 46, 38, 1

const SpriteDesc peonIcon		(SPRITE_DESC_PARAM_FROM_ICON(1, 0));
const SpriteDesc gruntIcon		(SPRITE_DESC_PARAM_FROM_ICON(3, 0));
const SpriteDesc mineIcon		(SPRITE_DESC_PARAM_FROM_ICON(4, 7));
const SpriteDesc townHallIcon	(SPRITE_DESC_PARAM_FROM_ICON(1, 4));
const SpriteDesc moveIcon		(SPRITE_DESC_PARAM_FROM_ICON(4, 8));
const SpriteDesc stopIcon		(SPRITE_DESC_PARAM_FROM_ICON(7, 16));
const SpriteDesc cancelIcon	(SPRITE_DESC_PARAM_FROM_ICON(1, 9));
const SpriteDesc buildIcon		(SPRITE_DESC_PARAM_FROM_ICON(7, 8));

#undef SPRITE_DESC_PARAM_FROM_ICON

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitSpriteDesc()
{
	unitTypeToImage[EUT_GRUNT] = IMG_Load("../Data/grunt.png");
	unitTypeToImage[EUT_PEON] = IMG_Load("../Data/peon.png");
	unitTypeToImage[EUT_MINE] = IMG_Load("../Data/misc.png");
	unitTypeToImage[EUT_TOWN_HALL] = IMG_Load("../Data/orc_building.png");

	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_IDLE] = gruntIdleAnim;
	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_MOVE] = gruntMoveAnim;
	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_ATTACK] = gruntAttackAnim;
	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_DEAD] = gruntDeadAnim;

	unitTypeStateToSpriteDesc[EUT_PEON][EUS_IDLE] = peonIdleAnim;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_MOVE] = peonMoveAnim;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_HARVEST] = peonHarvestAnim;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_GATHER_GOLD] = peonGatherGold;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_GATHER_WOOD] = peonGatherWood;

	unitTypeStateToSpriteDesc[EUT_MINE][EUS_IDLE] = mineIdleAnim;
	unitTypeStateToSpriteDesc[EUT_TOWN_HALL][EUS_IDLE] = townHallIdleAnim;

	// ---

	unitTypeToIconSpriteDesc[EUT_PEON] = peonIcon;
	unitTypeToIconSpriteDesc[EUT_GRUNT] = gruntIcon;
	unitTypeToIconSpriteDesc[EUT_MINE] = mineIcon;
	unitTypeToIconSpriteDesc[EUT_TOWN_HALL] = townHallIcon;
	
	// ---

	orderToIconSpriteDesc[EO_MOVE] = moveIcon;
	orderToIconSpriteDesc[EO_STOP] = stopIcon;
	orderToIconSpriteDesc[EO_CANCEL] = cancelIcon;
	orderToIconSpriteDesc[EO_TRAIN_PEON] = peonIcon;
	orderToIconSpriteDesc[EO_BUILD] = buildIcon;
	orderToIconSpriteDesc[EO_BUILD_TOWN_HALL] = townHallIcon;
}

// ============================================================================

void ReleaseSpriteDesc()
{
	SDL_FreeSurface(unitTypeToImage[EUT_GRUNT]);
	SDL_FreeSurface(unitTypeToImage[EUT_PEON]);
	SDL_FreeSurface(unitTypeToImage[EUT_MINE]);
	SDL_FreeSurface(unitTypeToImage[EUT_TOWN_HALL]);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================