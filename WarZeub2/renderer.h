#ifndef RENDERER_H_
#define RENDERER_H_


#include "util.h"
#include "unit.h"
#include "world.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

#if 1
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
#else
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
#endif

// ============================================================================

extern SDL_Surface* screen;
extern SDL_Surface* summerTilesSurface;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitRenderer();
void ReleaseRenderer();
void BeginScene();
void EndScene();

void Render(const Unit& parUnit);
void Render(EUnitType parUnitType, const Vec2& parPos); // used by building placement
void Render(const Map& parMap);
void RenderRightClick(const Vec2& parPos);
void RenderSquare(SDL_Rect& parSrc, SDL_Rect& parDst, Uint32 parColor);
void RenderProgressBar(SDL_Rect& parDimensions, float parStatus);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif