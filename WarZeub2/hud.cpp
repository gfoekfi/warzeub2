#include "hud.h"
#include "renderer.h"
#include "player.h"
#include "userInput.h"
#include "spriteDesc.h"
#include "util.h"
#include <assert.h>
#include <algorithm>
#include <SDL_Image.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

HUD::HUD() :
	lastOrder_(EO_NONE)
{
	InitOrderGridPosMapping_();

	iconsSurface_ = IMG_Load("../Data/orc_icons.png");
	assert(iconsSurface_);

	GenerateBackgroundSurface_();
}

// ============================================================================

HUD::~HUD()
{
	if (iconsSurface_)
		SDL_FreeSurface(iconsSurface_);
	if (backgroundSurface_)
		SDL_FreeSurface(backgroundSurface_);
}

// ============================================================================

void HUD::GenerateBackgroundSurface_()
{
	backgroundSurface_ = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w / 5,
		screen->h, screen->format->BitsPerPixel, screen->format->Rmask, 
		screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
	assert(backgroundSurface_);

	SDL_Rect src = { (MAP_TILE_SIZE + 1) * 11, (MAP_TILE_SIZE + 1) * 17, MAP_TILE_SIZE, MAP_TILE_SIZE }; // (11, 17) = mud
	SDL_Rect dst = { 0, 0, 0, 0 };

	for (size_t line = 0; line < (1 + screen->h / MAP_TILE_SIZE); ++line)
	{
		for (size_t col = 0; col < (1 + backgroundSurface_->w / MAP_TILE_SIZE); ++col)
		{
			dst.x = col * MAP_TILE_SIZE;
			dst.y = line * MAP_TILE_SIZE;
			SDL_BlitSurface(summerTilesSurface, &src, backgroundSurface_, &dst);
		}
	}
}

// ============================================================================

void HUD::Render()
{
	// Building placement
	if (player.selectedUnit && (player.selectedUnit->ActionState() == EUS_CHOOSE_DESTINATION))
	{
		switch (lastOrder_)
		{
		case EO_BUILD_TOWN_HALL: ::Render(EUT_TOWN_HALL, mouse.pos); break;
		};
	}

	SDL_BlitSurface(backgroundSurface_, 0, screen, 0);

	int miniMapOffsetX = backgroundSurface_->w / 20;
	int miniMapOffsetY = (backgroundSurface_->h / 3 ) / 20;
	static SDL_Rect miniMapRect = {miniMapOffsetX, miniMapOffsetY,
		backgroundSurface_->w - (2 * miniMapOffsetX),
		(backgroundSurface_->h / 3) - (2 * miniMapOffsetY)};
	SDL_FillRect(screen, &miniMapRect, 0);

	if (player.selectedUnit)
	{
		// Selection info
		int selectionInfoOffsetX = backgroundSurface_->w / 50;
		int selectionInfoOffsetY = (backgroundSurface_->h / 3) / 50;
		SDL_Rect borderSrc = { selectionInfoOffsetX,
			selectionInfoOffsetY + (backgroundSurface_->h / 3), 0, 0 };
		SDL_Rect borderDst = { backgroundSurface_->w - (2 * selectionInfoOffsetX),
			(2*backgroundSurface_->h / 3) - selectionInfoOffsetY, 0, 0 };
		RenderSelection(borderSrc, borderDst, 0x00ffffff);

		const SpriteDesc& unitIconSpriteDesc = unitTypeToIconSpriteDesc[player.selectedUnit->Type()];
		SDL_Rect src = { unitIconSpriteDesc.offsetX, unitIconSpriteDesc.offsetY,
			unitIconSpriteDesc.width, unitIconSpriteDesc.height };
		SDL_Rect dst = { borderSrc.x + 1, borderSrc.y + 1, 0, 0 };
		SDL_BlitSurface(iconsSurface_, &src, screen, &dst);

		// Grid
		Vec2 orderHudOffset(selectionInfoOffsetX,
			(2*backgroundSurface_->h / 3) + selectionInfoOffsetY);
		const std::set<EOrder>& unitOrders =
			unitTypeToUnitDesc[player.selectedUnit->Type()].unitStateToOrderSet[player.selectedUnit->ActionState()];
		for (std::set<EOrder>::const_iterator order = unitOrders.begin();
			order != unitOrders.end(); ++order)
		{
			RenderHUDOrder_(*order, orderHudOffset);
		}
	}
}

// ============================================================================

