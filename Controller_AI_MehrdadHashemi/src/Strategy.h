#pragma once

#include "../Game/src/Mob.h"
#include "../Game/src/Player.h"
#include "Constants.h"
#include "EntityStats.h"
#include "iPlayer.h"
#include "Vec2.h"

// We place the giant behind so that we have time to prepare support for him
// the Swordsman are put in front to stop the oponents Giant.

 // For Left Wing:
static const Vec2 ksGiantPos(LEFT_BRIDGE_CENTER_X, 0.f);
static const Vec2 ksArcherPos(LEFT_BRIDGE_CENTER_X, 0.f);
static const Vec2 ksSwordPos(LEFT_BRIDGE_CENTER_X, RIVER_TOP_Y - 0.5f);

// For Right Wing:
static const Vec2 ksGiantPos(RIGHT_BRIDGE_CENTER_X, 0.f);
static const Vec2 ksArcherPos(RIGHT_BRIDGE_CENTER_X, 0.f);
static const Vec2 ksSwordPos(RIGHT_BRIDGE_CENTER_X, RIVER_TOP_Y - 0.5f);

class Strategy {
public:
  Strategy(Player* p);
  ~Strategy();

protected:

  float EnemykingLife;  // to track enemy towers life left
  float leftEnemyLife;
  float rightEnemyLife;
  int myArchers;        // to know how many of each mob we have
  int MyGiants;
  int MySwordsman;
  float my_Elixir;      // tracking elixir is the most important
  float enemy_Elixir;

}
