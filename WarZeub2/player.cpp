#include "player.h"
#include "userInput.h"
#include "util.h"
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
	TransformToWorldCoordinate(selectionBoundingBox);

	if (player.selectedUnit && player.selectedUnit->ActionState() == EUS_CHOOSE_DESTINATION)
		player.selectedUnit->SetActionState(EUS_IDLE);
	
	const std::vector<Unit*>& units = World::Inst()->Units();
	for (size_t unit = 0; unit < units.size(); ++unit)
	{
		Unit* curUnit = units[unit];
		if (curUnit->ActionState() == EUS_BUILDING)
			continue; // we can't select an unit which is currently building

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
