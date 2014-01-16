#include "Renderer.h"
#include "SpriteDesc.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <assert.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

SDL_Surface* screen = 0;
SDL_Surface* summerTilesSurface = 0;

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

	summerTilesSurface = IMG_Load("../Data/summer_tiles.png");
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
	const SpriteDesc& spriteDesc = unitTypeStateToSpriteDesc[parUnit.type][parUnit.state];

	switch (parUnit.dir)
	{
	case DIR_N:		return spriteDesc.width * 0;
	case DIR_NE:	return spriteDesc.width * 1;
	case DIR_E:		return spriteDesc.width * 2;
	case DIR_SE:	return spriteDesc.width * 3;
	case DIR_S:		return spriteDesc.width * 4;

	case DIR_SW:	return spriteDesc.width * 1; // FIXME: symetry from sprite (Moon Walk Style ATM!)
	case DIR_W:		return spriteDesc.width * 2;
	case DIR_NW:	return spriteDesc.width * 3;
	}

	return 0;
}

// ============================================================================

void Render(const Unit& parUnit)
{
	const SpriteDesc& spriteDesc = unitTypeStateToSpriteDesc[parUnit.type][parUnit.state];

	int curStep = (parUnit.spriteStep % spriteDesc.maxStep);
	int spriteY = curStep * spriteDesc.height + spriteDesc.offsetY;
	int spriteX = (parUnit.state != EUS_DEAD ? SpriteXOffsetFromDir(parUnit) : 0) + spriteDesc.offsetX; // special case for dead
	SDL_Rect srcRect = { spriteX, spriteY, spriteDesc.width, spriteDesc.height };
	SDL_Rect dstRect = { parUnit.pos.x - spriteDesc.width / 2, parUnit.pos.y - spriteDesc.height / 2, 0, 0 };

	SDL_BlitSurface(unitTypeToImage[parUnit.type], &srcRect, screen, &dstRect);
}

// ============================================================================

void RenderHUD()
{
	assert(summerTilesSurface);

	static SDL_Surface* backgroundSurface = 0;
	if (!backgroundSurface)
	{
		backgroundSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w / 4, screen->h, 
			screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, 
			screen->format->Bmask, screen->format->Amask);

		SDL_Rect src = { MAP_TILE_SIZE * 11, MAP_TILE_SIZE * 17, MAP_TILE_SIZE, MAP_TILE_SIZE }; // (11, 17) = mud
		SDL_Rect dst = { 0, 0, 0, 0 };

		for (size_t line = 0; line < (1 + screen->h / MAP_TILE_SIZE); ++line)
		{
			for (size_t col = 0; col < (1 + backgroundSurface->w / MAP_TILE_SIZE); ++col)
			{
				dst.x = col * (MAP_TILE_SIZE - 1);
				dst.y = line * (MAP_TILE_SIZE - 1);
				SDL_BlitSurface(summerTilesSurface, &src, backgroundSurface, &dst);
			}
		}
	}

	if (!backgroundSurface)
		return;

	SDL_BlitSurface(backgroundSurface, 0, screen, 0);
}

// ============================================================================

void Render(const Map& parMap)
{
	assert(summerTilesSurface);

	static SDL_Surface* mapSurface = 0;
	if (!mapSurface)
	{
		mapSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h, 
			screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, 
			screen->format->Bmask, screen->format->Amask);

		SDL_Rect src = { MAP_TILE_SIZE * 14, MAP_TILE_SIZE * 18, MAP_TILE_SIZE, MAP_TILE_SIZE }; // (14, 18) = grass
		SDL_Rect dst = { 0, 0, 0, 0 };

		for (int x = 0; x < parMap.width; ++x)
		{
			for (int y = 0; y < parMap.height; ++y)
			{
				dst.x = x * (MAP_TILE_SIZE - 1);
				dst.y = y * (MAP_TILE_SIZE - 1);
				SDL_BlitSurface(summerTilesSurface, &src, mapSurface, &dst);
			}
		}
	}

	if (mapSurface)
		SDL_BlitSurface(mapSurface, 0, screen, 0);
}

// ============================================================================

void RenderRightClick(const Vec2& parPos)
{
	SDL_Rect rect = {parPos.x - 5, parPos.y - 5, 10, 10};
	SDL_FillRect(screen, &rect, 0x00ff0000);	
}

// ============================================================================

void RenderSelection(SDL_Rect& parSrc, SDL_Rect& parDst)
{
	int width = abs(parDst.x - parSrc.x);
	int height = abs(parDst.y - parSrc.y);

	SDL_Rect bar =
	{ 
		(parDst.x < parSrc.x) ? parDst.x : parSrc.x,
		(parDst.y < parSrc.y) ? parDst.y : parSrc.y, 
		width, 
		1
	};

	SDL_FillRect(screen, &bar, 0x0000ff00);
	bar.y += height;
	SDL_FillRect(screen, &bar, 0x0000ff00);
	bar.y -= height;
	bar.w = 1;
	bar.h = height;
	SDL_FillRect(screen, &bar, 0x0000ff00);
	bar.x += width;
	SDL_FillRect(screen, &bar, 0x0000ff00);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================