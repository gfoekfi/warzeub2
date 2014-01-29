#ifndef UNIT_H_
#define UNIT_H_


#include "../util.h"
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
	EUS_DEAD,
	EUS_TRAINING,
	EUS_BUILDING,

	EUS_BEING_BUILD_STATE0,
	EUS_BEING_BUILD_STATE1,
	EUS_BEING_BUILD_STATE2,

	EUS_SELECT_BUILDING, // TODO: Should be a 'hud state'
	EUS_CHOOSE_DESTINATION, // TODO: Should be a 'hud state'
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
	virtual bool Build(EUnitType parUnitTypeToBuild, const Vec2& parPos);
	virtual bool CancelOrder();

public:
	const Vec2& Pos() const { return pos_; }
	EUnitType Type() const { return type_; }
	EUnitState MoveState() const { return moveState_; }
	EUnitState ActionState() const { return actionState_; }
	EDir Dir() const { return dir_; }
	int SpriteStep() const { return spriteStep_; }
	Uint32 SpriteLastTime() const { return spriteLastTime_; }

	void SetMoveState(EUnitState parState) { moveState_ = parState; }
	void SetActionState(EUnitState parState) { actionState_ = parState; }
	void SetDir(EDir parDir) { dir_ = parDir; }
	void SetPos(const Vec2& parPos) { pos_ = parPos; };

public:
	float OrderCompletionStatus() const; // used by hud (progress bar)
	SDL_Rect BoundingBox() const;
	bool IsMovable() const;
	bool IsBeingConstructed() const;
	bool IsBuilding(const Unit* parUnit) const;

private:
	void UpdateOrder_(Uint32 parCurTime, Uint32 parElapsedTime);
	void UpdateAnimation_(Uint32 parCurTime);

protected:
	Vec2 pos_;
	EDir dir_;
	EUnitType type_;
	EUnitState moveState_;
	EUnitState actionState_;
	int spriteStep_;
	Uint32 spriteLastTime_;
	Order* curOrder_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif