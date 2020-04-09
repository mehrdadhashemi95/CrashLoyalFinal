// MIT License
//
// Copyright(c) 2020 Arthur Bacon and Kevin Dill
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Controller_AI_MehrdadHashemi.h"

#include "Constants.h"
#include "EntityStats.h"
#include "iPlayer.h"
#include "Vec2.h"
#include "Game.h"
#include "Entity.h"
#include "Player.h"

static const Vec2 ksGiantPos(LEFT_BRIDGE_CENTER_X, 1.f);
static const Vec2 ksArcher1Pos(LEFT_BRIDGE_CENTER_X - 2.5f, 0.f);
static const Vec2 ksArcher2Pos(LEFT_BRIDGE_CENTER_X + 2.5f, 0.f);
static const Vec2 ksSword1Pos(LEFT_BRIDGE_CENTER_X - 3.0f, RIVER_TOP_Y - 6.0f);
static const Vec2 ksSword2Pos(LEFT_BRIDGE_CENTER_X, RIVER_TOP_Y - 6.5f);
static const Vec2 ksSword3Pos(LEFT_BRIDGE_CENTER_X + 3.0f, RIVER_TOP_Y - 6.0f);

static const float archerY = 2.f;
static const float archerXLeft = LEFT_BRIDGE_CENTER_X;// + 1.f;
static const float archerXRight = RIGHT_BRIDGE_CENTER_X;// - 1.f;

static const float giantY = 5.f;
static const float giantXLeft = LEFT_BRIDGE_CENTER_X;
static const float giantXRight = RIGHT_BRIDGE_CENTER_X;

static const float swordY = 7.f;
static const float swordXLeft = LEFT_BRIDGE_CENTER_X;// + 3.f;
static const float swordXRight = RIGHT_BRIDGE_CENTER_X;// - 3.f;

Game& game = Game::get();

std::vector<Entity*> enemyTowers;
std::vector<Entity*> ourTowers;
int code = 3;
bool flag = false;
float minTowersHealth = 2000;

void Controller_AI_MehrdadHashemi::tick(float deltaTSec)
{

  assert(m_pPlayer);

  bool isNorth = m_pPlayer->isNorth();
  enemyTowers = game.getPlayer(!isNorth).getBuildings();
  ourTowers = game.getPlayer(isNorth).getBuildings();

// Codes path:  0 -> 1 -> -2 -> 2 -> -3 -> -1 -> 1 -> -2 -> ...
// Code 0 is only the initial state.
// if the code of attack is more than 0 we are in an offensive state.
  findEnemyGiant();
  if (code == 1) {
    if (m_pPlayer->getElixir() >= 9) {

      findEnemyArcher();

      Vec2 swordPosition1(decideWhichLane(swordXLeft, swordXRight), swordY);
      Vec2 swordPos_Game1 = swordPosition1.Player2Game(isNorth);
      m_pPlayer->placeMob(iEntityStats::Swordsman, swordPos_Game1);

      Vec2 swordPosition2(decideWhichLane(swordXLeft, swordXRight) + 2.f, swordY);
      Vec2 swordPos_Game2 = swordPosition2.Player2Game(isNorth);
      m_pPlayer->placeMob(iEntityStats::Swordsman, swordPos_Game2);

      Vec2 swordPosition3(decideWhichLane(swordXLeft, swordXRight)  - 2.f, swordY);
      Vec2 swordPos_Game3 = swordPosition3.Player2Game(isNorth);
      m_pPlayer->placeMob(iEntityStats::Swordsman, swordPos_Game3);

      if (princeTowersUnderAttack()) {

        code = 2;
      } else {

        code = 3;
      }
    }
  } else if (code == 2) {
    findEnemyGiant();
    if (m_pPlayer->getElixir() >= 3) {

      Vec2 archerPosition1(decideWhichLane(archerXLeft, archerXRight) - 1.f, RIVER_TOP_Y);
      Vec2 archerPos_Game1 = archerPosition1.Player2Game(isNorth);
      m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game1);

      if (princeTowersUnderAttack()) {

        code = 3;
      } else {
        code = -3;
      }
    }
  } else if (code == 3) {
    if (m_pPlayer->getElixir() >= 9) {

      findEnemyArcher();

      Vec2 giantPosition(decideWhichLane(giantXLeft, giantXRight), giantY);
      Vec2 giantPos_Game = giantPosition.Player2Game(isNorth);
      m_pPlayer->placeMob(iEntityStats::Giant, giantPos_Game);

      Vec2 archerPosition1(decideWhichLane(archerXLeft, archerXRight) - 1.f, archerY);
      Vec2 archerPos_Game1 = archerPosition1.Player2Game(isNorth);
      m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game1);

      Vec2 archerPosition2(decideWhichLane(archerXLeft, archerXRight) + 1.f, archerY);
      Vec2 archerPos_Game2 = archerPosition2.Player2Game(isNorth);
      m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game2);

      code = -3;
    }
  } else if (code == -3) {
    if (m_pPlayer->getElixir() >= 3) {

      Vec2 archerPosition1(decideWhichLane(archerXLeft, archerXRight) - 1.f, RIVER_TOP_Y - 5.f);
      Vec2 archerPos_Game1 = archerPosition1.Player2Game(isNorth);
      m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game1);

      code = -2;
    }
  } else if (code == -2) {
    if (m_pPlayer->getElixir() >= 5) {

      Vec2 swordPosition(decideWhichLane(swordXLeft, swordXRight), swordY);
      Vec2 swordPos_Game = swordPosition.Player2Game(isNorth);
      m_pPlayer->placeMob(iEntityStats::Swordsman, swordPos_Game);

      Vec2 archerPosition(decideWhichLane(archerXLeft, archerXRight), archerY);
      Vec2 archerPos_Game = archerPosition.Player2Game(isNorth);
      m_pPlayer->placeMob(iEntityStats::Archer, archerPos_Game);

      if (princeTowersUnderAttack()) {

        code = 3;
      } else {

        code = -1;
      }
    }
  } else {
    //  default: 0 or -1
    if (m_pPlayer->getElixir() >= 3) {

      Vec2 swordPosition(decideWhichLane(swordXLeft, swordXRight), RIVER_TOP_Y);
      Vec2 swordPos_Game = swordPosition.Player2Game(isNorth);
      m_pPlayer->placeMob(iEntityStats::Swordsman, swordPos_Game);

      code = 1;
    }
  }
}


