#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "sprites.h"
#include <memory>

class Controller
{
 public:
  Controller();
  void handleEvent(bool& running, std::vector<Building>& buildings, int& clickedBuildingID, std::vector<Button>& buttons, ButtonSprite& clickedButtonSprite ) const; // without const, this doesn't work. why not?
};


#endif