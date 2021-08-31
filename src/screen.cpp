#include "screen.h"
#include <thread>


void Screen::Run( Controller const &controller,
                    Renderer &renderer,
                    std::size_t target_frame_duration,
                    Status &status,
                    EScreen desiredScreen )
{
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  Uint32 delay;

  while( status.running && status.screen == desiredScreen )
  {
    
    frame_start = SDL_GetTicks();

    // INPUTE UPDATE RENDER
    controller.handleEvent( status.running, _buttons, _clickedButtonType );
    update( status );
    childRender( renderer );// renderer.renderAll( _buttons, _title );

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
  _title = TextDisplay( "Z", { 300, 200 } );
  _screenWidth = screen_width;
  _screenHeight= screen_height;
  makeButtons();
}

LoadingScreen::LoadingScreen( std::size_t screen_width, std::size_t screen_height )
{
  _msg = TextDisplay( "loading", { 300, 200 } );
  _screenWidth = screen_width;
  _screenHeight= screen_height;
  makeButtons();
}

CreditScreen::CreditScreen( std::size_t screen_width, std::size_t screen_height, TextDisplay msg )
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
  renderer.renderAll( _buttons, _title );
}

void LoadingScreen::childRender( Renderer &renderer ){
  renderer.renderAll( _buttons, _msg );
}

void CreditScreen::childRender( Renderer &renderer ){
  renderer.renderAll( _buttons, _msg );
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
  switch( _clickedButtonType )
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
  _clickedButtonType = ButtonType::BNULL;
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