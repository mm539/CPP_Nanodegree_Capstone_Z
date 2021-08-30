#include "mainmenu.h"
#include <thread>


MainMenu::MainMenu( std::size_t screen_width, std::size_t screen_height ) :
  _screenWidth( screen_width ), _screenHeight( screen_height )
{
  _title = TextDisplay( "Z", { 300, 200 } );
  makeButtons();
}

void MainMenu::Run( Controller const &controller,
                    Renderer &renderer,
                    std::size_t target_frame_duration,
                    Status &status )
{
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  Uint32 delay;

  while( status.running && status.menuScreen )
  {
    frame_start = SDL_GetTicks();

    // INPUTE UPDATE RENDER
    controller.handleEvent( status.running, _buttons, _clickedButtonSprite );
    update( status );
    renderer.renderAll( _buttons, _title );

    frame_end = SDL_GetTicks();
    frame_duration = frame_end - frame_start;
    delay = target_frame_duration - frame_duration;

    if( frame_duration < target_frame_duration )
    {
      SDL_Delay( delay );
    }
  }
}

void MainMenu::makeButtons()
{
  _buttons.push_back( Button( ButtonSprite::BUTTON_SPRITE_START, 130, 60, "../img/xx-start-130-60.bmp" ) );
  _buttons.push_back( Button( ButtonSprite::BUTTON_SPRITE_QUIT, 130, 60, "../img/xx-quit-130-60.bmp" ) );
}

void MainMenu::update( Status &status )
{
  updateButtons();
  buttonAction( status );
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
  switch( _clickedButtonSprite )
  {
    case ButtonSprite::BUTTON_SPRITE_START:
      status.menuScreen = false;
      status.gameScreen = true;
      break;
    case ButtonSprite::BUTTON_SPRITE_QUIT:
      status.menuScreen = false;
      status.running = false;
      break;
    default:
      break;
  }
  _clickedButtonSprite = ButtonSprite::BUTTON_SPRITE_NULL;
}
