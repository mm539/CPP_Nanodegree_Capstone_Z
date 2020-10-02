#include "gfx.h"

/********* 1. LTexture  *********/

LTexture::LTexture()
{
  _texture = nullptr;
  _width = 0;
  _height = 0;
}

LTexture::~LTexture()
{
  free();
}

void LTexture::loadTextureFromBMP( SDL_Renderer* rend, std::string path )
{
  free();
  SDL_Texture* newTexture = nullptr;
  SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );

  if( loadedSurface == nullptr ) std::cout << "Unable to load BMP. SDL Error:\n" << SDL_GetError() << std::endl;
  else
  {
    // can do color keying here if necessary.

    // create texture from surface
    newTexture = SDL_CreateTextureFromSurface( rend, loadedSurface );

    if( newTexture == nullptr ) std::cout << "Unable to create texture. SDL Error:\n" << SDL_GetError() << std::endl;
    else
    {
      _width = loadedSurface->w;
      _height = loadedSurface->h;
    }
    SDL_FreeSurface( loadedSurface );
  }
  _texture = newTexture;
}

void LTexture::loadFromRenderedText( SDL_Renderer* rend, std::string text, SDL_Color textColor, TTF_Font* font )
{
  free();

  SDL_Surface* textSurface = TTF_RenderText_Solid( font, text.c_str(), textColor );

  if( textSurface == nullptr ) std::cerr << "Unable to render text surface! SDL_ttf Error:\n" << TTF_GetError() << std::endl;
  else
  {
    _texture = SDL_CreateTextureFromSurface( rend, textSurface );
    
    if( _texture == nullptr ) std::cerr << "Unable to create texture from rendered text! SDL Error: \n" << SDL_GetError();
    else
    {
      _width = textSurface->w;
      _height = textSurface->h;
    }
    SDL_FreeSurface( textSurface );
  }
  
}

