#include "Unit.h"
#include "Util.h"
#include <stdlib.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

struct Dir
{
	int x;
	int y;
} dirs[MAX_DIRS] =
{
	{ 0, -1 }, // N
	{ 1, -1 },
	{ 1, 0 },	// E
	{ 1, 1 },
	{ 0, 1 },	// S
	{ -1, 1},
	{ -1, 0},	// W
	{ -1, -1}
};

// ============================================================================

void Update(Unit& parUnit, Uint32 parCurTime, Uint32 parElapsedTime)
{
	int deltaPosX = parUnit.targetPosX - parUnit.posX;
	int deltaPosY = parUnit.targetPosY - parUnit.posY;

	if (abs(deltaPosX) <= 3)
		parUnit.targetPosX = parUnit.posX;
	if (abs(deltaPosY) <= 3)
		parUnit.targetPosY = parUnit.posY;

	if (abs(deltaPosX) > 3 || abs(deltaPosY) > 3)
		parUnit.state = EUS_MOVE;
	else
		parUnit.state = EUS_IDLE;

	const int MOVE_STEP_TRESHOLD = 1;
	if (parUnit.state == EUS_MOVE)
	{
		if (deltaPosY >= MOVE_STEP_TRESHOLD)
		{
			if (deltaPosX <= -MOVE_STEP_TRESHOLD)
				parUnit.dir = DIR_SW;
			else if (deltaPosX >= MOVE_STEP_TRESHOLD)
				parUnit.dir = DIR_SE;
			else
				parUnit.dir = DIR_S;
		}
		else if (deltaPosY <= -MOVE_STEP_TRESHOLD)
		{
			if (deltaPosX <= -MOVE_STEP_TRESHOLD)
				parUnit.dir = DIR_NW;
			else if (deltaPosX >= MOVE_STEP_TRESHOLD)
				parUnit.dir = DIR_NE;
			else
				parUnit.dir = DIR_N;
		}
		else if (deltaPosX <= -MOVE_STEP_TRESHOLD)
			parUnit.dir = DIR_W;
		else if (deltaPosX >= MOVE_STEP_TRESHOLD)
			parUnit.dir = DIR_E;
		else
		{
			parUnit.state = EUS_IDLE;
		}

		int velX = dirs[parUnit.dir].x * (parElapsedTime / 3);
		int velY = dirs[parUnit.dir].y * (parElapsedTime / 3);
		if (abs(velX) > abs(deltaPosX))
			velX = deltaPosX;
		if (abs(velY) > abs(deltaPosY))
			velY = deltaPosY;
		
		parUnit.posX += velX;
		parUnit.posY += velY;
	}

	// sprite animation
	Uint32 deltaTime = parCurTime - parUnit.spriteLastTime;
	if (deltaTime > 170) // 170
	{
		parUnit.spriteStep++;
		parUnit.spriteLastTime = parCurTime;
	}
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================