#ifndef UNIT_H_
#define UNIT_H_


#include "../utils/util.h"
#include <SDL.h>
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

enum EUnitType
{
	EUT_PEON = 0,
	EUT_GRUNT,
	EUT_MINE,
	EUT_TOWN_HALL,
	EUT_FARM,
};

// ============================================================================

enum EUnitState
{
	EUS_IDLE = 0,
	EUS_IDLE_WITH_GOLD,
	EUS_MOVING,
	EUS_MOVING_WITH_GOLD,

	EUS_ATTACK,
	EUS_HARVEST,

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
	Unit(const float2& parPos, EUnitType parType);
	virtual ~Unit();

public:
	virtual void Update(Uint32 parCurTime, Uint32 parElapsedTime);

	virtual bool Gather(Unit* parMineUnit);
	virtual bool Train(EUnitType parUnitTypeToTrain);
	virtual bool Move(const float2& parTargetPos);
	virtual bool Build(EUnitType parUnitTypeToBuild, const float2& parPos);
	virtual bool CancelOrder();

	virtual void Render() const;
	virtual void RightClick(Unit* parTargetUnit);
	virtual void RightClick(const float2& parTargetpos);

public:
	float OrderCompletionStatus() const; // used by hud (progress bar)
	SDL_Rect BoundingBox() const;
	bool CanMove() const;
	bool IsBeingConstructed() const;
	bool IsBuilding(const Unit* parUnit) const;

	bool IsHoldingGold() const { return holdingGold_; }

public:
	const float2& Pos() const { return pos_; }
	EUnitType Type() const { return type_; }
	EUnitState ActionState() const { return actionState_; }
	EDir Dir() const { return dir_; }
	int SpriteStep() const { return spriteStep_; }
	Uint32 SpriteLastTime() const { return spriteLastTime_; }

	// TODO: Might need to be deleted and make Orders class friend
	void SetMoving(bool parMoving) { moving_ = parMoving; }
	void SetHoldingGold(bool parHoldingGold) { holdingGold_ = parHoldingGold; }
	//

	void SetActionState(EUnitState parState) { actionState_ = parState; }
	void SetDir(EDir parDir) { dir_ = parDir; }
	void SetPos(const float2& parPos) { pos_ = parPos; };

protected:
	void UpdateOrder_(Uint32 parCurTime, Uint32 parElapsedTime);
	void UpdateAnimation_(Uint32 parCurTime);

protected:
	float2 pos_;
	EDir dir_;
	EUnitType type_;

	bool moving_;
	bool holdingGold_;

	EUnitState actionState_;
	int spriteStep_;
	Uint32 spriteLastTime_;
	Order* curOrder_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif