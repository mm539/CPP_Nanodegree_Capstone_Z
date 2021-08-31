#include <iostream>
#include <vector>
#include <future>
#include "gfx.h"
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "status.h"
#include "screen.h"

int main()
{
  InitDimen id;
  Status status;

  Renderer renderer( id.kSCREEN_WIDTH, id.kSCREEN_HEIGHT, 
                     id.kGRID_WIDTH, id.kGRID_HEIGHT );
  Controller controller;
  TextDisplay creditsMSG( " testing", { 100, 100 } );

  while( status.running )
  {
    if( status.screen == EScreen::MENU )
    {
      MainMenu mainMenu( id.kSCREEN_WIDTH, id.kSCREEN_HEIGHT );
      mainMenu.Run( controller, renderer, id.kMsPerFrame, status, EScreen::MENU );
    }
    else if( status.screen == EScreen::INITIAL_LOAD )
    {
      std::future<void> ftr = async(std::launch::async, [&status](){
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        status._mutex.lock();
        status.screen = EScreen::PLAYING;
        status._mutex.unlock();
      });
      LoadingScreen loadingScreen( id.kSCREEN_WIDTH, id.kSCREEN_HEIGHT );
      loadingScreen.Run( controller, renderer, id.kMsPerFrame, status, EScreen::INITIAL_LOAD );
    }
    else if( status.screen == EScreen::PLAYING )
    {
      Game game( id.kSCREEN_WIDTH, id.kSCREEN_HEIGHT, 
             id.kGRID_WIDTH, id.kGRID_HEIGHT, 
             id.leftPanelPD, id.topPanelPD,
             id.bottomPanelPD, id.mapPanelPD );
      game.Run( controller, renderer, id.kMsPerFrame, status );
      creditsMSG = game.getCreditsMSG();
    }
    else if( status.screen == EScreen::CREDIT )
    {
      CreditScreen creditScreen( id.kSCREEN_WIDTH, id.kSCREEN_HEIGHT, creditsMSG );
      creditScreen.Run( controller, renderer, id.kMsPerFrame, status, EScreen::CREDIT );
    }
  }
    std::cout << "Game has terminated successfully!\n";
}