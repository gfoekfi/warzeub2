#include "Player.h"
#include "UserInput.h"
#include "Util.h"
#include "world.h"
#include "unit.h"


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
	
	const std::vector<Unit*>& units = World::Inst()->Units();
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
