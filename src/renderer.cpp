#include "renderer.h"
#include <alsa/asoundlib.h>
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
  if( SDL_Init( SDL_INIT_VIDEO || SDL_INIT_AUDIO) < 0 ) std::cerr << "SDL could not initialize. SDL Error: \n" << SDL_GetError() << std::endl;

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

  // check for an audio card
  int cardNum = -1;
  int nextCard = snd_card_next(&cardNum);
  int hasCard = snd_card_load(nextCard);
  if ( !hasCard ) std::cout << "No audio card detected.\n";

  //Initialize SDL_mixer
  if ( hasCard && Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ) {
    std::cerr << "Error: " << Mix_GetError() << std::endl;
  }
  else{
    std::string musicPath = "../sound/upside-down-grin2.ogg";
    std::string clickSoundPath = "../sound/click.wav";
    
    _gMusic = Mix_LoadMUS( musicPath.c_str() );
    if ( _gMusic == NULL ) std::cerr << musicPath << " : Failed to load. "<< Mix_GetError() << std::endl;

    _clickSound = Mix_LoadWAV( clickSoundPath.c_str() );
    if ( _gMusic == NULL ) std::cerr << clickSoundPath << " : Failed to load. "<< Mix_GetError() << std::endl;
  }

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

  // free audio
  Mix_FreeChunk( _clickSound );
  Mix_FreeMusic( _gMusic );
  _clickSound = NULL;
  _gMusic = NULL;

  // free ALSA memory
  snd_config_update_free_global();
}

void Renderer::playMusic(){
  if ( Mix_PlayingMusic() == 0) Mix_PlayMusic( _gMusic, -1 );
}

void Renderer::playClickSound(){
   Mix_PlayChannel(-1, _clickSound, 0 );
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

void Renderer::renderAll( std::vector<Button>& buttons, TextDisplay &textDisplay, LTexture &img )
{

  // clear the screen
  SDL_SetRenderDrawColor( _renderer, 0x00, 0x00, 0x00, 0x00 );
  SDL_RenderClear( _renderer );

  // render background image
  if ( img.hasImagePath() ){
    if (img.needsLoading() ) img.loadTextureFromBMP( _renderer, img.getImagePath() );
    img.render( _renderer, img.getPosition() , img.getWidth(), img.getHeight() );
  }

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

  textDisplay.render( _renderer );

  // update the screen
  SDL_RenderPresent( _renderer );
}