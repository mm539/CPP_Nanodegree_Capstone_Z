#ifndef STATUS_H
#define STATUS_H

enum EScreen{
  NONE = 0,
  MENU = 1,
  PLAYING = 2,
  CREDIT = 3,
  INITIAL_LOAD = 4,
};

struct Status
{
  bool running { true };
  EScreen screen { EScreen::MENU };
};

#endif