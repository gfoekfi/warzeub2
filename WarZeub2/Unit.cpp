#include "Unit.h"
#include "Util.h"
#include "UnitDesc.h"
#include <stdlib.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

Vec2 dirs[MAX_DIRS] =
{
	Vec2(0, -1), // N
	Vec2(1, -1),
	Vec2(1, 0),	// E
	Vec2(1, 1),
	Vec2(0, 1),	// S
	Vec2(-1, 1),
	Vec2(-1, 0),	// W
	Vec2(-1, -1)
};

#if 0

};
#endif

// ============================================================================

EDir DirectionToTarget(const Unit& parUnit)
{
	int deltaPosX = parUnit.targetPos.x - parUnit.pos.x;
	int deltaPosY = parUnit.targetPos.y - parUnit.pos.y;
	const int MOVE_STEP_TRESHOLD = 1;

	if (deltaPosY >= MOVE_STEP_TRESHOLD)
	{
		if (deltaPosX <= -MOVE_STEP_TRESHOLD)
			return DIR_SW;
		else if (deltaPosX >= MOVE_STEP_TRESHOLD)
			return DIR_SE;
		else
			return DIR_S;
	}
	else if (deltaPosY <= -MOVE_STEP_TRESHOLD)
	{
		if (deltaPosX <= -MOVE_STEP_TRESHOLD)
			return DIR_NW;
		else if (deltaPosX >= MOVE_STEP_TRESHOLD)
			return DIR_NE;
		else
			return DIR_N;
	}
	else if (deltaPosX <= -MOVE_STEP_TRESHOLD)
		return DIR_W;
	else if (deltaPosX >= MOVE_STEP_TRESHOLD)
		return DIR_E;

	return DIR_N;
}

// ============================================================================

void UpdatePosition(Unit& parUnit, Uint32 parElapsedTime)
{
	int deltaPosX = parUnit.targetPos.x - parUnit.pos.x;
	int deltaPosY = parUnit.targetPos.y - parUnit.pos.y;

	// Hack to force the convergence to target
	if (abs(deltaPosX) <= 3)
		parUnit.targetPos.x = parUnit.pos.x;
	if (abs(deltaPosY) <= 3)
		parUnit.targetPos.y = parUnit.pos.y;

	parUnit.state = (abs(deltaPosX) > 3 || abs(deltaPosY) > 3) ? EUS_MOVE : EUS_IDLE;

	if (parUnit.state == EUS_MOVE) // TODO: Add 'GatherGold', 'GatherWood'
	{
		parUnit.dir = DirectionToTarget(parUnit);

		int velX = dirs[parUnit.dir].x * (parElapsedTime / 3);
		int velY = dirs[parUnit.dir].y * (parElapsedTime / 3);
		if (abs(velX) > abs(deltaPosX))
			velX = deltaPosX;
		if (abs(velY) > abs(deltaPosY))
			velY = deltaPosY;
		
		parUnit.pos.x += velX;
		parUnit.pos.y += velY;
	}
}

// ============================================================================

void UpdateAnimation(Unit& parUnit, Uint32 parCurTime)
{
	Uint32 deltaTime = parCurTime - parUnit.spriteLastTime;
	if (deltaTime > 180) // 180 = grunt
	{
		parUnit.spriteStep++;
		parUnit.spriteLastTime = parCurTime;
	}
}

// ============================================================================

void Update(Unit& parUnit, Uint32 parCurTime, Uint32 parElapsedTime)
{
	UpdatePosition(parUnit, parElapsedTime);
#if 0
	//parUnit.state = EUS_HARVEST;
	//parUnit.state = EUS_GATHER_WOOD;
	//parUnit.state = EUS_MOVE;
	//parUnit.dir = DIR_S;
#endif
	UpdateAnimation(parUnit, parCurTime);
}

// ============================================================================

SDL_Rect BoundingBoxFromUnit(const Unit& parUnit)
{
	const UnitDesc& unitDesc = unitTypeToUnitDesc[parUnit.type];

	SDL_Rect boundingBox = { 
		parUnit.pos.x - unitDesc.width / 2,
		parUnit.pos.y - unitDesc.height / 2, 
		unitDesc.width, 
		unitDesc.height 
	};

	return boundingBox;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================