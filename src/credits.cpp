#include "credits.h"

Credits::Credits( std::size_t screen_width, std::size_t screen_height, TextDisplay &creditsMSG ) :
  _screenWidth( screen_width ), _screenHeight( screen_height ), _credits( creditsMSG )
{
  makeButtons();
}

void Credits::Run( Controller &controller,
                   Renderer &renderer,
                   std::size_t target_frame_duration,
                   Status &status )
{
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;

  while( status.running && status.screen == Screen::CREDIT )
  {
    frame_start = SDL_GetTicks();

    // INPUT UPDATE RENDER
    controller.handleEvent( status.running, _buttons, _clickedButtonType );
    update( status );
    renderer.renderAll( _buttons, _credits );

    frame_end = SDL_GetTicks();
    frame_duration = frame_start - frame_end;
    if( frame_duration < target_frame_duration )
    {
      SDL_Delay( target_frame_duration - frame_duration );
    }
  }
}

void Credits::makeButtons()
{
 _buttons.push_back( Button( ButtonType::QUIT, 130, 60 ) );
}

void Credits::update( Status &status )
{
  updateButtons();
  buttonAction( status );
}

void Credits::updateButtons()
{
  std::for_each( _buttons.begin(), _buttons.end(), [] (Button &button)
  {
    button.setButtonState( ButtonState::BUTTON_VISIBLE );
  });
}

void Credits::buttonAction( Status &status )
{
  switch( _clickedButtonType )
  {
    case ButtonType::QUIT:
      status.running = false;
      break;
    default:
      break;
  }
  _clickedButtonType = ButtonType::BNULL;
}

