#include "camera.h"
#include "renderer.h" // for viewport
#include "../userInput.h"
#include "../gameplay/world.h"
#include <algorithm>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Camera::Camera()
	: pos_(int2(0, 0)),
	lastPosOnRightClick_(int2(0, 0)),
	lastPosOnLeftClick_(int2(0, 0))
{
}

// ============================================================================

Camera::~Camera()
{
}

// ============================================================================

void Camera::Update(int parLastTime, int parCurTime)
{
	int2 scrollDir = ScrollDirFromUserInput_();
	if (scrollDir.x == 0 && scrollDir.y == 0)
		return; // no scrolling

	const float SCROLL_SPEED = 1000.f;
	int velocity = int(0.001f * float(parCurTime - parLastTime) * SCROLL_SPEED);
	int2 nextPos(pos_.x + velocity * scrollDir.x, pos_.y + velocity * scrollDir.y);

	int2 maxPos(
		std::max<int>(0, World::Inst()->GetMap().width * MAP_TILE_SIZE - viewport.w),
		std::max<int>(0, World::Inst()->GetMap().height * MAP_TILE_SIZE - viewport.h));
	pos_.x = Clamp<int>(nextPos.x, 0, maxPos.x);
	pos_.y = Clamp<int>(nextPos.y, 0, maxPos.y);
}

// ============================================================================

int2 Camera::ScrollDirFromUserInput_() const
{
	int2 scrollDir(0, 0);

	if (keyboard.keysPressed[SDLK_LEFT] ||
		(mouse.pos.x <= int(0.02f * float(SCREEN_WIDTH))))
		scrollDir.x--;
	if (keyboard.keysPressed[SDLK_RIGHT] ||
		(mouse.pos.x >= (SCREEN_WIDTH - int(0.02f * float(SCREEN_WIDTH)))))
		scrollDir.x++;
	if (keyboard.keysPressed[SDLK_UP] ||
		(mouse.pos.y <= int(0.02f * float(SCREEN_HEIGHT))))
		scrollDir.y--;
	if (keyboard.keysPressed[SDLK_DOWN] ||
		(mouse.pos.y >= SCREEN_HEIGHT - int(0.02f * float(SCREEN_HEIGHT))))
		scrollDir.y++;

	return scrollDir;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================