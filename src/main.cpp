#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "mainmenu.h"
#include "credits.h"
#include "status.h"

int main()
{
  constexpr std::size_t kTOP_PANEL_HEIGHT { 120 };
  constexpr std::size_t kBOTTOM_PANEL_HEIGHT { 80 };
  constexpr std::size_t kFPS{ 60 };
  constexpr std::size_t kMsPerFrame{ 1000 / kFPS };
  constexpr std::size_t kSCREEN_WIDTH{ 960 }; 
  constexpr std::size_t kSCREEN_HEIGHT{ 680 + kTOP_PANEL_HEIGHT + kBOTTOM_PANEL_HEIGHT};
  constexpr std::size_t kGRID_WIDTH{ 90 }; // 720 360 180 90
  constexpr std::size_t kGRID_HEIGHT{ 85 }; // 680 340 170 85
  constexpr std::size_t kLEFT_PANEL_WIDTH{ 240 };
  Status status;

  Renderer renderer( kSCREEN_WIDTH, kSCREEN_HEIGHT, 
                     kGRID_WIDTH, kGRID_HEIGHT, 
                     kLEFT_PANEL_WIDTH, kTOP_PANEL_HEIGHT,
                     kBOTTOM_PANEL_HEIGHT );
  Controller controller;

  while( status.running )
  {
    if( status.menuScreen )
    {
      MainMenu mainMenu( kSCREEN_WIDTH, kSCREEN_HEIGHT );
      mainMenu.Run( controller, renderer, kMsPerFrame, status );
    }
    else if( status.gameScreen )
    {
      Game game( kSCREEN_WIDTH, kSCREEN_HEIGHT, 
             kGRID_WIDTH, kGRID_HEIGHT, 
             kLEFT_PANEL_WIDTH, kTOP_PANEL_HEIGHT,
             kBOTTOM_PANEL_HEIGHT );
      game.Run( controller, renderer, kMsPerFrame, status );
    }
    else if( status.creditScreen )
    {
      Credits credits( kSCREEN_WIDTH, kSCREEN_HEIGHT );
      credits.Run( controller, renderer, kMsPerFrame, status );
    }
  }
    std::cout << "Game has terminated successfully!\n";
}