#include "renderer.h"
#include "spriteDesc.h"
#include "font.h"
#include "../gameplay/unitDesc.h"
#include "../gameplay/player.h"
#include "../orders/order.h"
#include "../hud.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <assert.h>
#include <map>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

#ifdef _DEBUG

//#define RENDER_BUILDTILE // Warning: walk or build tile, not both
//#define RENDER_WALKTILE

#endif

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

const SDL_Rect viewport = {SCREEN_WIDTH / 5, 0, 4 * SCREEN_WIDTH / 5, SCREEN_HEIGHT };
Camera* gCamera;
SDL_Surface* screen = 0;
SDL_Surface* summerTilesSurface = 0;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitRenderer()
{
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
#ifdef _DEBUG
		32, SDL_HWSURFACE | SDL_DOUBLEBUF);
#else
		//32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
		32, SDL_HWSURFACE | SDL_DOUBLEBUF);
#endif

	summerTilesSurface = IMG_Load("../Data/summer_tiles.png");
	gCamera = new Camera();
	gCamera->SetPos(float2(100.f, 100.f));

	bool initResult = FontInit();
	assert(initResult);
}

// ============================================================================

void ReleaseRenderer()
{
	if (gCamera)
		delete gCamera;
	SDL_FreeSurface(screen);

	FontQuit();
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

int OffsetXFromDirection(EDir parDir)
{
	switch (parDir)
	{
	case DIR_N:		return 0;
	case DIR_NE:	return 1;
	case DIR_E:		return 2;
	case DIR_SE:	return 3;
	case DIR_S:		return 4;

	case DIR_SW:	return 1; // FIXME: symetry from sprite (Moon Walk Style ATM!)
	case DIR_W:		return 2;
	case DIR_NW:	return 3;
	}

	return 0;
}

// ============================================================================

void Render(SDL_Surface* parSrcSurface,
				const SpriteDesc& parSpriteDesc,
				const float2& parWorldPos,
				EDir parDir,
				int parSpriteStep)
{
	// TODO: Visibility check

	SDL_Rect srcRect =
	{
		parSpriteDesc.offsetX + OffsetXFromDirection(parDir) * parSpriteDesc.width,
		parSpriteDesc.offsetY + (parSpriteStep % parSpriteDesc.maxStep) * parSpriteDesc.height,
		parSpriteDesc.width,
		parSpriteDesc.height
	};

	float2 screenPos(parWorldPos);
	TransformToScreenCoordinate(screenPos, gCamera->Pos());

	SDL_Rect dstRect =
	{
		Sint16(screenPos.x) - parSpriteDesc.width / 2,
		Sint16(screenPos.y) - parSpriteDesc.height / 2,
		0,
		0
	};

	SDL_BlitSurface(parSrcSurface, &srcRect, screen, &dstRect);
}

// ============================================================================

void Render(EUnitType parUnitType, const float2& parScreenPos)
{
	const SpriteDesc& spriteDesc = unitTypeStateToSpriteDesc[parUnitType][EUS_IDLE];

	SDL_Rect srcRect = { spriteDesc.offsetX, spriteDesc.offsetY, spriteDesc.width, spriteDesc.height };
	SDL_Rect dstRect = { Sint16(parScreenPos.x) - spriteDesc.width / 2,
		Sint16(parScreenPos.y) - spriteDesc.height / 2, 0, 0 };

	SDL_BlitSurface(unitTypeToImage[parUnitType], &srcRect, screen, &dstRect);
}

// ============================================================================

// TODO: Split me
void Render(const World& parWorld)
{
	assert(summerTilesSurface);

	static SDL_Surface* mapSurface = 0;
#ifdef RENDER_BUILDTILE
	static SDL_Surface* buildTileSurface = 0;
#endif
#ifdef RENDER_WALKTILE
	static SDL_Surface* walkTileSurface = 0;
#endif
	if (!mapSurface)
	{
		mapSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, parWorld.Width() * BUILD_TILE_SIZE,
			parWorld.Height() * BUILD_TILE_SIZE, screen->format->BitsPerPixel,
			screen->format->Rmask, screen->format->Gmask,
			screen->format->Bmask, screen->format->Amask);
#ifdef RENDER_BUILDTILE
		buildTileSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, parWorld.Width() * BUILD_TILE_SIZE,
			parWorld.Height() * BUILD_TILE_SIZE, screen->format->BitsPerPixel,
			screen->format->Rmask, screen->format->Gmask,
			screen->format->Bmask, screen->format->Amask);
#endif

#ifdef RENDER_WALKTILE
		walkTileSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, parWorld.Width() * BUILD_TILE_SIZE,
			parWorld.Height() * BUILD_TILE_SIZE, screen->format->BitsPerPixel,
			screen->format->Rmask, screen->format->Gmask,
			screen->format->Bmask, screen->format->Amask);
#endif

		SDL_Rect src = { (BUILD_TILE_SIZE + 1) * 14, (BUILD_TILE_SIZE + 1) * 18, BUILD_TILE_SIZE, BUILD_TILE_SIZE }; // (14, 18) = grass
		SDL_Rect dst = { 0, 0, 0, 0 };

		for (size_t x = 0; x < parWorld.Width(); ++x)
		{
			for (size_t y = 0; y < parWorld.Height(); ++y)
			{
				dst.x = x * BUILD_TILE_SIZE;
				dst.y = y * BUILD_TILE_SIZE;
				SDL_BlitSurface(summerTilesSurface, &src, mapSurface, &dst);
#ifdef RENDER_BUILDTILE
				SDL_Rect buildTileRect = { x * BUILD_TILE_SIZE, y * BUILD_TILE_SIZE, BUILD_TILE_SIZE, BUILD_TILE_SIZE};
				SDL_FillRect(buildTileSurface, &buildTileRect, ((x + y) % 2) ? 0x000000ff : 0x00ffffff);
#endif

#ifdef RENDER_WALKTILE
				for (size_t x2 = 0; x2 < (BUILD_TILE_SIZE / WALK_TILE_SIZE); ++x2)
					for (size_t y2 = 0; y2 < (BUILD_TILE_SIZE / WALK_TILE_SIZE); ++y2)
					{
						SDL_Rect walkTileRect = {
							x * BUILD_TILE_SIZE + x2 * WALK_TILE_SIZE,
							y * BUILD_TILE_SIZE + y2 * WALK_TILE_SIZE,
							WALK_TILE_SIZE,
							WALK_TILE_SIZE
						};
						SDL_FillRect(walkTileSurface, &walkTileRect, ((x2 + y2) % 2) ? 0x0000ffff : 0x00ffffff);
					}
#endif
			}
		}
	}

	if (mapSurface)
	{
		SDL_Rect dst = { 0, 0, 0, 0 };
		TransformToScreenCoordinate(dst, gCamera->Pos());

#ifdef RENDER_BUILDTILE
		SDL_BlitSurface(buildTileSurface, 0, screen, &dst);
#else
#ifdef RENDER_WALKTILE
		SDL_BlitSurface(walkTileSurface, 0, screen, &dst);
#else
		SDL_BlitSurface(mapSurface, 0, screen, &dst);
#endif
#endif
	}
}

// ============================================================================

void RenderSquare(SDL_Rect& parSrc, SDL_Rect& parDst, Uint32 parColor)
{
	// FIXME: Square might need to be truncated (ex: selection square)
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