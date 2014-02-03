#ifndef PLAYER_H_
#define PLAYER_H_


// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

class Unit;

class Player
{
public:
	void UpdateSelection();

public: // FIXME: viva abstraction!
	bool selectionMode;
	Unit* selectedUnit;
};

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

extern Player player;

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================

void UpdateSelection(Player& player);

// ============================================================================
// ----------------------------------------------------------------------------
// ============================================================================$


#endif