#ifndef STATUS_H
#define STATUS_H

#include <mutex>


enum EEndGameState{
  VICTORY = 1,
  PLAYER_DEATH = 2,
  BASE_DEATH = 3,
};

enum EScreen{
  NONE = 0,
  MENU = 1,
  PLAYING = 2,
  CREDIT = 3,
  INITIAL_LOAD = 4,
};

struct Status
{
  std::mutex _mutex;
  bool running { true };
  EScreen screen { EScreen::MENU };
  EEndGameState endGameState;
};

#endif