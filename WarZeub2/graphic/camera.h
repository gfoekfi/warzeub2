#ifndef CAMERA_H_
#define CAMERA_H_


#include "../utils/util.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class Camera
{
public:
	Camera();
	~Camera();

public:
	void Update(int parLastTime, int parCurTime);

public:
	void StorePosOnLeftClick() { lastPosOnLeftClick_ = pos_; }
	void StorePosOnRightClick() { lastPosOnRightClick_ = pos_; }

public:
	const int2& Pos() const { return pos_; }
	const int2& LastPosOnRightClick() const { return lastPosOnRightClick_; }
	const int2& LastPosOnLeftClick() const { return lastPosOnLeftClick_; }

	void SetPos(const int2& parPos) { pos_ = parPos; }

private:
	int2 ScrollDirFromUserInput_() const;

private:
	int2 pos_;
	int2 lastPosOnRightClick_;
	int2 lastPosOnLeftClick_;
};


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif