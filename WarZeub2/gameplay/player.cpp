#include "player.h"
#include "world.h"
#include "unit.h"
#include "../userInput.h"
#include "../utils/util.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Player player;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void Player::UpdateSelection()
{
	SDL_Rect selectionBoundingBox = BoundingBoxFromMouse(mouse, true);

	if (selectedUnit && selectedUnit->ActionState() == EUS_CHOOSE_DESTINATION)
		selectedUnit->SetActionState(EUS_IDLE);
	
	const std::vector<Unit*>& units = World::Inst()->Units();
	for (size_t unit = 0; unit < units.size(); ++unit)
	{
		Unit* curUnit = units[unit];
		if (curUnit->ActionState() == EUS_BUILDING)
			continue; // we can't select an unit which is currently building

		SDL_Rect unitBoundingBox = units[unit]->BoundingBox();

		if (DoesBBoxesCollide(&selectionBoundingBox, &unitBoundingBox))
		{
			selectedUnit = units[unit];
			return;
		}
	}

	selectedUnit = 0;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