float Controller_AI_MehrdadHashemi::decideWhichLane(float leftPosition, float rightPosition) {

  std::vector<Entity*> enemyPrincesTowers;
  for (Entity* e : enemyTowers) {
    if (e->getStats().getBuildingType() == iEntityStats::BuildingType::Princess) {
      enemyPrincesTowers.push_back(e);
    }
  }

  int x;
  if (enemyPrincesTowers[0]->getHealth() < enemyPrincesTowers[1]->getHealth()) {
    x = 0;
  } else { x = 1; }

  float leftDifference = abs(enemyPrincesTowers[x]->getPosition().x - leftPosition);
  float rightDifference = abs(enemyPrincesTowers[x]->getPosition().x - rightPosition);
  if (rightDifference > leftDifference) {
    return rightPosition;
  } else { return leftPosition; }

}

bool Controller_AI_MehrdadHashemi::princeTowersUnderAttack() {

  float x = 10000;
  std::vector<Entity*> allOutTowers;

  for (Entity* e : ourTowers) {
    if (e->getHealth() < x) {
      x = e->getHealth();
    }
  }

  if (minTowersHealth != x) {
    // in this case the towers have lost health.
    // so they are under attack.
    minTowersHealth == x;
    return true;
  }

  return false;
}

bool Controller_AI_MehrdadHashemi::findEnemyGiant() {
  for (Entity* mob : game.getPlayer(m_pPlayer->isNorth()).getMobs()) {
    if (mob->getStats().getMobType() == iEntityStats::Giant
        && !mob->isDead()) {
          code = 0;
          return true;
        }
  }
  return false;
}

bool Controller_AI_MehrdadHashemi::findEnemyArcher() {
  for (Entity* mob : game.getPlayer(m_pPlayer->isNorth()).getMobs()) {
    if (mob->getStats().getMobType() == iEntityStats::Archer
        && !mob->isDead()) {
          code = -1;
          return true;
        }
  }
  return false;
}

bool Controller_AI_MehrdadHashemi::findEnemySword() {
  for (Entity* mob : game.getPlayer(m_pPlayer->isNorth()).getMobs()) {
    if (mob->getStats().getMobType() == iEntityStats::Swordsman
        && !mob->isDead()) {
          code = -2;
          return true;
        }
  }
  return false;
}
