#include <SDL.h>


int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface* screen = SDL_SetVideoMode(1024, 768, 32, SDL_HWSURFACE);

	SDL_Flip(screen);

	SDL_Delay(2000);

	SDL_FreeSurface(screen);
	SDL_Quit();

	return 0;
}
