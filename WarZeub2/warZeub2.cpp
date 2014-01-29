#include "userInput.h"
#include "hud.h"
#include "graphic/spriteDesc.h"
#include "graphic/renderer.h"
#include "gameplay/unitDesc.h"
#include "gameplay/unit.h"
#include "gameplay/player.h"
#include "gameplay/world.h"


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

	HUD::Inst(); // force initialization
	World::Inst(); // force initialization

	player.selectedUnit = 0;

	return true;
}

// ============================================================================

void Quit()
{
	ReleaseSpriteDesc();
	ReleaseRenderer();

	HUD::Kill();
	World::Kill();

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

	if (keyboard.keysPressed[SDLK_ESCAPE])
		isDone = true;

	KeyboardScrollingHandler();
}

// ============================================================================

// TODO: Move it somewhere else
void DrawSelections()
{
	if (player.selectedUnit)
	{
		const SpriteDesc& spriteDesc =
			unitTypeStateToSpriteDesc[player.selectedUnit->Type()][player.selectedUnit->MoveState()];
		const UnitDesc& unitDesc = unitTypeToUnitDesc[player.selectedUnit->Type()];

		SDL_Rect src = { player.selectedUnit->Pos().x - unitDesc.width / 2,
			player.selectedUnit->Pos().y - unitDesc.height / 2, 0, 0 };
		SDL_Rect dst = { src.x + unitDesc.width, src.y + unitDesc.height, 0, 0 };

		TransformToScreenCoordinate(src, gCamera->Pos());
		TransformToScreenCoordinate(dst, gCamera->Pos());

		RenderSquare(src, dst, 0x0000ff00);
	}

	if (mouse.leftButtonPressed && (mouse.lastLeftClickPos.x > (screen->w / 5)))
	{
		// sexy stuff ;)
		Vec2 lastPos(mouse.lastLeftClickPos);
		TransformToWorldCoordinate(lastPos, gCamera->LastPosOnLeftClick());
		TransformToScreenCoordinate(lastPos, gCamera->Pos());

		SDL_Rect src = { lastPos.x, lastPos.y, 0, 0 };
		SDL_Rect dst = { mouse.pos.x, mouse.pos.y, 0, 0 };
		RenderSquare(src, dst, 0x0000ff00);
	}
}

// ============================================================================

void Render()
{
	BeginScene();
	{
		Render(World::Inst()->GetMap());

		const std::vector<Unit*>& units = World::Inst()->Units();
		for (size_t unit = 0; unit < units.size(); ++unit)
			Render(*units[unit]);

		DrawSelections();
		HUD::Inst()->Render();
	}
	EndScene();
}

// ============================================================================

void Run()
{
	SDL_Event event;
	int nbFrames = 0;
	Uint32 lastFrameCountTime = 0;

	while (!isDone)
	{
		SDL_PollEvent(&event);
		EventHandler(event);

		lastTime = curTime;
		curTime = SDL_GetTicks();
		Uint32 elapsedTime = curTime - lastTime;

		World::Inst()->Update(curTime, elapsedTime);

		gCamera->Update(lastTime, curTime);
		Render();		

		char* error = SDL_GetError();
		if (error && strlen(error) > 0)
			fprintf(stderr, "%s\n", error);

		// fps counter
		nbFrames++;
		if (curTime - lastFrameCountTime > 1000)
		{
			char winTitle[128];
			sprintf_s(winTitle, sizeof(winTitle), "WarZeub2! (fps: %d)", nbFrames);
			SDL_WM_SetCaption(winTitle, 0);
			nbFrames = 0;
			lastFrameCountTime = curTime;
		}
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
