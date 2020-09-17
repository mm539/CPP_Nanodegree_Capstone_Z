#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer( const std::size_t screen_width,
                    const std::size_t screen_height,
                    const std::size_t grid_width,
                    const std::size_t grid_height,
                    const std::size_t left_panel_width,
                    const std::size_t top_panel_height,
                    const std::size_t bottom_panel_height )
                : screen_width( screen_width ),
                  screen_height( screen_height ),
                  grid_width( grid_width ),
                  grid_height( grid_height),
                  left_panel_width( left_panel_width),
                  top_panel_height( top_panel_height ),
                  bottom_panel_height( bottom_panel_height )
{
  // initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) std::cerr << "SDL could not initialize. SDL Error: \n" << SDL_GetError() << std::endl;

  // create the window
  _window = SDL_CreateWindow("Z", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
  if( _window == nullptr ) std::cerr << "Window could not be created. SDL Error: \n" << SDL_GetError() << std::endl;

  // create the renderer
  _renderer = SDL_CreateRenderer( _window, -1, SDL_RENDERER_ACCELERATED );
  if( _renderer == nullptr ) std::cerr << "Renderer could not be created. SDL Error: \n" << SDL_GetError() << std::endl;

  // initialize the sdl2_image library for PNG loading
  int imgFlags = IMG_INIT_PNG;
  if( !( IMG_Init( imgFlags ) & imgFlags ) ) std::cerr << "SDL_image could not initialize! SDL_image Error:\n" << IMG_GetError() << std::endl;

  // initialize the sdl2_ttf library for fonts
  if( TTF_Init() == -1 ) std::cerr << "SDL_ttf could not initialize! SDL_ttf Error:\n" << TTF_GetError() << std::endl;
}

Renderer::~Renderer()
{
  // destroy window
  SDL_DestroyRenderer( _renderer );
  SDL_DestroyWindow( _window );
  _renderer = nullptr;
  _window = nullptr;

  // quit SDL subsystems
  SDL_Quit();
  IMG_Quit();
  TTF_Quit();
}

void Renderer::renderAll( std::vector<Building>& buildings, Overlay& overlay, std::vector<Button>& buttons, Player& player )
{
  // clear the screen
  SDL_SetRenderDrawColor( _renderer, 0x00, 0x00, 0x00, 0x00 );
  SDL_RenderClear( _renderer );

  // render the overlay
  overlay.render( _renderer );

  // render buildings on map
  for( int i = 0; i < buildings.size(); i++ )
  {
    buildings[ i ].render( _renderer );
  }

  // render buttons
  int buttonCounter = 0;
  int spacer = 340;
  for( int j = 0; j < buttons.size(); j++ )
  {
    if( buttons[ j ].getButtonState() != ButtonState::BUTTON_INVISIBLE )
    {
      buttons[ j ].setPosition( 50, spacer + 75 * buttonCounter );
      buttons[ j ].render( _renderer );
      buttonCounter++;
    }
    
  }

  // render the player token
  player.render( _renderer );


  // update the screen
  SDL_RenderPresent( _renderer );
}