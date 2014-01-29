#ifndef CAMERA_H_
#define CAMERA_H_


#include "util.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class Camera
{
public:
	Camera();
	~Camera();

public:
	void StorePosOnLeftClick() { lastPosOnLeftClick_ = pos_; }
	void StorePosOnRightClick() { lastPosOnRightClick_ = pos_; }

public:
	const Vec2& Pos() const { return pos_; }
	const Vec2& LastPosOnRightClick() const { return lastPosOnRightClick_; }
	const Vec2& LastPosOnLeftClick() const { return lastPosOnLeftClick_; }

	void SetPos(const Vec2& parPos) { pos_ = parPos; }

private:
	Vec2 pos_;
	Vec2 lastPosOnRightClick_;
	Vec2 lastPosOnLeftClick_;
};


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif