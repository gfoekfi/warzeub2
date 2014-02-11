#include "buildOrder.h"
#include "moveOrder.h"
#include "../gameplay/player.h"
#include "../gameplay/world.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

BuildOrder::BuildOrder(Unit *parHostUnit, EUnitType parUnitTypeToBuild, const float2& parPos)
	: Order(parHostUnit),
	unitTypeToBuild_(parUnitTypeToBuild),
	buildingUnit_(0),
	buildingStartTime_(-1),
	moveOrder_(0)
{
	buildingPos_ = parPos;
	BuildTile::Align(buildingPos_, parUnitTypeToBuild);
}

// ============================================================================

BuildOrder::~BuildOrder()
{
	if (moveOrder_)
		delete moveOrder_;

	// TODO: Move it somewhere else
	if (player.selectedUnit && player.selectedUnit == buildingUnit_)
		player.selectedUnit = hostUnit_;

	hostUnit_->SetActionState(EUS_IDLE);
}

// ============================================================================

bool BuildOrder::Update(Uint32 parCurTime, Uint32 parElapsedTime)
{
	if (!moveOrder_)
		moveOrder_ = new MoveOrder(hostUnit_, buildingPos_);
	if (!moveOrder_->Update(parCurTime, parElapsedTime))
		return false;

	if (!buildingUnit_)
	{
		buildingStartTime_ = parCurTime;
		completionStatus_ = 0.0f;

		buildingUnit_ = new Unit(buildingPos_, unitTypeToBuild_);
		buildingUnit_->SetActionState(EUS_BEING_BUILD_STATE0);

		World::Inst()->AddUnit(buildingUnit_);

		hostUnit_->SetActionState(EUS_BUILDING);

		// TODO: Move it to another place
		if (player.selectedUnit == hostUnit_)
			player.selectedUnit = buildingUnit_; // force the selection change if necessary
	}
	else
	{
		completionStatus_ = float(parCurTime - buildingStartTime_) / float(unitTypeToUnitDesc[unitTypeToBuild_].buildTime);
		completionStatus_ = Clamp(completionStatus_, 0.f, 1.f);

		if (completionStatus_ < 1.f)
		{
			assert(EUS_BEING_BUILD_STATE1 == (EUS_BEING_BUILD_STATE0 + 1));
			assert(EUS_BEING_BUILD_STATE2 == (EUS_BEING_BUILD_STATE1 + 1));

			EUnitState beingBuildState = EUnitState(EUS_BEING_BUILD_STATE0 +
				int(completionStatus_ * (EUS_BEING_BUILD_STATE2 - EUS_BEING_BUILD_STATE0 + 1)));

			buildingUnit_->SetActionState(beingBuildState);
		}
		else
		{
			buildingUnit_->SetActionState(EUS_IDLE);
			hostUnit_->SetActionState(EUS_IDLE);

			// Make the unit pop at a suitable location
			const UnitDesc& hostUnitDesc = unitTypeToUnitDesc[hostUnit_->Type()];
			int2 hostUnitDimensions(hostUnitDesc.width, hostUnitDesc.height);
			float2 newUnitPos = World::Inst()->NearestWalkableTileOf(buildingUnit_->Pos(),
				hostUnit_->Pos(), hostUnitDimensions).ToWorldPos();
			hostUnit_->SetPos(newUnitPos);

			return true;
		}
	}

	return false;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================