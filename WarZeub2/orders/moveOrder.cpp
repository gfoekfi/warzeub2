#include "moveOrder.h"
#include "../gameplay/world.h"
#include "../gameplay/path.h"
#include <math.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

MoveOrder::MoveOrder(Unit* parHostUnit, const float2& parTargetPos)
	: Order(parHostUnit),
	targetPos_(parTargetPos),
	path_(),
	curWaypoint_(0)
{
	assert(parHostUnit);

	SetTargetPos(parTargetPos);
}

// ============================================================================

MoveOrder::~MoveOrder()
{
	if (path_)
		delete path_;
}

// ============================================================================

bool MoveOrder::Update(Uint32 parCurTime, Uint32 parElapsedTime)
{
	assert(hostUnit_->Type() == EUT_PEON || hostUnit_->Type() == EUT_GRUNT);
	assert(path_);

	if (!path_->HasPath())
	{
		hostUnit_->SetMoving(false);
		return false;
	}

	float2 deltaPos(
		targetPos_.x - hostUnit_->Pos().x,
		targetPos_.y - hostUnit_->Pos().y);
	if (fabs(deltaPos.x) <= 1.f && fabs(deltaPos.y) <= 1.f)
	{
		if (curWaypoint_ < (path_->PathSize() - 1))
		{ // next Target position
			curWaypoint_++;
			const int2& nextTargetTile = path_->TileFromWaypoint(curWaypoint_);
			targetPos_ = float2((0.5f + float(nextTargetTile.x)) * MAP_BUILD_TILE_SIZE,
									  (0.5f + float(nextTargetTile.y)) * MAP_BUILD_TILE_SIZE);
			return false;
		}
		else
		{ // move order complete
			targetPos_ = hostUnit_->Pos();
			hostUnit_->SetMoving(false);
			return true;
		}
	}

	hostUnit_->SetMoving(true);
	hostUnit_->SetDir(DirectionToTarget(hostUnit_->Pos(), targetPos_));

	const float moveSpeed = 0.02f * float(unitTypeToUnitDesc[hostUnit_->Type()].moveSpeed);
	float2 velocity(
		moveSpeed * float(parElapsedTime) * dirs[hostUnit_->Dir()].x,
		moveSpeed * float(parElapsedTime) * dirs[hostUnit_->Dir()].y);
	if (fabs(deltaPos.x) < fabs(velocity.x))
		velocity.x = deltaPos.x;
	if (fabs(deltaPos.y) < fabs(velocity.y))
		velocity.y = deltaPos.y;

	float2 nextPos(hostUnit_->Pos().x + velocity.x, hostUnit_->Pos().y + velocity.y);

	SDL_Rect nextBoundingBox = hostUnit_->BoundingBox();
	nextBoundingBox.x = Sint16(nextPos.x) - nextBoundingBox.w / 2;
	nextBoundingBox.y = Sint16(nextPos.y) - nextBoundingBox.h / 2;
	if (World::Inst()->Collides(hostUnit_, nextBoundingBox))
		hostUnit_->SetMoving(false);
	else
		hostUnit_->SetPos(nextPos);

	return false;
}

// ============================================================================

void MoveOrder::SetTargetPos(const float2& parTargetPos)
{
	// TODO: assert parTargetPos is different from old path
	if (path_)
		delete path_;

	const UnitDesc& unitDesc = unitTypeToUnitDesc[hostUnit_->Type()];
	int2 unitDimensions(unitDesc.width, unitDesc.height);
	path_ = new Path(hostUnit_->Pos(), parTargetPos, unitDimensions);
	curWaypoint_ = 0;
	assert(path_);

	if (path_->HasPath())
	{
		curWaypoint_ = 0;
		const int2& nextTargetTile = path_->TileFromWaypoint(curWaypoint_);
		targetPos_ = float2((0.5f + float(nextTargetTile.x)) * MAP_BUILD_TILE_SIZE,
								  (0.5f + float(nextTargetTile.y)) * MAP_BUILD_TILE_SIZE);
	}
	else
	{
		// TODO: alert("Can't move here");
		targetPos_ = hostUnit_->Pos();
	}
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================