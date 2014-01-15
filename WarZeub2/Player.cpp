#include "Player.h"
#include "UserInput.h"
#include "Util.h"
#include "Renderer.h" //only for SCREEN_WIDTH & SCREEN_HEIGHT

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Player player;

Unit grunt =
{
	Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
	Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
	DIR_N,
	EUT_GRUNT,
	EUS_IDLE,
	0,
	0
};

Unit peon =
{
	Vec2(SCREEN_WIDTH/4, SCREEN_HEIGHT/4),
	Vec2(SCREEN_WIDTH/4, SCREEN_HEIGHT/4),
	DIR_N,
	EUT_PEON,
	EUS_IDLE,
	0,
	0
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void UpdateSelection(Player& player)
{
	SDL_Rect peonBoundingBox = BoundingBoxFromUnit(peon);
	SDL_Rect selectionBoundingBox = BoundingBoxFromMouse(mouse);

	if (DoesBBoxesCollide(&selectionBoundingBox, &peonBoundingBox))
	{
		player.selectedUnit = &peon;
		return;
	}

	SDL_Rect gruntBoundingBox = BoundingBoxFromUnit(grunt);
	if (DoesBBoxesCollide(&gruntBoundingBox, &selectionBoundingBox))
	{
		player.selectedUnit = &grunt;
		return;
	}

	player.selectedUnit = 0;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
