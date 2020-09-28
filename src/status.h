#ifndef STATUS_H
#define STATUS_H

struct Status
{
  bool running { true };
  bool menuScreen { true };
  bool gameScreen { false };
  bool creditScreen { false };
};

#endif