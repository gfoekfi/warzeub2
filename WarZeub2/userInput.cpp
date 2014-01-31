#include "userInput.h"
#include "hud.h"
#include "graphic/renderer.h"
#include "graphic/spriteDesc.h"
#include "gameplay/player.h"
#include "gameplay/world.h"
#include "gameplay/unitDesc.h"
#include "gameplay/order.h"
#include <assert.h>
#include <math.h> // fabs()


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
					mouse.lastRightClickPos = float2(parEvent.motion.x, parEvent.motion.y);
					gCamera->StorePosOnRightClick();
					// TODO: Refactor (it's getting messy)
					if (player.selectedUnit && !HUD::Inst()->IsInHUDRegion(mouse.lastRightClickPos)
						&& player.selectedUnit->IsMovable())
					{
						float2 pos = mouse.lastRightClickPos;
						TransformToWorldCoordinate(pos, gCamera->LastPosOnRightClick());

						Unit* pointedUnit = World::Inst()->GetUnitAt(pos);
						if (pointedUnit && pointedUnit->Type() == EUT_MINE && player.selectedUnit->Type() == EUT_PEON)
							player.selectedUnit->Gather(pointedUnit);
						else
							player.selectedUnit->Move(pos);
					}
				}
				mouse.rightButtonPressed = true;
			}
			else if (parEvent.button.button == SDL_BUTTON_LEFT)
			{
				if (!mouse.leftButtonPressed)
				{
					mouse.lastLeftClickPos = float2(parEvent.motion.x, parEvent.motion.y);
					gCamera->StorePosOnLeftClick();
				}
				mouse.leftButtonPressed = true;
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (parEvent.button.button == SDL_BUTTON_RIGHT)
			mouse.rightButtonPressed = false;
		else if (parEvent.button.button == SDL_BUTTON_LEFT && mouse.leftButtonPressed)
		{ // prevents SDL send 'up' events multiple times
			if (HUD::Inst()->IsInHUDRegion(mouse.lastLeftClickPos))
				HUD::Inst()->GridClickHandler();
			else if (player.selectedUnit && player.selectedUnit->ActionState() == EUS_CHOOSE_DESTINATION)
			{
				float2 worldPos(mouse.lastLeftClickPos);
				TransformToWorldCoordinate(worldPos, gCamera->LastPosOnLeftClick());
				HUD::Inst()->ApplyLastOrderAtPosition(*player.selectedUnit, worldPos);
			}
			else
				UpdateSelection(player);
			mouse.leftButtonPressed = false;
		}
		break;

	case SDL_MOUSEMOTION:
		mouse.pos = float2(parEvent.motion.x, parEvent.motion.y);
		// TODO: should disappears from here
		if (mouse.leftButtonPressed && !HUD::Inst()->IsInHUDRegion(mouse.lastLeftClickPos))
		{
			mouse.pos.x = (HUD::Inst()->IsInHUDRegion(mouse.pos)) ?
				float(screen->w / 5) : mouse.pos.x;
			UpdateSelection(player);
		}
		break;
	}
}

// ============================================================================

void KeyboardScrollingHandler()
{
	// TODO: Move it to another place
	if (mouse.leftButtonPressed && !HUD::Inst()->IsInHUDRegion(mouse.lastLeftClickPos) &&
		(keyboard.keysPressed[SDLK_RIGHT] || keyboard.keysPressed[SDLK_LEFT] ||
		 keyboard.keysPressed[SDLK_UP] || keyboard.keysPressed[SDLK_DOWN]))
	{
			mouse.pos.x = (HUD::Inst()->IsInHUDRegion(mouse.pos)) ?
				float(screen->w / 5) : mouse.pos.x;
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
	float2 curPos = parMouse.pos;
	float2 lastPos = parMouse.lastLeftClickPos;

	if (parTransformToWorldCoordinate)
	{
		TransformToWorldCoordinate(curPos, gCamera->Pos());
		TransformToWorldCoordinate(lastPos, gCamera->LastPosOnLeftClick());
	}

	SDL_Rect boundingBox =
	{ 
		Sint16((curPos.x < lastPos.x) ? curPos.x : lastPos.x),
		Sint16((curPos.y < lastPos.y) ? curPos.y : lastPos.y),
		// width and height should be at least 1, otherwise it's a point not a box
		std::max<Sint16>(1, Sint16(fabs(curPos.x - lastPos.x))),
		std::max<Sint16>(1, Sint16(fabs(curPos.y - lastPos.y)))
	};

	return boundingBox;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================