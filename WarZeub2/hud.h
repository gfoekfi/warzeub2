#ifndef HUD_H_
#define HUD_H_


#include "singleton.h"
#include "util.h"
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
	bool IsInHUDRegion(const Vec2& parPos) const;

public:
	void GridClickHandler();
	void Render();

	void ApplyLastOrderAtPosition(Unit& parUnit, const Vec2& parPosition);

private:
	void InitOrderGridPosMapping_();
	int GridClickPositionFromMouse_();
	void ApplyGridClick_(Unit& parUnit, int parGridClickPos);

	void GenerateBackgroundSurface_();
	void RenderHUDOrder_(EOrder parOrder, const Vec2& parGridRegionOffset);
	void RenderSelectionInfos_(const Vec2& parInfoRegionOffset);
	void RenderMinimap_(const Vec2& parMinimapRegionOffset);

private:
	std::map<EOrder, int> orderToGridPos_; // 0 to 8 (3 per line)
	std::map<int, std::set<EOrder> > gridPosToOrders_;
	SDL_Surface* iconsSurface_;
	SDL_Surface* backgroundSurface_;
	EOrder lastOrder_;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif