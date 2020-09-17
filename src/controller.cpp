#include "controller.h"
#include <iostream>
#include "SDL.h"

Controller::Controller()
{
  
}

void Controller::handleEvent( bool &running, 
            std::vector<std::shared_ptr<Building>>& buildings, std::vector<Button>& buttons, ButtonSprite& clickedButtonSprite, std::shared_ptr<Building>& clickedBuilding ) const
{
  SDL_Event e;
  bool mouseDown = false;
  bool mouseUp = false;
  bool inside = false;
  int x, y;

  while( SDL_PollEvent( &e ) )
  {
   
    if( e.type == SDL_QUIT ) running = false;
    else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP )
    { 
      // check user interactions with building sprites
      for( int i = 0; i < buildings.size(); i++ )
      {
        if( buildings[ i ]->handleEvent( &e ) ) //if building is clicked, true
        {
          clickedBuilding = buildings[ i ];
          std::cout << "clicked building ID: " << clickedBuilding->getID() << std::endl;
        }
      }

      // check user interactions with button sprites
      for( int j = 0; j < buttons.size(); j++ )
      {
        if( buttons[ j ].handleEvent( &e ) && buttons[ j ].getButtonState() == ButtonState::BUTTON_VISIBLE) //if button is clicked && button is visible, true
        {
          clickedButtonSprite = buttons[ j ].getSprite();
          std::cout << "clicked button sprite: " << clickedButtonSprite << std::endl;
        }
      }
    }
  }
}
