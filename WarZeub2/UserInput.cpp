#include "UserInput.h"


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
			mouse.leftButtonPressed = false;
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
// ----------------------------------------------------------------------------
// ============================================================================