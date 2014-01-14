//#include <SDL.h>
//#include <SDL_image.h>
//#include <map>
//#include <utility>

#include "Unit.h"
#include "AnimDesc.h"
#include "Renderer.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

bool isDone = false;
bool keys[5] = { false, false, false, false, false }; // up, down, left, right, space
int lastClickX = SCREEN_WIDTH / 2;
int lastClickY = SCREEN_HEIGHT / 2;
Uint32 lastTime = 0;
Uint32 curTime = 0;

Unit grunt = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, DIR_N, EUT_GRUNT, EUS_IDLE, 0, 0 };

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

bool Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_WM_SetCaption("WarZeub 2 !", 0);
	
	InitRenderer();
	InitAnimDesc();

	return true;
}

// ============================================================================

void Quit()
{
	ReleaseAnimDesc();
	ReleaseRenderer();

	SDL_Quit();
}

// ============================================================================

int KeyOffsetFromKey(SDLKey parKey)
{
	if (parKey == SDLK_UP)
		return 0;
	if (parKey == SDLK_DOWN)
		return 1;
	if (parKey == SDLK_LEFT)
		return 2;
	if (parKey == SDLK_RIGHT)
		return 3;
	if (parKey == SDLK_SPACE)
		return 4;

	return -1;
}

// ============================================================================

void EventHandler(const SDL_Event& parEvent)
{
	switch (parEvent.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		{
			if (parEvent.button.button == SDL_BUTTON_RIGHT)
			{
				lastClickX = parEvent.button.x;
				lastClickY = parEvent.button.y;
			}
		}
		break;
	case SDL_KEYDOWN:
		{
			const SDLKey& key = parEvent.key.keysym.sym;
			if (key == SDLK_ESCAPE)
				isDone = true;
			else
			{
				int curKey = KeyOffsetFromKey(key);
				if (curKey >= 0)
					keys[curKey] = true;
			}
		}
		break;

	case SDL_KEYUP:
		{
			const SDLKey& key = parEvent.key.keysym.sym;
			int curKey = KeyOffsetFromKey(key);
			if (curKey >= 0)
				keys[curKey] = false;
		}
		break;

	case SDL_QUIT:
		isDone = true;
		break;

	default:
		break;
	}
}

// ============================================================================

void DirectionFromKeys()
{
	EUnitState prevState = grunt.state;
	//grunt.state = keys[4] ? EUS_DEAD : EUS_MOVE;
	grunt.state = keys[4] ? EUS_ATTACK : EUS_MOVE;

	if (keys[0])
	{
		if (keys[2])
			grunt.dir = DIR_NW;
		else if (keys[3])
			grunt.dir = DIR_NE;
		else
			grunt.dir = DIR_N;
	}
	else if (keys[1])
	{
		if (keys[2])
			grunt.dir = DIR_SW;
		else if (keys[3])
			grunt.dir = DIR_SE;
		else
			grunt.dir = DIR_S;
	}
	else if (keys[2])
		grunt.dir = DIR_W;
	else if (keys[3])
		grunt.dir = DIR_E;
	else
	{
		grunt.state = EUS_IDLE;
	}

	if (grunt.state != prevState)
		grunt.spriteStep = 0;
}

// ============================================================================

void Render()
{
	BeginScene();
	{
		RenderRightClick(lastClickX, lastClickY);
		Render(grunt);
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
		grunt.targetPosX = lastClickX;
		grunt.targetPosY = lastClickY;
		Update(grunt, curTime, elapsedTime);

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
