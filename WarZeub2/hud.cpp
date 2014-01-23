#include "hud.h"
#include "renderer.h"
#include "player.h"
#include "userInput.h"
#include "spriteDesc.h"
#include <assert.h>
#include <algorithm>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

HUD::HUD()
{
	InitOrderGridPosMapping_();
}

// ============================================================================

HUD::~HUD()
{
}

// ============================================================================

void HUD::InitOrderGridPosMapping_()
{
	orderToGridPos_[EO_MOVE] = 0;
	orderToGridPos_[EO_STOP] = 1;
	orderToGridPos_[EO_CANCEL] = 8;
	orderToGridPos_[EO_TRAIN_PEON] = 0;

	for (std::map<EOrder, int>::iterator it = orderToGridPos_.begin();
			it != orderToGridPos_.end(); ++it)
	{
		gridPosToOrders_[it->second].insert(it->first);
	}
}

// ============================================================================

bool HUD::IsInHUDRegion(const Vec2& parPos) const
{
	return (parPos.x <= (screen->w / 5));
}

// ============================================================================

int HUD::GridClickPositionFromMouse_()
{
	SDL_Rect mouseRect = BoundingBoxFromMouse(mouse);
	const SpriteDesc& orderIconSpriteDesc = orderToIconSpriteDesc[EO_CANCEL];

	for (int line = 0; line < 3; ++line)
	{
		for (int col = 0; col < 3; ++col)
		{
			int iconOffsetX = (screen->w / 5) / 50 + col * (orderIconSpriteDesc.width + 5);
			int iconOffsetY = (2 * screen->h / 3) + ((screen->h / 3) / 50) + line * (orderIconSpriteDesc.height + 5);
			SDL_Rect iconRect = {iconOffsetX, iconOffsetY, orderIconSpriteDesc.width, orderIconSpriteDesc.height};

			if (DoesBBoxesCollide(&mouseRect, &iconRect))
				return (line*3 + col);
		}
	}

	return -1;
}

// ============================================================================

void HUD::ApplyGridClick_(Unit& parUnit, int parGridClickPos)
{
	assert(parGridClickPos >= 0 && parGridClickPos <= 8); // grid is 3x3

	const std::set<EOrder>& ordersOnGridPos = gridPosToOrders_[parGridClickPos];
	UnitDesc& unitDesc = unitTypeToUnitDesc[parUnit.Type()];
	const std::set<EOrder>& ordersFromUnitState =
		unitDesc.unitStateToOrderSet[parUnit.ActionState()];

	std::set<EOrder> intersectSet;
	std::set_intersection(ordersOnGridPos.begin(), ordersOnGridPos.end(),
		ordersFromUnitState.begin(), ordersFromUnitState.end(),
		std::inserter(intersectSet, intersectSet.begin()));

	if (intersectSet.size() > 0)
	{
		assert(intersectSet.size() == 1);
		EOrder order = *intersectSet.begin();
		switch (order)
		{
		case EO_CANCEL:
		case EO_STOP: parUnit.CancelOrder(); break;
		case EO_MOVE: parUnit.Move(mouse.lastRightClickPos); break; // FIXME: Should enter in 'You have to click' mode
		case EO_TRAIN_PEON: parUnit.Train(EUT_PEON); break;
		default: break;
		};
	}
}

// ============================================================================

void HUD::GridClickHandler()
{
	if (!player.selectedUnit || mouse.lastLeftClickPos.y < 2* screen->h / 3)
		return;

	int gridClickPos = GridClickPositionFromMouse_();
	if (gridClickPos != -1)
		ApplyGridClick_(*player.selectedUnit, gridClickPos);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================