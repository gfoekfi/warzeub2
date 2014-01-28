#include "renderer.h"
#include "unitDesc.h"
#include "spriteDesc.h"
#include "player.h"
#include "order.h"
#include "hud.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <assert.h>
#include <map>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

const SDL_Rect viewport = {SCREEN_WIDTH / 5, 0, 4 * SCREEN_WIDTH / 5, SCREEN_HEIGHT };
Vec2 cameraPos(0, 0);
SDL_Surface* screen = 0;
SDL_Surface* summerTilesSurface = 0;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitRenderer()
{
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
#if 1
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
	assert(parUnit.IsMovable());
	const SpriteDesc& spriteDesc = unitTypeStateToSpriteDesc[parUnit.Type()][parUnit.MoveState()];

	switch (parUnit.Dir())
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
	if (parUnit.ActionState() == EUS_BUILDING) // don't render unit that are building
		return;

	const SpriteDesc& spriteDesc = unitTypeStateToSpriteDesc[parUnit.Type()][parUnit.MoveState()];

	int curStep = (parUnit.SpriteStep() % spriteDesc.maxStep);
	int spriteY = curStep * spriteDesc.height + spriteDesc.offsetY;
	int spriteX = spriteDesc.offsetX;
	if (parUnit.IsMovable() && (parUnit.MoveState() != EUS_DEAD))
		spriteX += SpriteXOffsetFromDir(parUnit);
	SDL_Rect srcRect = { spriteX, spriteY, spriteDesc.width, spriteDesc.height };

	SDL_Rect dstRect = { parUnit.Pos().x - spriteDesc.width / 2, parUnit.Pos().y - spriteDesc.height / 2, 0, 0 };
	TransformToScreenCoordinate(dstRect, cameraPos);

	if ((parUnit.ActionState() == EUS_BEING_BUILD_STATE0) || (parUnit.ActionState() == EUS_BEING_BUILD_STATE1))
		SDL_BlitSurface(unitTypeToImage[EUT_MINE], &srcRect, screen, &dstRect);
	else
		SDL_BlitSurface(unitTypeToImage[parUnit.Type()], &srcRect, screen, &dstRect);
}

// ============================================================================

void Render(EUnitType parUnitType, const Vec2& parScreenPos)
{
	const SpriteDesc& spriteDesc = unitTypeStateToSpriteDesc[parUnitType][EUS_IDLE];

	SDL_Rect srcRect = { spriteDesc.offsetX, spriteDesc.offsetY, spriteDesc.width, spriteDesc.height };
	SDL_Rect dstRect = { parScreenPos.x - spriteDesc.width / 2, parScreenPos.y - spriteDesc.height / 2, 0, 0 };

	SDL_BlitSurface(unitTypeToImage[parUnitType], &srcRect, screen, &dstRect);
}

// ============================================================================

void Render(const Map& parMap)
{
	assert(summerTilesSurface);

	static SDL_Surface* mapSurface = 0;
	if (!mapSurface)
	{
		mapSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, parMap.width * MAP_TILE_SIZE,
			parMap.height * MAP_TILE_SIZE, screen->format->BitsPerPixel,
			screen->format->Rmask, screen->format->Gmask,
			screen->format->Bmask, screen->format->Amask);

		SDL_Rect src = { (MAP_TILE_SIZE + 1) * 14, (MAP_TILE_SIZE + 1) * 18, MAP_TILE_SIZE, MAP_TILE_SIZE }; // (14, 18) = grass
		SDL_Rect dst = { 0, 0, 0, 0 };

		for (int x = 0; x < parMap.width; ++x)
		{
			for (int y = 0; y < parMap.height; ++y)
			{
				dst.x = x * MAP_TILE_SIZE;
				dst.y = y * MAP_TILE_SIZE;
				SDL_BlitSurface(summerTilesSurface, &src, mapSurface, &dst);
			}
		}
	}

	if (mapSurface)
	{
		SDL_Rect dst = { 0, 0, 0, 0 };
		TransformToScreenCoordinate(dst, cameraPos);
		SDL_BlitSurface(mapSurface, 0, screen, &dst);
	}
}

// ============================================================================

void RenderSquare(SDL_Rect& parSrc, SDL_Rect& parDst, Uint32 parColor)
{
	parSrc.x = Clamp<int>(parSrc.x, 0, SCREEN_WIDTH - 1);
	parSrc.y = Clamp<int>(parSrc.y, 0, SCREEN_HEIGHT - 1);
	parDst.x = Clamp<int>(parDst.x, 0, SCREEN_WIDTH - 1);
	parDst.y = Clamp<int>(parDst.y, 0, SCREEN_HEIGHT - 1);

	int width = abs(parDst.x - parSrc.x);
	int height = abs(parDst.y - parSrc.y);

	SDL_Rect bar =
	{ 
		(parDst.x < parSrc.x) ? parDst.x : parSrc.x,
		(parDst.y < parSrc.y) ? parDst.y : parSrc.y, 
		width, 
		1
	};

	SDL_FillRect(screen, &bar, parColor);
	bar.y += height;
	SDL_FillRect(screen, &bar, parColor);
	bar.y -= height;
	bar.w = 1;
	bar.h = height;
	SDL_FillRect(screen, &bar, parColor);
	bar.x += width;
	SDL_FillRect(screen, &bar, parColor);
}

// ============================================================================

void RenderProgressBar(SDL_Rect& parDimensions, float parStatus)
{
	assert(parStatus >= 0.0f && parStatus <= 1.0f);

	Uint32 greyColor = 0x00999999;
	SDL_Rect dst = { parDimensions.x + parDimensions.w, parDimensions.y + parDimensions.h, 0, 0 };
	RenderSquare(parDimensions, dst, greyColor);

	parDimensions.y += 2;
	parDimensions.h -= 3;
	parDimensions.x += 2;
	parDimensions.w -= 3;
	parDimensions.w = Uint16(parDimensions.w * parStatus);
	SDL_FillRect(screen, &parDimensions, greyColor);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================