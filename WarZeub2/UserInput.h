#ifndef USER_INPUT_H_
#define USER_INPUT_H_


#include <SDL.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

struct Mouse
{
	int posX;
	int posY;
	bool leftButtonPressed;
	bool rightButtonPressed;
	int lastRightClickX;
	int lastRightClickY;
	int lastLeftClickX;
	int lastLeftClickY;
};

struct Keyboard
{
	bool keysPressed[SDLK_LAST];
};

// ============================================================================

extern Mouse mouse;
extern Keyboard keyboard;

// ============================================================================

void MouseEventHandler(const SDL_Event& parEvent);
void KeyboardEventHandler(const SDL_Event& parEvent);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif