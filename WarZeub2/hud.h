#ifndef HUD_H_
#define HUD_H_


#include "utils/singleton.h"
#include "utils/util.h"
#include "gameplay/unitDesc.h"
#include "gameplay/unit.h"
#include <SDL.h>


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class HUD : public Singleton<HUD>
{
public:
	HUD();
	~HUD();

public:
	bool IsInHUDRegion(const float2& parPos) const;

public:
	void GridClickHandler();
	void Render();

	void ApplyLastOrderAtPosition(Unit& parUnit, const float2& parPosition);

private:
	void InitCommandGridPosMapping_();
	int GridClickPositionFromMouse_();
	void ApplyGridClick_(Unit& parUnit, int parGridClickPos);

	void GenerateBackgroundSurface_();
	void RenderHUDCommand_(ECommand parCommand, const float2& parGridRegionOffset, bool parAvailable);
	void RenderSelectionInfos_(const float2& parInfoRegionOffset);
	void RenderMinimap_(const float2& parMinimapRegionOffset);
	void RenderBuildingPlacementIFN_() const;

private:
	SDL_Surface* iconsSurface_;
	SDL_Surface* backgroundSurface_;

	std::map<ECommand, int> commandToGridPos_; // 0 to 8 (3 per line)
	std::map<int, std::set<ECommand> > gridPosToCommands_;
	ECommand lastCommand_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif