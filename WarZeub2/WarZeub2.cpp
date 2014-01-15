#include "Unit.h"
#include "SpriteDesc.h"
#include "UnitDesc.h"
#include "Renderer.h"
#include "UserInput.h"
#include "Player.h"
#include "World.h"

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

bool isDone = false;
Uint32 lastTime = 0;
Uint32 curTime = 0;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

bool Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_WM_SetCaption("WarZeub 2 !", 0);
	
	InitRenderer();
	InitSpriteDesc();
	InitUnitDesc();

	player.selectedUnit = &peon;

	return true;
}

// ============================================================================

void Quit()
{
	ReleaseSpriteDesc();
	ReleaseRenderer();

	SDL_Quit();
}

// ============================================================================

void EventHandler(const SDL_Event& parEvent)
{
	switch (parEvent.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEMOTION:
		MouseEventHandler(parEvent);
		break;

	case SDL_KEYDOWN:
	case SDL_KEYUP:
		KeyboardEventHandler(parEvent);
		break;

	case SDL_QUIT:
		isDone = true;
		break;
	}
}

// ============================================================================

void DirectionFromKeys()
{
	EUnitState prevState = grunt.state;
	//grunt.state = keyboard.keysPressed[SDLK_SPACE] ? EUS_DEAD : EUS_MOVE;
	grunt.state = keyboard.keysPressed[SDLK_SPACE] ? EUS_ATTACK : EUS_MOVE;

	if (keyboard.keysPressed[SDLK_UP])
	{
		if (keyboard.keysPressed[SDLK_LEFT])
			grunt.dir = DIR_NW;
		else if (keyboard.keysPressed[SDLK_RIGHT])
			grunt.dir = DIR_NE;
		else
			grunt.dir = DIR_N;
	}
	else if (keyboard.keysPressed[SDLK_DOWN])
	{
		if (keyboard.keysPressed[SDLK_LEFT])
			grunt.dir = DIR_SW;
		else if (keyboard.keysPressed[SDLK_RIGHT])
			grunt.dir = DIR_SE;
		else
			grunt.dir = DIR_S;
	}
	else if (keyboard.keysPressed[SDLK_LEFT])
		grunt.dir = DIR_W;
	else if (keyboard.keysPressed[SDLK_RIGHT])
		grunt.dir = DIR_E;
	else
	{
		grunt.state = EUS_IDLE;
	}

	if (grunt.state != prevState)
		grunt.spriteStep = 0;
}

// ============================================================================

void DrawSelections()
{
	if (player.selectedUnit)
	{
		const SpriteDesc& spriteDesc =
			unitTypeStateToSpriteDesc[player.selectedUnit->type][player.selectedUnit->state];
		const UnitDesc& unitDesc = unitTypeToUnitDesc[player.selectedUnit->type];

		SDL_Rect src = { player.selectedUnit->pos.x - unitDesc.width / 2,
			player.selectedUnit->pos.y - unitDesc.height / 2, 0, 0 };
		SDL_Rect dst = { src.x + unitDesc.width, src.y + unitDesc.height, 0, 0 };

		RenderSelection(src, dst);
	}

	if (mouse.leftButtonPressed)
	{
		SDL_Rect src = { mouse.lastLeftClickPos.x, mouse.lastLeftClickPos.y, 0, 0 };
		SDL_Rect dst = { mouse.pos.x, mouse.pos.y, 0, 0 };
		RenderSelection(src, dst);
	}
}

// ============================================================================

void Render()
{
	BeginScene();
	{
		RenderRightClick(mouse.lastRightClickPos);
		Render(grunt);
		Render(peon);

		DrawSelections();
	}
	EndScene();
}

// ============================================================================

void Run()
{
	SDL_Event event;

	while (!isDone)
	{
		SDL_PollEvent(&event);
		EventHandler(event);

		lastTime = curTime;
		curTime = SDL_GetTicks();
		Uint32 elapsedTime = curTime - lastTime;

		//DirectionFromKeys();

		Update(grunt, curTime, elapsedTime);
		Update(peon, curTime, elapsedTime);
		Render();		

		SDL_Delay(1);
	}
}

// ============================================================================

int main(int argc, char* argv[])
{
	if (Init())
	{
		Run();
		Quit();
	}

	return 0;
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================
