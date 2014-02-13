#ifndef RENDERER_H_
#define RENDERER_H_


#include "../utils/util.h"
#include "../gameplay/unit.h"
#include "../gameplay/world.h"
#include "camera.h"
#include "spriteDesc.h"
#include "font.h"


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
// ----------------------------------------------------------------------------
// ============================================================================

extern const SDL_Rect viewport;
extern Camera* gCamera;
extern SDL_Surface* screen;
extern SDL_Surface* summerTilesSurface;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitRenderer();
void ReleaseRenderer();
void BeginScene();
void EndScene();

void Render(SDL_Surface* parSrcSurface,
				const SpriteDesc& parSpriteDesc,
				const float2& parWorldPos,
				EDir parDir,
				int parSpriteStep);

void Render(EUnitType parUnitType, const float2& parScreenPos); // used by building placement
void Render(const World& parWorld);
void RenderSquare(SDL_Rect& parSrc, SDL_Rect& parDst, Uint32 parColor);
void RenderProgressBar(SDL_Rect& parDimensions, float parStatus);

void RenderText(const int2& parScreenPos, const char* parFormat, ...);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif