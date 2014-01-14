#ifndef RENDERER_H_
#define RENDERER_H_


#include "Unit.h"


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void InitRenderer();
void ReleaseRenderer();
void BeginScene();
void EndScene();

void Render(const Unit& parUnit);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================


#endif