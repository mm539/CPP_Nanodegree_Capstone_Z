#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "sprites.h"
#include <memory>

class Controller
{
 public:
  Controller();
  void handleEvent(bool& running, std::vector<std::shared_ptr<Building>>& buildings, std::vector<Button>& buttons, ButtonSprite& clickedButtonSprite, std::shared_ptr<Building>& clickedBuilding ) const; // without const, this doesn't work. why not?
};


#endif