void LTexture::free()
{
  if( _texture != nullptr )
  {
    SDL_DestroyTexture( _texture );
    _texture = nullptr;
    _width = 0;
    _height = 0;
  }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
  SDL_SetTextureColorMod( _texture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
  SDL_SetTextureBlendMode( _texture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
  SDL_SetTextureAlphaMod( _texture, alpha );
}

void LTexture::render( SDL_Renderer* rend, SDL_Point pos, int width, int height )
{
  SDL_Rect src = { 0, 0, _width, _height };
  SDL_Rect dest = { pos.x, pos.y, width, height };
  SDL_RenderCopy( rend, _texture, &src, &dest );
}

void LTexture::renderRect( SDL_Renderer* rend, SDL_Point pos )
{
  SDL_Rect fillRect = { pos.x, pos.y, _width, _height };
  SDL_SetRenderDrawColor( rend, 0x40, 0x31, 0x0A ,0xFF );
  SDL_RenderFillRect( rend, &fillRect );
}

void LTexture::renderRectOutline( SDL_Renderer* rend, SDL_Point pos, int width, int height )
{
  SDL_Rect outlineRect = { pos.x, pos.y, width, height };
  SDL_SetRenderDrawColor( rend, 0xae, 0x31, 0xd4 ,0xFF ); //#ae31d4
  SDL_RenderDrawRect( rend, &outlineRect );
}

int LTexture::getWidth()
{
	return _width;
}

int LTexture::getHeight()
{
	return _height;
}

void LTexture::setTextureWH( int width, int height )
{
  _width = width;
  _height = height;
}

/********* 2. GameStatsDisplay  *********/

GameStatsDisplay::GameStatsDisplay()
{
  _font = TTF_OpenFont( "../fonts/black-caps-font/BlackCapsRegular-wgA2.ttf", 16 );
  if( _font == nullptr  ) std::cout<< "Failed to load _font. SDL_ttf Error:\n" << TTF_GetError() << std::endl;
 }

int determineWidth( std::string &text )
{
  int space = 12; //space per character
  return space * strlen( text.c_str() );
}

void GameStatsDisplay::render( SDL_Renderer* rend )
{
  // 1. render left panel
  _gameStatsTextures._leftPanel.renderRect( rend, {0, 0} );

  // 2. render game and base info
  SDL_Color textColor = { 255, 255, 255 };

  _gameStatsTextures._clockT.loadFromRenderedText( rend, _gameStatsTexts._clockText, textColor, _font );
  _gameStatsTextures._clockT.render( rend, { 400 , 5 }, determineWidth( _gameStatsTexts._clockText ), 50 );

  _gameStatsTextures._homeHealthT.loadFromRenderedText( rend, _gameStatsTexts._homeHealthText, textColor, _font );
  _gameStatsTextures._homeHealthT.render( rend, { 250 , 60 }, determineWidth( _gameStatsTexts._homeHealthText ), 40 );

  _gameStatsTextures._homeFoodT.loadFromRenderedText( rend, _gameStatsTexts._homeFoodText, textColor, _font );
  _gameStatsTextures._homeFoodT.render( rend, { 530 , 60 }, determineWidth( _gameStatsTexts._homeFoodText ), 40 );

  _gameStatsTextures._homeMaterialsT.loadFromRenderedText( rend, _gameStatsTexts._homeMaterialsText, textColor, _font );
  _gameStatsTextures._homeMaterialsT.render( rend, { 740 , 60 }, determineWidth( _gameStatsTexts._homeMaterialsText ), 40 );

  // 3. render player info
  textColor = { 0xa6, 0xa5, 0xa2 };

  _gameStatsTextures._playerHealthT.loadFromRenderedText( rend, _gameStatsTexts._playerHealthText, textColor, _font );
  _gameStatsTextures._playerHealthT.render( rend, { 20 , 100 }, determineWidth( _gameStatsTexts._playerHealthText ), 40 );

  _gameStatsTextures._playerFoodT.loadFromRenderedText( rend, _gameStatsTexts._playerFoodText, textColor, _font );
  _gameStatsTextures._playerFoodT.render( rend, { 20 , 160 }, determineWidth( _gameStatsTexts._playerFoodText ), 40 );

  _gameStatsTextures._playerMaterialsT.loadFromRenderedText( rend, _gameStatsTexts._playerMaterialsText, textColor, _font );
  _gameStatsTextures._playerMaterialsT.render( rend, { 20 , 220 }, determineWidth( _gameStatsTexts._playerMaterialsText ), 40 );

  // 4. render bottom panel ( result of a button click )
  textColor = { 0xff, 0xcb, 0x3d };
  _gameStatsTextures._bottomPanel.loadFromRenderedText( rend, _gameStatsTexts._actionResultText, textColor, _font );
  _gameStatsTextures._bottomPanel.render( rend, { 270, 820 }, determineWidth( _gameStatsTexts._actionResultText ), 40);

  // 5. render building info
  textColor = { 0xc4, 0x66, 0x66 };
  _gameStatsTextures._buildingScoutedT.loadFromRenderedText( rend, _gameStatsTexts._buildingScoutedText, textColor, _font );
  _gameStatsTextures._buildingScoutedT.render( rend, { 20, 700 }, determineWidth( _gameStatsTexts._buildingScoutedText ), 40 );

  _gameStatsTextures._buildingDangerT.loadFromRenderedText( rend, _gameStatsTexts._buildingDangerText, textColor, _font );
  _gameStatsTextures._buildingDangerT.render( rend, { 20, 760 }, determineWidth( _gameStatsTexts._buildingDangerText ), 40 );

  _gameStatsTextures._buildingFoodT.loadFromRenderedText( rend, _gameStatsTexts._buildingFoodText, textColor, _font );
  _gameStatsTextures._buildingFoodT.render( rend, { 20, 800 }, determineWidth( _gameStatsTexts._buildingFoodText ), 40 );

  _gameStatsTextures._buildingMaterialsT.loadFromRenderedText( rend, _gameStatsTexts._buildingMaterialsText, textColor, _font );
  _gameStatsTextures._buildingMaterialsT.render( rend, { 20, 840 }, determineWidth( _gameStatsTexts._buildingMaterialsText ), 40 );

}

void GameStatsDisplay::setLeftPanelWH( int width, int height )
{
  _gameStatsTextures._leftPanel.setTextureWH( width, height );
}

void GameStatsDisplay::setBottomPanelWH( int width, int height )
{
  _gameStatsTextures._bottomPanel.setTextureWH( width, height );
}

void GameStatsDisplay::setTopPanelWH( int width, int height )
{
  _gameStatsTextures._topPanel.setTextureWH( width, height );
}

/********* 3. Panel  *********/

void PanelPD::setPanelPD( int x, int y, int w, int h )
{
  int _x { x };
  int _y { y };
  int _w { w };
  int _h { h };
}

/********* 4. Initialization dimensions  *********/

InitDimen::InitDimen()
{
    // x, y, w, h
  leftPanelPD = PanelPD( 0, 0, leftPanelWidth, kSCREEN_HEIGHT );
  topPanelPD = PanelPD( leftPanelWidth, 0, 
                       kSCREEN_WIDTH - leftPanelWidth, topPanelHeight );
  bottomPanelPD = PanelPD( leftPanelWidth, kSCREEN_HEIGHT - bottomPanelHeight,
                       kSCREEN_WIDTH - leftPanelWidth, bottomPanelHeight );
  mapPanelPD = PanelPD( leftPanelWidth, topPanelHeight,
          kSCREEN_WIDTH - leftPanelWidth, kSCREEN_HEIGHT - topPanelHeight );
          /* why does the code below produce a segmentation fault??
  leftPanelPD.setPanelPD( 0, 0, leftPanelWidth, kSCREEN_HEIGHT );
  topPanelPD.setPanelPD( leftPanelWidth, 0, 
                       kSCREEN_WIDTH - leftPanelWidth, topPanelHeight );
  bottomPanelPD.setPanelPD( leftPanelWidth, kSCREEN_HEIGHT - bottomPanelHeight,
                       kSCREEN_WIDTH - leftPanelWidth, bottomPanelHeight );
  mapPanelPD.setPanelPD( leftPanelWidth, topPanelHeight,
          kSCREEN_WIDTH - leftPanelWidth, kSCREEN_HEIGHT - topPanelHeight );*/
}