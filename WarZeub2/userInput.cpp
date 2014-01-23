#include "userInput.h"
#include "player.h"
#include "renderer.h"
#include "spriteDesc.h"
#include "world.h"
#include "unitDesc.h"
#include "order.h"
#include <assert.h>
#include <algorithm> // for set_intersection


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Mouse mouse;
Keyboard keyboard;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

int GetGridClickPosition()
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

void ApplyGridClick(Unit& parUnit, int parGridClickPos)
{
	assert(parGridClickPos >= 0 && parGridClickPos <= 8); // grid is 3x3

	const std::set<EOrder>& ordersOnGridPos = gridPosToOrders[parGridClickPos];
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

void MouseEventHandler(const SDL_Event& parEvent)
{
	switch (parEvent.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		{
			if (parEvent.button.button == SDL_BUTTON_RIGHT)
			{
				if (!mouse.rightButtonPressed)
				{
					mouse.lastRightClickPos = Vec2(parEvent.motion.x, parEvent.motion.y);
					if (player.selectedUnit && (mouse.lastRightClickPos.x >= (screen->w / 5)))
						player.selectedUnit->Move(mouse.lastRightClickPos);
				}
				mouse.rightButtonPressed = true;
			}
			else if (parEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (!mouse.leftButtonPressed)
					mouse.lastLeftClickPos = Vec2(parEvent.motion.x, parEvent.motion.y);
				mouse.leftButtonPressed = true;
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (parEvent.button.button == SDL_BUTTON_RIGHT)
			mouse.rightButtonPressed = false;
		else if (parEvent.button.button == SDL_BUTTON_LEFT
			&& mouse.leftButtonPressed) // prevents SDL send 'up' events multiple times
		{
			if (mouse.lastLeftClickPos.x >= screen->w / 5)
				UpdateSelection(player);
			else if (player.selectedUnit && mouse.lastLeftClickPos.y >= 2* screen->h / 3)
			{ // grid region
				int gridClickPos = GetGridClickPosition();
				if (gridClickPos != -1)
					ApplyGridClick(*player.selectedUnit, gridClickPos);
			}

			mouse.leftButtonPressed = false;
		}
		break;

	case SDL_MOUSEMOTION:
		mouse.pos = Vec2(parEvent.motion.x, parEvent.motion.y);
		if (mouse.leftButtonPressed && (mouse.lastLeftClickPos.x >= screen->w / 5))
		{
			mouse.pos.x = (mouse.pos.x < (screen->w / 5)) ? (screen->w / 5) : mouse.pos.x;
			UpdateSelection(player);
		}
		break;
	}
}

// ============================================================================

void KeyboardEventHandler(const SDL_Event& parEvent)
{
	switch (parEvent.type)
	{
	case SDL_KEYDOWN:
		keyboard.keysPressed[parEvent.key.keysym.sym] = true;
		break;

	case SDL_KEYUP:
		keyboard.keysPressed[parEvent.key.keysym.sym] = false;
		break;
	}
}

// ============================================================================

SDL_Rect BoundingBoxFromMouse(const Mouse& parMouse)
{
	int width = abs(parMouse.pos.x - parMouse.lastLeftClickPos.x);
	int height = abs(parMouse.pos.y - parMouse.lastLeftClickPos.y);

	SDL_Rect boundingBox =
	{ 
		(parMouse.pos.x < parMouse.lastLeftClickPos.x) ? 
			parMouse.pos.x : parMouse.lastLeftClickPos.x,
		(parMouse.pos.y < parMouse.lastLeftClickPos.y) ?
			parMouse.pos.y : parMouse.lastLeftClickPos.y, 
		width, 
		height
	};

	return boundingBox;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================