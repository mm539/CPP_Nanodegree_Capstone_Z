#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer( const int screen_width,
                    const int screen_height,
                    const int grid_width,
                    const int grid_height )
                : screen_width( screen_width ),
                  screen_height( screen_height ),
                  grid_width( grid_width ),
                  grid_height( grid_height)
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

void Renderer::renderAll( std::vector<std::shared_ptr<Building>>& buildings, GameStatsDisplay& stats, std::vector<Button>& buttons, Player& player )
{
  // clear the screen
  SDL_SetRenderDrawColor( _renderer, 0x00, 0x00, 0x00, 0x00 );
  SDL_RenderClear( _renderer );

  // render the game stats
  stats.render( _renderer );

  // render buildings on map
  for( int i = 0; i < buildings.size(); i++ )
  {
    buildings[ i ]->render( _renderer );
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

void Renderer::renderAll( std::vector<Button>& buttons)
{
  // clear the screen
  SDL_SetRenderDrawColor( _renderer, 0x00, 0x00, 0x00, 0x00 );
  SDL_RenderClear( _renderer );

   // render buttons
  int buttonCounter = 0;
  int xPos = screen_width / 2 - 65;
  int yPos = screen_height / 3;
  int spacer = 75;

  for( int j = 0; j < buttons.size(); j++ )
  {
    if( buttons[ j ].getButtonState() != ButtonState::BUTTON_INVISIBLE )
    {
      buttons[ j ].setPosition( xPos, yPos + spacer * buttonCounter );
      buttons[ j ].render( _renderer );
      buttonCounter++;
    }
  }

  // update the screen
  SDL_RenderPresent( _renderer );
}