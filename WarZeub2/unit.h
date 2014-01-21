#ifndef UNIT_H_
#define UNIT_H_


#include "Util.h"
#include <SDL.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

enum EUnitType
{
	EUT_PEON = 0,
	EUT_GRUNT,
	EUT_AXE_THROWER,
	EUT_MINE,
	EUT_TOWN_HALL,
};

// ============================================================================

enum EUnitState
{
	EUS_IDLE = 0,
	EUS_MOVE,
	EUS_ATTACK,
	EUS_HARVEST,
	EUS_GATHER_GOLD,
	EUS_GATHER_WOOD,
	EUS_DEAD
};

// ============================================================================

class Order;

class Unit
{
public:
	Unit(const Vec2& parPos, EUnitType parType);
	virtual ~Unit();

public:
	virtual void Update(Uint32 parCurTime, Uint32 parElapsedTime);
	virtual bool Train(EUnitType parUnitTypeToTrain);
	virtual bool Move(const Vec2& parTargetPos);

public:
	const Vec2& Pos() const { return pos_; }
	EUnitType Type() const { return type_; }
	EUnitState State() const { return state_; }
	EDir Dir() const { return dir_; }
	int SpriteStep() const { return spriteStep_; }
	Uint32 SpriteLastTime() const { return spriteLastTime_; }

	void SetState(EUnitState parUnitState) { state_ = parUnitState; }
	void SetDir(EDir parDir) { dir_ = parDir; }
	void SetPos(const Vec2& parPos) { pos_ = parPos; };

public:
	SDL_Rect BoundingBox() const;

private:
	void UpdateOrder_(Uint32 parElapsedTime);
	void UpdateAnimation_(Uint32 parCurTime);

private:
	bool IsMovable_() const;

protected:
	Vec2 pos_;
	EDir dir_;
	EUnitType type_;
	EUnitState state_;
	int spriteStep_;
	Uint32 spriteLastTime_;
	Order* curOrder_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif