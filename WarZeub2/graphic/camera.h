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
	const float2& Pos() const { return pos_; }
	const float2& LastPosOnRightClick() const { return lastPosOnRightClick_; }
	const float2& LastPosOnLeftClick() const { return lastPosOnLeftClick_; }

	void SetPos(const float2& parPos);

private:
	float2 ScrollDirFromUserInput_() const;
	float2 MaxPosition_() const;
	bool IsValidPosition_(const float2& parPos) const;

private:
	float2 pos_;
	float2 lastPosOnRightClick_;
	float2 lastPosOnLeftClick_;
};


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif