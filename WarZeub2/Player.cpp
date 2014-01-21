#include "Player.h"
#include "UserInput.h"
#include "Util.h"
#include "World.h"
#include "Unit.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Player player;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void UpdateSelection(Player& player)
{
	SDL_Rect selectionBoundingBox = BoundingBoxFromMouse(mouse);
	
	for (size_t unit = 0; unit < units.size(); ++unit)
	{
		SDL_Rect unitBoundingBox = units[unit]->BoundingBox();

		if (DoesBBoxesCollide(&selectionBoundingBox, &unitBoundingBox))
		{
			player.selectedUnit = units[unit];
			return;
		}
	}

	player.selectedUnit = 0;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
