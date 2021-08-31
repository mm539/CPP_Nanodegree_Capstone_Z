#ifndef STATUS_H
#define STATUS_H

enum Screen{
  NONE = 0,
  MENU = 1,
  PLAYING = 2,
  CREDIT = 3
};

struct Status
{
  bool running { true };
  Screen screen { Screen::MENU };
};

#endif