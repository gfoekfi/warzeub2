#include "camera.h"
#include "renderer.h" // for viewport
#include "../userInput.h"
#include "../gameplay/world.h"
#include "../util.h"
#include <algorithm>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Camera::Camera()
	: pos_(Vec2(0, 0)),
	lastPosOnRightClick_(Vec2(0, 0)),
	lastPosOnLeftClick_(Vec2(0, 0))
{
}

// ============================================================================

Camera::~Camera()
{
}

// ============================================================================

void Camera::Update(int parLastTime, int parCurTime)
{
	Vec2 scrollDir(0, 0);
	if (keyboard.keysPressed[SDLK_LEFT])
		scrollDir.x--;
	if (keyboard.keysPressed[SDLK_RIGHT])
		scrollDir.x++;
	if (keyboard.keysPressed[SDLK_UP])
		scrollDir.y--;
	if (keyboard.keysPressed[SDLK_DOWN])
		scrollDir.y++;

	if (scrollDir.x == 0 && scrollDir.y == 0)
		return; // no scrolling

	const float SCROLL_SPEED = 1000.f; // in # of pixels per seconds
	int velocity = int(0.001f * float(parCurTime - parLastTime) * SCROLL_SPEED);
	Vec2 nextPos(pos_.x + velocity * scrollDir.x, pos_.y + velocity * scrollDir.y);

	Vec2 maxPos(
		std::max<int>(0, World::Inst()->GetMap().width * MAP_TILE_SIZE - viewport.w),
		std::max<int>(0, World::Inst()->GetMap().height * MAP_TILE_SIZE - viewport.h));
	pos_.x = Clamp<int>(nextPos.x, 0, maxPos.x);
	pos_.y = Clamp<int>(nextPos.y, 0, maxPos.y);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================