#ifndef RENDERER_H_
#define RENDERER_H_


#include "util.h"
#include "unit.h"
#include "world.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

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
void Render(const Map& parMap);
void RenderRightClick(const Vec2& parPos);
void RenderSelection(SDL_Rect& parSrc, SDL_Rect& parDst, Uint32 parColor);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif