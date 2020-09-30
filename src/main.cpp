#include <iostream>
#include <vector>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "mainmenu.h"
#include "credits.h"
#include "status.h"

int main()
{
  constexpr std::size_t kFPS{ 60 };
  constexpr std::size_t kMsPerFrame{ 1000 / kFPS };
  constexpr int kSCREEN_WIDTH{ 960 };
  constexpr int kSCREEN_HEIGHT{ 880};
  constexpr int kGRID_WIDTH{ 90 }; // 720 360 180 90
  constexpr int kGRID_HEIGHT{ 85 }; // 680 340 170 85

  // 
  int leftPanelWidth = 240;
  int topPanelHeight = 120;
  int bottomPanelHeight = 80;

  // x, y, w, h
  PanelPD leftPanelPD( 0, 0, leftPanelWidth, kSCREEN_HEIGHT );
  PanelPD topPanelPD( leftPanelPD._w, 0, 
                       kSCREEN_WIDTH - leftPanelPD._w, topPanelHeight );
  PanelPD bottomPanelPD( leftPanelPD._w, kSCREEN_HEIGHT - bottomPanelHeight,
                       kSCREEN_WIDTH - leftPanelPD._w, bottomPanelHeight );
  PanelPD mapPanelPD( leftPanelPD._w, topPanelPD._h,
          kSCREEN_WIDTH - leftPanelPD._w, kSCREEN_HEIGHT - topPanelPD._h );

  Status status; // struct containing bools about whether the game is running and which screen ( menu, game, credits ) is active

  Renderer renderer( kSCREEN_WIDTH, kSCREEN_HEIGHT, 
                     kGRID_WIDTH, kGRID_HEIGHT );
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
             leftPanelPD, topPanelPD,
             bottomPanelPD, mapPanelPD );
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