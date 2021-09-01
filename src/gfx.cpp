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

void LTexture::setBlendModeForDraw( SDL_Renderer *rend, SDL_BlendMode blending )
{
  SDL_SetRenderDrawBlendMode( rend, blending );
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

// this function was overloaded because there was a bug that I couldn't figure out with clicking on the building; passing width and height is a workaround.
void LTexture::renderRect( SDL_Renderer* rend, SDL_Point pos, int width, int height, SDL_Color color )
{
  SDL_Rect fillRect = { pos.x, pos.y, width, height };
  SDL_SetRenderDrawColor( rend, color.r, color.g, color.b, color.a );
  SDL_RenderFillRect( rend, &fillRect );
}

void LTexture::renderRect( SDL_Renderer* rend, SDL_Point pos, SDL_Color color )
{
  SDL_Rect fillRect = { pos.x, pos.y, _width, _height };
  SDL_SetRenderDrawColor( rend, color.r, color.g, color.b, color.a );
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

bool LTexture::needsLoading(){
  return _texture == nullptr;
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
  _leftPanel.renderRect( rend, {0, 0}, { 0x40, 0x31, 0x0A ,0xFF } );

  // 2. render game and base info
  SDL_Color textColor = { 255, 255, 255 };

  _clockD.setText( _gameStatsTexts._clockText );
  _clockD.render( rend, { 400 , 5 } );

  _homeHealthD.setText( _gameStatsTexts._homeHealthText );
  _homeHealthD.render( rend, { 250 , 60 } );

  _homeFoodD.setText( _gameStatsTexts._homeFoodText );
  _homeFoodD.render( rend, { 530 , 60 } );

  _homeMaterialsD.setText( _gameStatsTexts._homeMaterialsText );
  _homeMaterialsD.render( rend, { 740 , 60 } );

  // 3. render player info
  textColor = { 0xa6, 0xa5, 0xa2 };

  _playerHealthD.setText( _gameStatsTexts._playerHealthText );
  _playerHealthD.render( rend, { 20 , 100 } );

  _playerFoodD.setText( _gameStatsTexts._playerFoodText );
  _playerFoodD.render( rend, { 20 , 160 } );

  _playerMaterialsD.setText( _gameStatsTexts._playerMaterialsText );
  _playerMaterialsD.render( rend, { 20 , 220 } );

  // 4. render bottom panel ( result of a button click )
  textColor = { 0xff, 0xcb, 0x3d };
  _bottomPanel.setText( _gameStatsTexts._actionResultText );
  _bottomPanel.render( rend, { 270, 820 } );

  // 5. render building info
  textColor = { 0xc4, 0x66, 0x66 };
  _buildingScoutedD.setText( _gameStatsTexts._buildingScoutedText );
  _buildingScoutedD.render( rend, { 20, 700 } );

  _buildingDangerD.setText( _gameStatsTexts._buildingDangerText );
  _buildingDangerD.render( rend, { 20, 760 } );

  _buildingFoodD.setText( _gameStatsTexts._buildingFoodText );
  _buildingFoodD.render( rend, { 20, 800 } );

  _buildingMaterialsD.setText( _gameStatsTexts._buildingMaterialsText );
  _buildingMaterialsD.render( rend, { 20, 840 } );

}

void GameStatsDisplay::setLeftPanelWH( int width, int height )
{
  _leftPanel.setTextureWH( width, height );
}

void GameStatsDisplay::setBottomPanelWH( int width, int height )
{
  _bottomPanel.setTextureWH( width, height );
}

void GameStatsDisplay::setTopPanelWH( int width, int height )
{
  _topPanel.setTextureWH( width, height );
}

/********* 3. Panel Struct && Initialization dimensions  *********/

void PanelPD::setPanelPD( int x, int y, int w, int h )
{
  int _x { x };
  int _y { y };
  int _w { w };
  int _h { h };
}

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

/********* 4. TextDisplay  *********/

TextDisplay::TextDisplay( )
{
  _font = TTF_OpenFont( "../fonts/black-caps-font/BlackCapsRegular-wgA2.ttf", 16 );
  if( _font == nullptr  ) std::cout<< "Failed to load _font. SDL_ttf Error:\n" << TTF_GetError() << std::endl;
 }

TextDisplay::TextDisplay( std::string text, SDL_Point position )
{
  _font = TTF_OpenFont( "../fonts/black-caps-font/BlackCapsRegular-wgA2.ttf", 16 );
  if( _font == nullptr  ) std::cout<< "Failed to load _font. SDL_ttf Error:\n" << TTF_GetError() << std::endl;

  _text = text;
  _position = position;
 }

void TextDisplay::setText( std::string text )
{
  _text = text;
}

void TextDisplay::setTextureWH( int width, int height )
{
  _LTexture.setTextureWH( width, height );
}

void TextDisplay::render( SDL_Renderer* rend )
{
  _LTexture.loadFromRenderedText( rend, _text, _textColor, _font );
  _LTexture.render( rend, _position, determineWidth( _text ), _textHeight );
}

void TextDisplay::render( SDL_Renderer* rend, SDL_Point position )
{
  _LTexture.loadFromRenderedText( rend, _text, _textColor, _font );
  _LTexture.render( rend, position, determineWidth( _text ), _textHeight );
}