#include "UserInput.h"
#include "Player.h"


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
				mouse.lastRightClickX = parEvent.button.x;
				mouse.lastRightClickY = parEvent.button.y;
			}
			else if (parEvent.button.button == SDL_BUTTON_LEFT)
			{
				mouse.leftButtonPressed = true;
				mouse.lastLeftClickX = parEvent.button.x;
				mouse.lastLeftClickY = parEvent.button.y;
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (parEvent.button.button == SDL_BUTTON_LEFT)
		{
			UpdateSelection(player);
			mouse.leftButtonPressed = false;
		}
		break;

	case SDL_MOUSEMOTION:
		mouse.posX = parEvent.motion.x;
		mouse.posY = parEvent.motion.y;
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
	int width = abs(parMouse.posX - parMouse.lastLeftClickX);
	int height = abs(parMouse.posY - parMouse.lastLeftClickY);

	SDL_Rect boundingBox =
	{ 
		(parMouse.posX < parMouse.lastLeftClickX) ? parMouse.posX : parMouse.lastLeftClickX,
		(parMouse.posY < parMouse.lastLeftClickY) ? parMouse.posY : parMouse.lastLeftClickY, 
		width, 
		height
	};

	return boundingBox;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================