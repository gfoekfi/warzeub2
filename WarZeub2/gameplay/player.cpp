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

Player::Player()
	: goldAmount_(0)
{
}

// ============================================================================

Player::~Player()
{
}

// ============================================================================

void Player::IncreaseGoldAmount(int parGoldAmount)
{
	if (parGoldAmount > 0)
	{
		size_t nextGoldAmount = goldAmount_ + size_t(parGoldAmount);
		if (nextGoldAmount < goldAmount_)
			fprintf(stderr, "[PLAYER] IncreaseGoldAmount() overflow detected\n");
		else
			goldAmount_ = nextGoldAmount;
	}
	else
	{
		assert(size_t(-1 * parGoldAmount) <= goldAmount_);
		size_t nextGoldAmount = goldAmount_ - size_t(-parGoldAmount);
		if (nextGoldAmount > goldAmount_)
		{
			fprintf(stderr, "[PLAYER] IncreaseGoldAmount() overflow detected\n");
			nextGoldAmount = 0;
		}

		goldAmount_ = nextGoldAmount;
	}
}

// ============================================================================

void Player::UpdateSelection()
{
	// FIXME: Cut selection box to not consider units behind HUD
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
