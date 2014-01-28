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

	Vec2 lastCameraPosOnRightClick;	// It might sounds odd to declare this here,
	Vec2 lastCameraPosOnLeftClick;	// but it makes things easier

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