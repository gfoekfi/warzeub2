#include "Player.h"
#include "UserInput.h"
#include "Util.h"
#include "World.h"

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Player player;

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
