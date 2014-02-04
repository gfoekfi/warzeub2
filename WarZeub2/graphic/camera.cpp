#include "camera.h"
#include "renderer.h" // for viewport
#include "../userInput.h"
#include "../gameplay/world.h"
#include "../gameplay/map.h"
#include <algorithm>
#include <math.h> // fabs()


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Camera::Camera()
	: pos_(float2(0, 0)),
	lastPosOnRightClick_(float2(0, 0)),
	lastPosOnLeftClick_(float2(0, 0))
{
}

// ============================================================================

Camera::~Camera()
{
}

// ============================================================================

void Camera::Update(int parLastTime, int parCurTime)
{
	float2 scrollDir = ScrollDirFromUserInput_();
	if (fabs(scrollDir.x) <= FLOAT_PRECISION && fabs(scrollDir.y) <= FLOAT_PRECISION)
		return; // no scrolling

	const float SCROLL_SPEED = 0.7f;
	float velocity = SCROLL_SPEED * float(parCurTime - parLastTime);
	float2 nextPos(pos_.x + velocity * scrollDir.x, pos_.y + velocity * scrollDir.y);

	float2 maxPos(
		std::max<float>(0.f, float(World::Inst()->GetMap().Width() * MAP_TILE_SIZE - viewport.w)),
		std::max<float>(0.f, float(World::Inst()->GetMap().Height() * MAP_TILE_SIZE - viewport.h)));
	pos_.x = Clamp<float>(nextPos.x, 0.f, maxPos.x);
	pos_.y = Clamp<float>(nextPos.y, 0.f, maxPos.y);
}

// ============================================================================

float2 Camera::ScrollDirFromUserInput_() const
{
	float2 scrollDir(0.f, 0.f);
	float2 screenDimensions = float2(float(SCREEN_WIDTH), float(SCREEN_HEIGHT));

	if (keyboard.keysPressed[SDLK_LEFT] ||
		(mouse.pos.x <= (0.02f * screenDimensions.w)))
		scrollDir.x--;
	if (keyboard.keysPressed[SDLK_RIGHT] ||
		(mouse.pos.x >= (screenDimensions.w - 0.02f * screenDimensions.w)))
		scrollDir.x++;
	if (keyboard.keysPressed[SDLK_UP] ||
		(mouse.pos.y <= (0.02f * screenDimensions.h)))
		scrollDir.y--;
	if (keyboard.keysPressed[SDLK_DOWN] ||
		(mouse.pos.y >= (screenDimensions.h - 0.02f * screenDimensions.h)))
		scrollDir.y++;

	return scrollDir;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================