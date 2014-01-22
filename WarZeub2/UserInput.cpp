#include "UserInput.h"
#include "Player.h"
#include "renderer.h"
#include "SpriteDesc.h"
#include "World.h"
#include "UnitDesc.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Mouse mouse;
Keyboard keyboard;

// ============================================================================
// ----------------------------------------------------------------------------
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
			else
			{
				if (player.selectedUnit && mouse.lastLeftClickPos.y >= 2* screen->h / 3)
				{
					SDL_Rect mouseRect = BoundingBoxFromMouse(mouse);

					const SpriteDesc& orderIconSpriteDesc = orderToIconSpriteDesc[EO_CANCEL];
					int iconOffsetX = (screen->w / 5) / 50;
					int iconOffsetY = (2 * screen->h / 3) + ((screen->h / 3) / 50);
					SDL_Rect iconRect = {iconOffsetX, iconOffsetY, orderIconSpriteDesc.width, orderIconSpriteDesc.height};

					if (DoesBBoxesCollide(&mouseRect, &iconRect))
					{
						if (player.selectedUnit->Type() == EUT_TOWN_HALL)
							player.selectedUnit->Train(EUT_PEON);
						else if (player.selectedUnit->Type() == EUT_PEON || 
									player.selectedUnit->Type() == EUT_GRUNT)
						{
							player.selectedUnit->CancelOrder();
						}
					}
				}
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