void HUD::RenderHUDOrder_(EOrder parOrder, const Vec2& parGridRegionOffset)
{
	if (parOrder == EO_NONE)
		return;

	const SpriteDesc& orderIconSpriteDesc = orderToIconSpriteDesc[parOrder];
	SDL_Rect orderIconSrc = { orderIconSpriteDesc.offsetX, orderIconSpriteDesc.offsetY,
		orderIconSpriteDesc.width, orderIconSpriteDesc.height };

	int gridPos = orderToGridPos_[parOrder];
	SDL_Rect orderIconDst = { parGridRegionOffset.x + (gridPos % 3) * (orderIconSpriteDesc.width + 5),
		parGridRegionOffset.y + (gridPos / 3) * (orderIconSpriteDesc.height + 5), 0, 0};
	SDL_BlitSurface(iconsSurface_, &orderIconSrc, screen, &orderIconDst);
}

// ============================================================================

void HUD::InitOrderGridPosMapping_()
{
	orderToGridPos_[EO_MOVE] = 0;
	orderToGridPos_[EO_STOP] = 1;
	orderToGridPos_[EO_CANCEL] = 8;
	orderToGridPos_[EO_TRAIN_PEON] = 0;
	orderToGridPos_[EO_BUILD] = 6;
	orderToGridPos_[EO_BUILD_TOWN_HALL] = 0;

	for (std::map<EOrder, int>::iterator it = orderToGridPos_.begin();
			it != orderToGridPos_.end(); ++it)
	{
		gridPosToOrders_[it->second].insert(it->first);
	}
}

// ============================================================================

bool HUD::IsInHUDRegion(const Vec2& parPos) const
{
	return (parPos.x <= (screen->w / 5));
}

// ============================================================================

int HUD::GridClickPositionFromMouse_()
{
	SDL_Rect mouseRect = BoundingBoxFromMouse(mouse);
	const SpriteDesc& orderIconSpriteDesc = orderToIconSpriteDesc[EO_CANCEL];

	for (int line = 0; line < 3; ++line)
	{
		for (int col = 0; col < 3; ++col)
		{
			int iconOffsetX = (screen->w / 5) / 50 + col * (orderIconSpriteDesc.width + 5);
			int iconOffsetY = (2 * screen->h / 3) + ((screen->h / 3) / 50) + line * (orderIconSpriteDesc.height + 5);
			SDL_Rect iconRect = {iconOffsetX, iconOffsetY, orderIconSpriteDesc.width, orderIconSpriteDesc.height};

			if (DoesBBoxesCollide(&mouseRect, &iconRect))
				return (line*3 + col);
		}
	}

	return -1;
}

// ============================================================================

void HUD::ApplyGridClick_(Unit& parUnit, int parGridClickPos)
{
	assert(parGridClickPos >= 0 && parGridClickPos <= 8); // grid is 3x3

	const std::set<EOrder>& ordersOnGridPos = gridPosToOrders_[parGridClickPos];
	UnitDesc& unitDesc = unitTypeToUnitDesc[parUnit.Type()];
	const std::set<EOrder>& ordersFromUnitState =
		unitDesc.unitStateToOrderSet[parUnit.ActionState()];

	std::set<EOrder> intersectSet;
	std::set_intersection(ordersOnGridPos.begin(), ordersOnGridPos.end(),
		ordersFromUnitState.begin(), ordersFromUnitState.end(),
		std::inserter(intersectSet, intersectSet.begin()));

	if (intersectSet.size() > 0)
	{
		assert(intersectSet.size() == 1);
		EOrder order = *intersectSet.begin();
		switch (order)
		{
		case EO_CANCEL:
			parUnit.CancelOrder();
			lastOrder_ = EO_NONE;
			break;
		case EO_STOP: parUnit.CancelOrder(); break;
		case EO_MOVE:
			lastOrder_ = EO_MOVE;
			parUnit.SetActionState(EUS_CHOOSE_DESTINATION);
			break;
		case EO_TRAIN_PEON: parUnit.Train(EUT_PEON); break;
		case EO_BUILD: parUnit.SetActionState(EUS_SELECT_BUILDING); break;
		case EO_BUILD_TOWN_HALL:
			lastOrder_ = EO_BUILD_TOWN_HALL;
			parUnit.SetActionState(EUS_CHOOSE_DESTINATION);
			break;
		};
	}
}

// ============================================================================

void HUD::ApplyLastOrderAtPosition(Unit& parUnit, const Vec2& parPosition)
{
	switch (lastOrder_)
	{
	case EO_MOVE: parUnit.Move(parPosition); break;
	case EO_BUILD_TOWN_HALL: parUnit.Build(EUT_TOWN_HALL, parPosition); break;
	default: break;
	};

	lastOrder_ = EO_NONE;
	parUnit.SetActionState(EUS_IDLE);
}

// ============================================================================

void HUD::GridClickHandler()
{
	if (!player.selectedUnit || mouse.lastLeftClickPos.y < 2* screen->h / 3)
		return;

	int gridClickPos = GridClickPositionFromMouse_();
	if (gridClickPos != -1)
		ApplyGridClick_(*player.selectedUnit, gridClickPos);
}

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================