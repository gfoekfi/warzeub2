#include <SDL.h>
#include <SDL_image.h>


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int SPRITE_WIDTH = 76;
const int SPRITE_HEIGHT = 54;

bool isDone = false;
SDL_Surface* screen = 0;
SDL_Surface* img = 0;
Uint32 curTime = 0, lastTime = 0;
int spriteStep = 0;
bool keys[4] = { false, false, false, false }; // up, down, left, right

enum EDir
{
	DIR_N = 0,
	DIR_NE,
	DIR_E,
	DIR_SE,
	DIR_S,
	DIR_SW,
	DIR_W,
	DIR_NW
} spriteDir;

// XXX) Afficher une image
// XXX) Afficher une partie de l'image
// XXX) Animer une sequence sprite (avec timer)
// XXX) Coordonner les fleches clavier avec la sprite
// 5) Coder ca proprement


// TODO: Error handling
bool Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_WM_SetCaption("WarZeub 2 !", 0);
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
#ifdef _DEBUG
		32, SDL_HWSURFACE);
#else
		32, SDL_HWSURFACE | SDL_FULLSCREEN);
#endif

	img = IMG_Load("../Data/grunt.png");
	if (!img)
		return false;

	return true;
}

void Quit()
{
	SDL_FreeSurface(img);
	SDL_FreeSurface(screen);
	SDL_Quit();
}

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

	return -1;
}

void EventHandler(const SDL_Event& parEvent)
{
	switch (parEvent.type)
	{
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

void DirectionFromKeys()
{
	if (keys[0])
	{
		if (keys[2])
			spriteDir = DIR_NW;
		else if (keys[3])
			spriteDir = DIR_NE;
		else
			spriteDir = DIR_N;
	}
	else if (keys[1])
	{
		if (keys[2])
			spriteDir = DIR_SW;
		else if (keys[3])
			spriteDir = DIR_SE;
		else
			spriteDir = DIR_S;
	}
	else if (keys[2])
		spriteDir = DIR_W;
	else if (keys[3])
		spriteDir = DIR_E;
	else
	{
		spriteStep = 0; // we force the animation to stop
	}
}

int SpriteXOffsetFromDir()
{
	switch (spriteDir)
	{
	case DIR_N:		return SPRITE_WIDTH * 0;
	case DIR_NE:	return SPRITE_WIDTH * 1;
	case DIR_E:		return SPRITE_WIDTH * 2;
	case DIR_SE:	return SPRITE_WIDTH * 3;
	case DIR_S:		return SPRITE_WIDTH * 4;

	case DIR_SW:	return SPRITE_WIDTH * 1; // FIXME: symetry (Moon Walk Style !)
	case DIR_W:		return SPRITE_WIDTH * 2;
	case DIR_NW:	return SPRITE_WIDTH * 3;
	}

	return 0;
}

void Render()
{
	SDL_FillRect(screen, 0, 0);

	const int spriteHeight = 54;
	int spriteY = (spriteStep % 5) * spriteHeight; // FIXME: Bug in the animation?
	int spriteX = SpriteXOffsetFromDir();
	SDL_Rect srcRect = { spriteX, spriteY, 76, spriteHeight };
	SDL_Rect dstRect = { SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0, 0 };

	SDL_BlitSurface(img, &srcRect, screen, &dstRect);

	SDL_Flip(screen);
}

void Run()
{
	SDL_Event event;

	while (!isDone)
	{
		curTime = SDL_GetTicks();
		Uint32 deltaTime = curTime - lastTime;
		if (deltaTime > 170)
		{
			spriteStep++;
			lastTime = curTime;
		}

		SDL_PollEvent(&event);
		EventHandler(event);

		DirectionFromKeys();

		Render();		
	}
}

int main(int argc, char* argv[])
{
	if (Init())
	{
		Run();
		Quit();
	}

	return 0;
}
