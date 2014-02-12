#include "hud.h"
#include "graphic/renderer.h"
#include "graphic/spriteDesc.h"
#include "gameplay/player.h"
#include "userInput.h"
#include <assert.h>
#include <algorithm>
#include <SDL_Image.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

HUD::HUD() :
	lastCommand_(EC_NONE)
{
	InitCommandGridPosMapping_();

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

	SDL_Rect src = { (BUILD_TILE_SIZE + 1) * 11, (BUILD_TILE_SIZE + 1) * 17, BUILD_TILE_SIZE, BUILD_TILE_SIZE }; // (11, 17) = mud
	SDL_Rect dst = { 0, 0, 0, 0 };

	for (size_t line = 0; line < (1 + screen->h / BUILD_TILE_SIZE); ++line)
	{
		for (size_t col = 0; col < (1 + backgroundSurface_->w / BUILD_TILE_SIZE); ++col)
		{
			dst.x = col * BUILD_TILE_SIZE;
			dst.y = line * BUILD_TILE_SIZE;
			SDL_BlitSurface(summerTilesSurface, &src, backgroundSurface_, &dst);
		}
	}
}

// ============================================================================

void HUD::RenderSelectionInfos_(const float2& parInfoRegionOffset)
{
	assert(player.selectedUnit);

	SDL_Rect borderSrc = { Sint16(parInfoRegionOffset.x),
		Sint16(parInfoRegionOffset.y) + (backgroundSurface_->h / 3), 0, 0 };
	SDL_Rect borderDst = { backgroundSurface_->w - (2 * Sint16(parInfoRegionOffset.x)),
		(2*backgroundSurface_->h / 3) - Sint16(parInfoRegionOffset.y), 0, 0 };
	RenderSquare(borderSrc, borderDst, 0x00ffffff);

	const SpriteDesc& unitIconSpriteDesc = unitTypeToIconSpriteDesc[player.selectedUnit->Type()];
	SDL_Rect src = { unitIconSpriteDesc.offsetX, unitIconSpriteDesc.offsetY,
		unitIconSpriteDesc.width, unitIconSpriteDesc.height };
	SDL_Rect dst = { borderSrc.x + 1, borderSrc.y + 1, 0, 0 };
	SDL_BlitSurface(iconsSurface_, &src, screen, &dst);

	SDL_Rect progressBarRect = { 2*Sint16(parInfoRegionOffset.x), borderSrc.y + 60,
			backgroundSurface_->w - (5 * Sint16(parInfoRegionOffset.x)), 15 };
	if (player.selectedUnit->ActionState() == EUS_TRAINING)
		RenderProgressBar(progressBarRect, player.selectedUnit->OrderCompletionStatus());
	else if (player.selectedUnit->IsBeingConstructed())
	{
		const Unit* builder = World::Inst()->BuilderOf(player.selectedUnit);
		RenderProgressBar(progressBarRect, builder->OrderCompletionStatus());
	}
}

// ============================================================================

void HUD::RenderMinimap_(const float2& parMinimapRegionOffset)
{
	static SDL_Rect miniMapRect = {Sint16(parMinimapRegionOffset.x),
		Sint16(parMinimapRegionOffset.y),
		backgroundSurface_->w - Sint16(2.f * parMinimapRegionOffset.x),
		(backgroundSurface_->h / 3) - Sint16(2.f * parMinimapRegionOffset.y)};
	SDL_FillRect(screen, &miniMapRect, 0);
}

// ============================================================================

void HUD::RenderBuildingPlacementIFN_() const
{
	if (!player.selectedUnit ||
		(player.selectedUnit->ActionState() != EUS_CHOOSE_DESTINATION) ||
		((lastCommand_ != EC_BUILD_TOWN_HALL) &&
		(lastCommand_ != EC_BUILD_FARM) &&
		(lastCommand_ != EC_BUILD_BARRACK)))
	{
		return;
	}

	EUnitType unitType;
	switch (lastCommand_)
	{
	case EC_BUILD_TOWN_HALL: unitType = EUT_TOWN_HALL; break;
	case EC_BUILD_FARM: unitType = EUT_FARM; break;
	case EC_BUILD_BARRACK: unitType = EUT_BARRACK; break;
	};

	float2 pos(mouse.pos);
	TransformToWorldCoordinate(pos, gCamera->Pos());
	BuildTile::Align(pos, unitType);
	TransformToScreenCoordinate(pos, gCamera->Pos());

	::Render(unitType, pos);
}

// ============================================================================

void HUD::Render()
{
	RenderBuildingPlacementIFN_();

	SDL_BlitSurface(backgroundSurface_, 0, screen, 0);

	float2 minimapRegionOffset(float(backgroundSurface_->w) / 20.f,
		(float(backgroundSurface_->h) / 3.f ) / 20.f);
	RenderMinimap_(minimapRegionOffset);

	if (player.selectedUnit)
	{
		// Selection info
		float2 infoRegionOffset(float(backgroundSurface_->w) / 50.f,
			(float(backgroundSurface_->h) / 3.f) / 50.f);
		RenderSelectionInfos_(infoRegionOffset);

		// Grid
		float2 commandHudOffset(infoRegionOffset.x,
			(2.f * float(backgroundSurface_->h) / 3.f) + infoRegionOffset.y);
		const std::set<ECommand>& unitCommands =
			unitTypeToUnitDesc[player.selectedUnit->Type()].unitStateToCommandSet[player.selectedUnit->ActionState()];
		for (std::set<ECommand>::const_iterator command = unitCommands.begin();
			command != unitCommands.end(); ++command)
		{
			RenderHUDCommand_(*command, commandHudOffset);
		}
	}
}

