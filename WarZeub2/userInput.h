#ifndef USER_INPUT_H_
#define USER_INPUT_H_


#include "utils/util.h"
#include <SDL.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

struct Mouse
{
	int2 pos;
	int2 lastRightClickPos;
	int2 lastLeftClickPos;

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

void KeyboardScrollingHandler();

SDL_Rect BoundingBoxFromMouse(const Mouse& parMouse, bool parTransformToWorldCoordinate);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif