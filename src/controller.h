#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "button.h"
#include "status.h"
#include <memory>

class Controller
{
 public:
  Controller();
  void handleEvent(bool& running, std::vector<std::shared_ptr<Building>>& buildings, std::vector<Button>& buttons, ButtonType& clickedButtonType, std::shared_ptr<Building>& clickedBuilding ) const; // without const, this doesn't work. why not? 
  void handleEvent( bool &running, std::vector<Button> &buttons, ButtonType &clickedButtonType ) const; // for the menu screen and credits screen
};


#endif