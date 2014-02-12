#include "spriteDesc.h"
#include <SDL_Image.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

std::map<EUnitType, std::map<EUnitState, SpriteDesc> > unitTypeStateToSpriteDesc;
std::map<EUnitType, SDL_Surface*> unitTypeToImage;
std::map<EUnitType, SpriteDesc> unitTypeToIconSpriteDesc;
std::map<ECommand, SpriteDesc> commandToIconSpriteDesc;

// ============================================================================

const SpriteDesc gruntIdleAnim(0, 0, 75, 54, 1);
const SpriteDesc gruntMoveAnim(0, 0, 76, 54, 5);
const SpriteDesc gruntAttackAnim(0, 5*54, 76, 56, 4);
const SpriteDesc gruntDeadAnim(0, 11*51, 76, 54, 1);

const SpriteDesc peonIdleAnim(0, 0, 51, 40, 1);
const SpriteDesc peonIdleWithGoldAnim(26, 40*5 + 50*7 - 20, 38, 43, 1);
const SpriteDesc peonMovingAnim(0, 0, 51, 40, 5);
const SpriteDesc peonMovingWithGoldAnim(26, 40*5 + 50*7 - 20, 38, 43, 5);

const SpriteDesc peonHarvestAnim(0, 40*5, 51, 50, 5);
const SpriteDesc peonGatherWood(24, 40*5 + 50*7 + 5*43 - 20, 42, 46, 5);

const SpriteDesc mineIdleAnim(10, 5, 100, 100, 1);
const SpriteDesc townHallIdleAnim(140, 540, 130, 130, 1);
const SpriteDesc townHallBuildingStep2Anim(12, 540, 130, 130, 1);

const SpriteDesc farmBuildingStep2Anim(270, 600, 64, 64, 1);
const SpriteDesc farmIdleAnim(337, 600, 64, 64, 1);

const SpriteDesc buildingStep0Anim(585, 145, 60, 45, 1);
const SpriteDesc buildingStep1Anim(585, 200, 65, 60, 1);

// ============================================================================


#define SPRITE_DESC_PARAM_FROM_ICON(col, line)\
	6 + col*(46 + 3), 5 + line*(38 + 3), 46, 38, 1

const SpriteDesc peonIcon		(SPRITE_DESC_PARAM_FROM_ICON(1, 0));
const SpriteDesc gruntIcon		(SPRITE_DESC_PARAM_FROM_ICON(3, 0));
const SpriteDesc mineIcon		(SPRITE_DESC_PARAM_FROM_ICON(4, 7));
const SpriteDesc townHallIcon	(SPRITE_DESC_PARAM_FROM_ICON(1, 4));
const SpriteDesc moveIcon		(SPRITE_DESC_PARAM_FROM_ICON(4, 8));
const SpriteDesc stopIcon		(SPRITE_DESC_PARAM_FROM_ICON(7, 16));
const SpriteDesc cancelIcon		(SPRITE_DESC_PARAM_FROM_ICON(1, 9));
const SpriteDesc buildIcon		(SPRITE_DESC_PARAM_FROM_ICON(7, 8));
const SpriteDesc farmIcon		(SPRITE_DESC_PARAM_FROM_ICON(9, 3));

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
	unitTypeToImage[EUT_FARM] = IMG_Load("../Data/orc_building.png"); // TODO: implement cache for images

	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_IDLE] = gruntIdleAnim;
	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_MOVING] = gruntMoveAnim;
	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_ATTACK] = gruntAttackAnim;
	unitTypeStateToSpriteDesc[EUT_GRUNT][EUS_DEAD] = gruntDeadAnim;

	unitTypeStateToSpriteDesc[EUT_PEON][EUS_IDLE] = peonIdleAnim;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_IDLE_WITH_GOLD] = peonIdleWithGoldAnim;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_MOVING] = peonMovingAnim;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_MOVING_WITH_GOLD] = peonMovingWithGoldAnim;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_HARVEST] = peonHarvestAnim;
	unitTypeStateToSpriteDesc[EUT_PEON][EUS_GATHER_WOOD] = peonGatherWood;

	unitTypeStateToSpriteDesc[EUT_MINE][EUS_IDLE] = mineIdleAnim;

#define BUILD_STATES_TO_SPRITE_DESC(unitType, idleAnim, step2Anim) \
	unitTypeStateToSpriteDesc[unitType][EUS_IDLE] = idleAnim; \
	unitTypeStateToSpriteDesc[unitType][EUS_BEING_BUILD_STATE0] = buildingStep0Anim; \
	unitTypeStateToSpriteDesc[unitType][EUS_BEING_BUILD_STATE1] = buildingStep1Anim; \
	unitTypeStateToSpriteDesc[unitType][EUS_BEING_BUILD_STATE2] = step2Anim;

	BUILD_STATES_TO_SPRITE_DESC(EUT_TOWN_HALL, townHallIdleAnim, townHallBuildingStep2Anim);
	BUILD_STATES_TO_SPRITE_DESC(EUT_FARM, farmIdleAnim, farmBuildingStep2Anim);
#undef BUILD_STATES_DECL

	// ---

	unitTypeToIconSpriteDesc[EUT_PEON] = peonIcon;
	unitTypeToIconSpriteDesc[EUT_GRUNT] = gruntIcon;
	unitTypeToIconSpriteDesc[EUT_MINE] = mineIcon;
	unitTypeToIconSpriteDesc[EUT_TOWN_HALL] = townHallIcon;
	unitTypeToIconSpriteDesc[EUT_FARM] = farmIcon;
	
	// ---

	commandToIconSpriteDesc[EC_MOVE] = moveIcon;
	commandToIconSpriteDesc[EC_STOP] = stopIcon;
	commandToIconSpriteDesc[EC_CANCEL] = cancelIcon;
	commandToIconSpriteDesc[EC_TRAIN_PEON] = peonIcon;
	commandToIconSpriteDesc[EC_BUILD] = buildIcon;
	commandToIconSpriteDesc[EC_BUILD_TOWN_HALL] = townHallIcon;
	commandToIconSpriteDesc[EC_BUILD_FARM] = farmIcon;
}

// ============================================================================

void ReleaseSpriteDesc()
{
	SDL_FreeSurface(unitTypeToImage[EUT_GRUNT]);
	SDL_FreeSurface(unitTypeToImage[EUT_PEON]);
	SDL_FreeSurface(unitTypeToImage[EUT_MINE]);
	SDL_FreeSurface(unitTypeToImage[EUT_TOWN_HALL]);
	SDL_FreeSurface(unitTypeToImage[EUT_FARM]);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================