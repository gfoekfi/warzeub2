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

void OnMouseRightButtonPressed(const SDL_Event& parEvent)
{
	assert(!mouse.rightButtonPressed);
	mouse.rightButtonPressed = true;

	mouse.lastRightClickPos = float2(parEvent.motion.x, parEvent.motion.y);
	gCamera->StorePosOnRightClick();

	if (player.selectedUnit && !HUD::Inst()->IsInHUDRegion(mouse.lastRightClickPos))
	{
		float2 pos = mouse.lastRightClickPos;
		TransformToWorldCoordinate(pos, gCamera->LastPosOnRightClick());

		Unit* pointedUnit = World::Inst()->GetUnitAt(pos);
		if (pointedUnit)
			player.selectedUnit->RightClick(pointedUnit);
		else
			player.selectedUnit->RightClick(pos);
	}
}

// ============================================================================

void OnMouseLeftButtonPressed(const SDL_Event& parEvent)
{
	assert(!mouse.leftButtonPressed);
	mouse.leftButtonPressed = true;

	mouse.lastLeftClickPos = float2(parEvent.motion.x, parEvent.motion.y);
	gCamera->StorePosOnLeftClick();

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
}

// ============================================================================

void OnMouseRightButtonReleased(const SDL_Event& parEvent)
{
	assert(mouse.rightButtonPressed);
	mouse.rightButtonPressed = false;
}

// ============================================================================

void OnMouseLeftButtonReleased(const SDL_Event& parEvent)
{
	assert(mouse.leftButtonPressed);
	mouse.leftButtonPressed = false;
}

// ============================================================================

void OnMouseMotion(const SDL_Event& parEvent)
{
	mouse.pos = float2(parEvent.motion.x, parEvent.motion.y);

	// TODO: should disappears from here
	if (mouse.leftButtonPressed && !HUD::Inst()->IsInHUDRegion(mouse.lastLeftClickPos))
	{
		mouse.pos.x = (HUD::Inst()->IsInHUDRegion(mouse.pos)) ? // FIXME: Buggy
			float(screen->w / 5) : mouse.pos.x;
		UpdateSelection(player);
	}
}

// ============================================================================

void MouseEventHandler(const SDL_Event& parEvent)
{
	switch (parEvent.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (parEvent.button.button == SDL_BUTTON_RIGHT && !mouse.rightButtonPressed)
			OnMouseRightButtonPressed(parEvent);
		else if (parEvent.button.button == SDL_BUTTON_LEFT && !mouse.leftButtonPressed)
			OnMouseLeftButtonPressed(parEvent);
		break;

	case SDL_MOUSEBUTTONUP:
		if (parEvent.button.button == SDL_BUTTON_RIGHT && mouse.rightButtonPressed)
			OnMouseRightButtonReleased(parEvent);
		else if (parEvent.button.button == SDL_BUTTON_LEFT && mouse.leftButtonPressed)
			OnMouseLeftButtonReleased(parEvent);
		break;

	case SDL_MOUSEMOTION:
		OnMouseMotion(parEvent);
		break;
	}
}

// ============================================================================

// FIXME: Remove me
void KeyboardScrollingHandler()
{
	// TODO: Move it to another place
	if (mouse.leftButtonPressed && !HUD::Inst()->IsInHUDRegion(mouse.lastLeftClickPos) &&
		(keyboard.keysPressed[SDLK_RIGHT] || keyboard.keysPressed[SDLK_LEFT] ||
		 keyboard.keysPressed[SDLK_UP] || keyboard.keysPressed[SDLK_DOWN]))
	{
			mouse.pos.x = (HUD::Inst()->IsInHUDRegion(mouse.pos)) ? // FIXME: Buggy
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

// TODO: refactor, this is a Mouse method
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