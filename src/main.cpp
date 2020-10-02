#include <iostream>
#include <vector>
#include "gfx.h"
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "mainmenu.h"
#include "credits.h"
#include "status.h"

int main()
{
  InitDimen id;
  Status status; // which screen ( menu, game, credits ) is active

  Renderer renderer( id.kSCREEN_WIDTH, id.kSCREEN_HEIGHT, 
                     id.kGRID_WIDTH, id.kGRID_HEIGHT );
  Controller controller;

  while( status.running )
  {
    if( status.menuScreen )
    {
      MainMenu mainMenu( id.kSCREEN_WIDTH, id.kSCREEN_HEIGHT );
      mainMenu.Run( controller, renderer, id.kMsPerFrame, status );
    }
    else if( status.gameScreen )
    {
      Game game( id.kSCREEN_WIDTH, id.kSCREEN_HEIGHT, 
             id.kGRID_WIDTH, id.kGRID_HEIGHT, 
             id.leftPanelPD, id.topPanelPD,
             id.bottomPanelPD, id.mapPanelPD );
      game.Run( controller, renderer, id.kMsPerFrame, status );
    }
    else if( status.creditScreen )
    {
      Credits credits( id.kSCREEN_WIDTH, id.kSCREEN_HEIGHT );
      credits.Run( controller, renderer, id.kMsPerFrame, status );
    }
  }
    std::cout << "Game has terminated successfully!\n";
}