// ============================================================================

void HUD::RenderHUDCommand_(ECommand parCommand, const float2& parGridRegionOffset)
{
	if (parCommand == EC_NONE)
		return;

	const SpriteDesc& commandIconSpriteDesc = commandToIconSpriteDesc[parCommand];
	SDL_Rect commandIconSrc = { commandIconSpriteDesc.offsetX, commandIconSpriteDesc.offsetY,
		commandIconSpriteDesc.width, commandIconSpriteDesc.height };

	int gridPos = commandToGridPos_[parCommand];
	SDL_Rect commandIconDst = { Sint16(parGridRegionOffset.x) + (gridPos % 3) * (commandIconSpriteDesc.width + 5),
		Sint16(parGridRegionOffset.y) + (gridPos / 3) * (commandIconSpriteDesc.height + 5), 0, 0};
	SDL_BlitSurface(iconsSurface_, &commandIconSrc, screen, &commandIconDst);
}

// ============================================================================

void HUD::InitCommandGridPosMapping_()
{
	commandToGridPos_[EC_MOVE] = 0;
	commandToGridPos_[EC_STOP] = 1;
	commandToGridPos_[EC_CANCEL] = 8;
	commandToGridPos_[EC_TRAIN_PEON] = 0;
	commandToGridPos_[EC_BUILD] = 6;
	commandToGridPos_[EC_BUILD_TOWN_HALL] = 0;
	commandToGridPos_[EC_BUILD_FARM] = 1;
	commandToGridPos_[EC_BUILD_BARRACK] = 2;

	for (std::map<ECommand, int>::iterator it = commandToGridPos_.begin();
			it != commandToGridPos_.end(); ++it)
	{
		gridPosToCommands_[it->second].insert(it->first);
	}
}

// ============================================================================

bool HUD::IsInHUDRegion(const float2& parPos) const
{
	return (parPos.x <= float(screen->w / 5));
}

// ============================================================================

int HUD::GridClickPositionFromMouse_()
{
	SDL_Rect mouseRect = BoundingBoxFromMouse(mouse, false);
	const SpriteDesc& commandIconSpriteDesc = commandToIconSpriteDesc[EC_CANCEL];

	for (int line = 0; line < 3; ++line)
	{
		for (int col = 0; col < 3; ++col)
		{
			int iconOffsetX = (screen->w / 5) / 50 + col * (commandIconSpriteDesc.width + 5);
			int iconOffsetY = (2 * screen->h / 3) + ((screen->h / 3) / 50) + line * (commandIconSpriteDesc.height + 5);
			SDL_Rect iconRect = {iconOffsetX, iconOffsetY, commandIconSpriteDesc.width, commandIconSpriteDesc.height};

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

	const std::set<ECommand>& commandsOnGridPos = gridPosToCommands_[parGridClickPos];
	UnitDesc& unitDesc = unitTypeToUnitDesc[parUnit.Type()];
	const std::set<ECommand>& commandsFromUnitState =
		unitDesc.unitStateToCommandSet[parUnit.ActionState()];

	std::set<ECommand> intersectSet;
	std::set_intersection(commandsOnGridPos.begin(), commandsOnGridPos.end(),
		commandsFromUnitState.begin(), commandsFromUnitState.end(),
		std::inserter(intersectSet, intersectSet.begin()));

	if (intersectSet.size() > 0)
	{
		assert(intersectSet.size() == 1); // otherwise, there is a conflict
		ECommand order = *intersectSet.begin();
		switch (order)
		{
		case EC_CANCEL:
			if (parUnit.IsBeingConstructed())
				World::Inst()->RemoveUnit(&parUnit);
			else
				parUnit.CancelOrder();
			break;
		case EC_STOP: parUnit.CancelOrder(); break;
		case EC_TRAIN_PEON: parUnit.Train(EUT_PEON); break;
		case EC_BUILD: parUnit.SetActionState(EUS_SELECT_BUILDING); break; // TODO: It shouldn't modified unit action state
		// orders with destination
		case EC_MOVE:
		case EC_BUILD_FARM:
		case EC_BUILD_BARRACK:
		case EC_BUILD_TOWN_HALL:
			lastCommand_ = order;
			parUnit.SetActionState(EUS_CHOOSE_DESTINATION); // TODO: It shouldn't modified unit action state
			break;
		};
	}
}

// ============================================================================

void HUD::ApplyLastOrderAtPosition(Unit& parUnit, const float2& parPosition)
{
	switch (lastCommand_)
	{
	case EC_MOVE: parUnit.Move(parPosition); break;
	case EC_BUILD_FARM: parUnit.Build(EUT_FARM, parPosition); break;
	case EC_BUILD_TOWN_HALL: parUnit.Build(EUT_TOWN_HALL, parPosition); break;
	case EC_BUILD_BARRACK: parUnit.Build(EUT_BARRACK, parPosition); break;
	};

	lastCommand_ = EC_NONE;
	// TODO: It shouldn't modified unit action state
	parUnit.SetActionState(EUS_IDLE); // used to reset the 'hud state' of the unit
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