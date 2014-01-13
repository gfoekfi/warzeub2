#include <SDL.h>

SDL_Surface* screen = 0;
bool isDone = false;


// TODO: Error handling
bool Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_WM_SetCaption("WarZeub 2 !", 0);
	screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);

	return true;
}

void Quit()
{
	SDL_FreeSurface(screen);
	SDL_Quit();
}

void EventHandler(const SDL_Event& parEvent)
{
	switch (parEvent.type)
	{
	case SDL_KEYDOWN:
	case SDL_QUIT:
		isDone = true;
		break;

	default:
		break;
	}
}

void Render()
{
	SDL_Flip(screen);
}

void Run()
{
	SDL_Event event;

	while (!isDone)
	{
		SDL_PollEvent(&event);
		EventHandler(event);
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
