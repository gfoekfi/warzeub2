#ifndef USER_INPUT_H_
#define USER_INPUT_H_


#include "utils/util.h"
#include <SDL.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

struct Mouse
{
	float2 pos;
	float2 lastRightClickPos;
	float2 lastLeftClickPos;

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

void UpdateSelectionOnKeyboardScrollingIFN();

SDL_Rect BoundingBoxFromMouse(const Mouse& parMouse, bool parTransformToWorldCoordinate);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif