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
	const SpriteDesc& spriteDesc = unitTypeStateToSpriteDesc[parUnit.Type()][parUnit.MoveState()];

	int curStep = (parUnit.SpriteStep() % spriteDesc.maxStep);
	int spriteY = curStep * spriteDesc.height + spriteDesc.offsetY;
	int spriteX = (parUnit.MoveState() != EUS_DEAD ? SpriteXOffsetFromDir(parUnit) : 0) + spriteDesc.offsetX; // special case for dead
	SDL_Rect srcRect = { spriteX, spriteY, spriteDesc.width, spriteDesc.height };
	SDL_Rect dstRect = { parUnit.Pos().x - spriteDesc.width / 2, parUnit.Pos().y - spriteDesc.height / 2, 0, 0 };

	SDL_BlitSurface(unitTypeToImage[parUnit.Type()], &srcRect, screen, &dstRect);
}

// ============================================================================

SDL_Surface* GenerateHudBackgroundSurface()
{
	SDL_Surface* backgroundSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w / 5,
		screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, 
		screen->format->Bmask, screen->format->Amask);

	SDL_Rect src = { (MAP_TILE_SIZE + 1) * 11, (MAP_TILE_SIZE + 1) * 17, MAP_TILE_SIZE, MAP_TILE_SIZE }; // (11, 17) = mud
	SDL_Rect dst = { 0, 0, 0, 0 };

	for (size_t line = 0; line < (1 + screen->h / MAP_TILE_SIZE); ++line)
	{
		for (size_t col = 0; col < (1 + backgroundSurface->w / MAP_TILE_SIZE); ++col)
		{
			dst.x = col * MAP_TILE_SIZE;
			dst.y = line * MAP_TILE_SIZE;
			SDL_BlitSurface(summerTilesSurface, &src, backgroundSurface, &dst);
		}
	}

	return backgroundSurface;
}

// ============================================================================

void RenderHUDOrder(SDL_Surface* parIconSurface, EOrder parOrder, const Vec2& parOffset)
{
	if (parOrder == EO_NONE)
		return;

	const SpriteDesc& orderIconSpriteDesc = orderToIconSpriteDesc[parOrder];
	SDL_Rect orderIconSrc = { orderIconSpriteDesc.offsetX, orderIconSpriteDesc.offsetY,
		orderIconSpriteDesc.width, orderIconSpriteDesc.height };

	int gridPos = HUD::Inst()->OrderToGridPos()[parOrder];
	SDL_Rect orderIconDst = { parOffset.x + (gridPos % 3) * (orderIconSpriteDesc.width + 5),
		parOffset.y + (gridPos / 3) * (orderIconSpriteDesc.height + 5), 0, 0};
	SDL_BlitSurface(parIconSurface, &orderIconSrc, screen, &orderIconDst);
}

// ============================================================================

void RenderHUD()
{
	assert(summerTilesSurface);

	static SDL_Surface* backgroundSurface = GenerateHudBackgroundSurface();
	assert(backgroundSurface);
	SDL_BlitSurface(backgroundSurface, 0, screen, 0);

	static SDL_Surface* iconsSurface = IMG_Load("../Data/orc_icons.png");
	assert(iconsSurface);

	int miniMapOffsetX = backgroundSurface->w / 20;
	int miniMapOffsetY = (backgroundSurface->h / 3 ) / 20;
	static SDL_Rect miniMapRect = {miniMapOffsetX, miniMapOffsetY, 
		backgroundSurface->w - (2 * miniMapOffsetX), 
		(backgroundSurface->h / 3) - (2 * miniMapOffsetY)};
	SDL_FillRect(screen, &miniMapRect, 0);

	if (player.selectedUnit)
	{
		// Selection info
		int selectionInfoOffsetX = backgroundSurface->w / 50;
		int selectionInfoOffsetY = (backgroundSurface->h / 3) / 50;
		SDL_Rect borderSrc = { selectionInfoOffsetX,
			selectionInfoOffsetY + (backgroundSurface->h / 3), 0, 0 };
		SDL_Rect borderDst = { backgroundSurface->w - (2 * selectionInfoOffsetX),
			(2*backgroundSurface->h / 3) - selectionInfoOffsetY, 0, 0 };
		RenderSelection(borderSrc, borderDst, 0x00ffffff);

		const SpriteDesc& unitIconSpriteDesc = unitTypeToIconSpriteDesc[player.selectedUnit->Type()];
		SDL_Rect src = { unitIconSpriteDesc.offsetX, unitIconSpriteDesc.offsetY, 
			unitIconSpriteDesc.width, unitIconSpriteDesc.height };
		SDL_Rect dst = { borderSrc.x + 1, borderSrc.y + 1, 0, 0 };
		SDL_BlitSurface(iconsSurface, &src, screen, &dst);

		Vec2 orderHudOffset(selectionInfoOffsetX,
			(2*backgroundSurface->h / 3) + selectionInfoOffsetY);
		const std::set<EOrder>& unitOrders =
			unitTypeToUnitDesc[player.selectedUnit->Type()].unitStateToOrderSet[player.selectedUnit->ActionState()];
		for (std::set<EOrder>::const_iterator order = unitOrders.begin();
			order != unitOrders.end(); ++order)
		{
			RenderHUDOrder(iconsSurface, *order, orderHudOffset);
		}
	}
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
		SDL_BlitSurface(mapSurface, 0, screen, 0);
}

// ============================================================================

void RenderRightClick(const Vec2& parPos)
{
	SDL_Rect rect = {parPos.x - 5, parPos.y - 5, 10, 10};
	SDL_FillRect(screen, &rect, 0x00ff0000);	
}

// ============================================================================

void RenderSelection(SDL_Rect& parSrc, SDL_Rect& parDst, Uint32 parColor)
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
// ----------------------------------------------------------------------------
// ============================================================================