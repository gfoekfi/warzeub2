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
					mouse.lastCameraPosOnRightClick = cameraOffset;
					if (player.selectedUnit && !HUD::Inst()->IsInHUDRegion(mouse.lastRightClickPos)
						&& player.selectedUnit->IsMovable())
					{
						Vec2 pos = mouse.lastRightClickPos;
						TransformToWorldCoordinate(pos, mouse.lastCameraPosOnRightClick);
						player.selectedUnit->Move(pos);
					}
				}
				mouse.rightButtonPressed = true;
			}
			else if (parEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (!mouse.leftButtonPressed)
				{
					mouse.lastLeftClickPos = Vec2(parEvent.motion.x, parEvent.motion.y);
					mouse.lastCameraPosOnLeftClick = cameraOffset;
				}
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
			{
				Vec2 worldPos(mouse.lastLeftClickPos);
				TransformToWorldCoordinate(worldPos, mouse.lastCameraPosOnLeftClick);
				HUD::Inst()->ApplyLastOrderAtPosition(*player.selectedUnit, worldPos);
			}
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

void KeyboardScrollingHandler()
{
	// TODO: Better and smoother implementation
	const int SCROLLING_SENSITIVITY = 5;
	cameraOffset.x += keyboard.keysPressed[SDLK_RIGHT] ? SCROLLING_SENSITIVITY : 0;
	cameraOffset.x -= keyboard.keysPressed[SDLK_LEFT] ? SCROLLING_SENSITIVITY : 0;
	cameraOffset.x = Clamp<int>(cameraOffset.x, 0,
		World::Inst()->GetMap().width * MAP_TILE_SIZE - viewport.w);

	cameraOffset.y += keyboard.keysPressed[SDLK_DOWN] ? SCROLLING_SENSITIVITY : 0;
	cameraOffset.y -= keyboard.keysPressed[SDLK_UP] ? SCROLLING_SENSITIVITY : 0;
	cameraOffset.y = Clamp<int>(cameraOffset.y, 0,
		World::Inst()->GetMap().height * MAP_TILE_SIZE - viewport.h);

	if (mouse.leftButtonPressed && !HUD::Inst()->IsInHUDRegion(mouse.lastLeftClickPos) &&
		(keyboard.keysPressed[SDLK_RIGHT] || keyboard.keysPressed[SDLK_LEFT] ||
		 keyboard.keysPressed[SDLK_UP] || keyboard.keysPressed[SDLK_DOWN]))
	{
			mouse.pos.x = (HUD::Inst()->IsInHUDRegion(mouse.pos)) ?
				(screen->w / 5) : mouse.pos.x;
			UpdateSelection(player);
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

SDL_Rect BoundingBoxFromMouse(const Mouse& parMouse, bool parTransformToWorldCoordinate)
{
	Vec2 curPos = parMouse.pos;
	Vec2 lastPos = parMouse.lastLeftClickPos;

	if (parTransformToWorldCoordinate)
	{
		TransformToWorldCoordinate(curPos, cameraOffset);
		TransformToWorldCoordinate(lastPos, mouse.lastCameraPosOnLeftClick);
	}

	SDL_Rect boundingBox =
	{ 
		(curPos.x < lastPos.x) ? curPos.x : lastPos.x,
		(curPos.y < lastPos.y) ? curPos.y : lastPos.y,
		abs(curPos.x - lastPos.x),
		abs(curPos.y - lastPos.y)
	};

	return boundingBox;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================