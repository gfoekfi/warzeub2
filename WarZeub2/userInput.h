#ifndef USER_INPUT_H_
#define USER_INPUT_H_


#include "util.h"
#include <SDL.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

struct Mouse
{
	Vec2 pos;
	Vec2 lastRightClickPos;
	Vec2 lastLeftClickPos;

	bool leftButtonPressed;
	bool rightButtonPressed;
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

SDL_Rect BoundingBoxFromMouse(const Mouse& parMouse);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif