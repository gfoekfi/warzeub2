#include "userInput.h"
#include "player.h"
#include "renderer.h"
#include "spriteDesc.h"
#include "world.h"
#include "unitDesc.h"
#include "order.h"
#include "hud.h"
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Mouse mouse;
Keyboard keyboard;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

// TODO: Implement a better event handler
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
					if (player.selectedUnit && !HUD::Inst()->IsInHUDRegion(mouse.lastRightClickPos))
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
			if (HUD::Inst()->IsInHUDRegion(mouse.lastLeftClickPos))
				HUD::Inst()->GridClickHandler();
			else if (player.selectedUnit && player.selectedUnit->ActionState() == EUS_CHOOSE_DESTINATION)
				HUD::Inst()->ApplyLastOrderAtPosition(*player.selectedUnit, mouse.lastLeftClickPos);
			else
				UpdateSelection(player);
			mouse.leftButtonPressed = false;
		}
		break;

	case SDL_MOUSEMOTION:
		mouse.pos = Vec2(parEvent.motion.x, parEvent.motion.y);
		if (mouse.leftButtonPressed && !HUD::Inst()->IsInHUDRegion(mouse.lastLeftClickPos))
		{
			mouse.pos.x = (HUD::Inst()->IsInHUDRegion(mouse.pos)) ?
				(screen->w / 5) : mouse.pos.x;
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