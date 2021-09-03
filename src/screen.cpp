#include "screen.h"
#include <thread>


void Screen::Run( Controller const &controller,
                    Renderer &renderer,
                    std::size_t target_frame_duration,
                    Status &status,
                    EScreen desiredScreen )
{
  childRun(status);
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int delay;

  while( true )
  {
    status._mutex.lock();
    if ( status.running == false || status.screen != desiredScreen ){
      status._mutex.unlock();
      break;
    }
    frame_start = SDL_GetTicks();

    // INPUT UPDATE RENDER
    controller.handleEvent( status.running, _buttons, _clickedButtonInfo );
    update( status );
    childRender( renderer );// renderer.renderAll( _buttons, _title );
    renderer.playMusic();

    status._mutex.unlock();
    
    frame_end = SDL_GetTicks();
    frame_duration = frame_end - frame_start;
    delay = target_frame_duration - frame_duration;

    if( frame_duration < target_frame_duration )
    {
      SDL_Delay( delay );
    }
  }
}

void Screen::update( Status &status )
{
  updateButtons();
  buttonAction( status );
}


/*
  Derived Class Constructors
*/

MainMenu::MainMenu( std::size_t screen_width, std::size_t screen_height )
{
  _screenWidth = screen_width;
  _screenHeight= screen_height;
  makeButtons();
}

LoadingScreen::LoadingScreen( std::size_t screen_width, std::size_t screen_height )
{
  _screenWidth = screen_width;
  _screenHeight= screen_height;
  makeButtons();
}

CreditScreen::CreditScreen( std::size_t screen_width, std::size_t screen_height)
{  
  _screenWidth = screen_width;
  _screenHeight= screen_height;
  makeButtons();
}

/*
  Derived Class methods
*/

void MainMenu::makeButtons()
{
  _buttons.push_back( Button( ButtonType::START, 130, 60 ) );
  _buttons.push_back( Button( ButtonType::QUIT, 130, 60 ) );
}

void LoadingScreen::makeButtons()
{
  //
}

void CreditScreen::makeButtons()
{
  //
}

void MainMenu::childRender( Renderer &renderer ){
  renderer.renderAll( _buttons, _msg, _backgroundImg );
  if (_clickedButtonInfo._clicked == true){
    _clickedButtonInfo._clicked = false;
    renderer.playClickSound();
  }
}

void LoadingScreen::childRender( Renderer &renderer ){
  renderer.renderAll( _buttons, _msg, _backgroundImg );
}

void CreditScreen::childRender( Renderer &renderer ){
  renderer.renderAll( _buttons, _msg, _backgroundImg );
}

void MainMenu::updateButtons()
{
  std::for_each( _buttons.begin(), _buttons.end(), [] ( Button &button) 
  {
    button.setButtonState( ButtonState::BUTTON_VISIBLE );
  } );
}

void MainMenu::buttonAction( Status &status)
{
  switch( _clickedButtonInfo._buttonType )
  {
    case ButtonType::START:
      status.screen = EScreen::INITIAL_LOAD;
      break;
    case ButtonType::QUIT:
      status.screen = EScreen::NONE;
      status.running = false;
      break;
    default:
      break;
  }
  _clickedButtonInfo._buttonType = ButtonType::BNULL;
}

void LoadingScreen::updateButtons()
{
  //
}

void LoadingScreen::buttonAction( Status &status)
{
  //
}

void CreditScreen::updateButtons()
{
  //
}

void CreditScreen::buttonAction( Status &status)
{
  //
}

/*

*/

void MainMenu::childRun( Status &status)
{
  _msg = TextDisplay( "Z", { 300, 200 } );
  _msg.setTextureWH( 256, 256);
  _msg.setTextHeight( 80 );

  _backgroundImg.setImgPath("../img/main-menu.bmp");
  _backgroundImg.setTextureWH(1024, 1024);
}

void LoadingScreen::childRun( Status &status)
{
  _msg = TextDisplay( "loading", { 300, 200 } );

  _backgroundImg.setImgPath("../img/z128red.bmp");
  _backgroundImg.setTextureWH(512, 512);
  _backgroundImg.setPosition( 256, 256);
}

void CreditScreen::childRun( Status &status)
{
  std::string msgText;
  std::string imgPath;

  if (status.endGameState == EEndGameState::VICTORY)
  {
    msgText = "A passing helicopter spots you and picks you up. You survived!";
    imgPath = "../img/heli.bmp";
  }
  else if (status.endGameState == EEndGameState::PLAYER_DEATH)
  {
    msgText = "The zombies tore you apart!";
    imgPath = "../img/death-192.bmp";
  }
  else if (status.endGameState == EEndGameState::BASE_DEATH)
  {
    msgText = "The zombies overran your base!";
    imgPath = "../img/death-192.bmp";
  }

  _msg = TextDisplay( msgText , {256, 256 });
  _msg.setTextureWH( 256, 256);
  _msg.setTextHeight( 80 );
  _msg.setPosition({256, 256});

  _backgroundImg.setImgPath( imgPath );
  _backgroundImg.setTextureWH(256, 256);
  _backgroundImg.setPosition( 256, 362);

}