#include "Renderer.h"
#include "AnimDesc.h"
#include <SDL.h>

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

SDL_Surface* screen = 0;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitRenderer()
{
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
#ifdef _DEBUG
		32, SDL_HWSURFACE);
#else
		32, SDL_HWSURFACE | SDL_FULLSCREEN);
#endif
}

// ============================================================================

void ReleaseRenderer()
{
	SDL_FreeSurface(screen);
}

// ============================================================================

void BeginScene()
{
	SDL_FillRect(screen, 0, 0);
}

// ============================================================================

void EndScene()
{
	SDL_Flip(screen);
}

// ============================================================================

int SpriteXOffsetFromDir(const Unit& parUnit)
{
	const AnimDesc& animDesc = unitTypeStateToAnimation[parUnit.type][parUnit.state];

	switch (parUnit.dir)
	{
	case DIR_N:		return animDesc.width * 0;
	case DIR_NE:	return animDesc.width * 1;
	case DIR_E:		return animDesc.width * 2;
	case DIR_SE:	return animDesc.width * 3;
	case DIR_S:		return animDesc.width * 4;

	case DIR_SW:	return animDesc.width * 1; // FIXME: symetry (Moon Walk Style ATM!)
	case DIR_W:		return animDesc.width * 2;
	case DIR_NW:	return animDesc.width * 3;
	}

	return 0;
}

// ============================================================================

void Render(const Unit& parUnit)
{
	const AnimDesc& animDesc = unitTypeStateToAnimation[parUnit.type][parUnit.state];

	int curStep = (parUnit.spriteStep % animDesc.maxStep);
	int spriteY = curStep * animDesc.height + animDesc.offsetY;
	int spriteX = (parUnit.state != EUS_DEAD ? SpriteXOffsetFromDir(parUnit) : 0) + animDesc.offsetX; // special case for dead
	SDL_Rect srcRect = { spriteX, spriteY, animDesc.width, animDesc.height };
	SDL_Rect dstRect = { parUnit.posX - animDesc.width / 2, parUnit.posY - animDesc.height / 2, 0, 0 };

	SDL_BlitSurface(unitTypeToImage[parUnit.type], &srcRect, screen, &dstRect);
}

// ============================================================================

void RenderRightClick(int parX, int parY)
{
	SDL_Rect rect = {parX - 5, parY - 5, 10, 10};
	//SDL_Color color = {0, 255, 0};
	SDL_FillRect(screen, &rect, 0x00ff0000);	
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================