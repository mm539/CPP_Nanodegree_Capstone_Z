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

void LTexture::renderRect( SDL_Renderer* rend, SDL_Point pos, int width, int height )
{
  SDL_Rect fillRect = { pos.x, pos.y, width, height };
  SDL_SetRenderDrawColor( rend, 0x40, 0x31, 0x0A ,0xFF );
  SDL_RenderFillRect( rend, &fillRect );
}

int LTexture::getWidth()
{
	return _width;
}

int LTexture::getHeight()
{
	return _height;
}


/********* 2. Overlay  *********/

Overlay::Overlay( SDL_Point pos, int width, int height )
{
  _position = pos;
  _width = width;
  _height = height;

  _font = TTF_OpenFont( "../fonts/black-caps-font/BlackCapsRegular-wgA2.ttf", 16 );
  if( _font == nullptr  ) std::cout<< "Failed to load _font. SDL_ttf Error:\n" << TTF_GetError() << std::endl;
 }

Overlay::~Overlay(){}

void Overlay::render( SDL_Renderer* rend )
{
  // 1. render left panel
  _leftPanel.renderRect( rend, _position, _width, _height );

  // 2. render game and base info
  SDL_Color textColor = { 255, 255, 255 };

  _oTextures._clockT.loadFromRenderedText( rend, _texts._clockText, textColor, _font );
  _oTextures._clockT.render( rend, { 400 , 5 }, 210, 50 );

  _oTextures._homeHealthT.loadFromRenderedText( rend, _texts._homeHealthText, textColor, _font );
  _oTextures._homeHealthT.render( rend, { 250 , 60 }, 250, 40 );

  _oTextures._homeFoodT.loadFromRenderedText( rend, _texts._homeFoodText, textColor, _font );
  _oTextures._homeFoodT.render( rend, { 530 , 60 }, 150, 40 );

  _oTextures._homeMaterialsT.loadFromRenderedText( rend, _texts._homeMaterialsText, textColor, _font );
  _oTextures._homeMaterialsT.render( rend, { 740 , 60 }, 180, 40 );

  // 3. render player info
  textColor = { 0xa6, 0xa5, 0xa2 };

  _oTextures._playerHealthT.loadFromRenderedText( rend, _texts._playerHealthText, textColor, _font );
  _oTextures._playerHealthT.render( rend, { 20 , 100 }, 180, 40 );

  _oTextures._playerFoodT.loadFromRenderedText( rend, _texts._playerFoodText, textColor, _font );
  _oTextures._playerFoodT.render( rend, { 20 , 160 }, 180, 40 );

  _oTextures._playerMaterialsT.loadFromRenderedText( rend, _texts._playerMaterialsText, textColor, _font );
  _oTextures._playerMaterialsT.render( rend, { 20 , 220 }, 180, 40 );

  // 4. render bottom panel ( result of a button click )
  textColor = { 0xff, 0xcb, 0x3d };
  int stringLength = strlen( _texts._actionResultText.c_str() );
  _bottomPanel.loadFromRenderedText( rend, _texts._actionResultText, textColor, _font );
  _bottomPanel.render( rend, { 270, 820 }, stringLength * 15, 40);

  // 5. render building info
  textColor = { 0xc4, 0x66, 0x66 };
  _oTextures._buildingScoutedT.loadFromRenderedText( rend, _texts._buildingScoutedText, textColor, _font );
  _oTextures._buildingScoutedT.render( rend, { 20, 700 }, 180, 40 );

  _oTextures._buildingDangerT.loadFromRenderedText( rend, _texts._buildingDangerText, textColor, _font );
  _oTextures._buildingDangerT.render( rend, { 20, 760 }, 180, 40 );

  _oTextures._buildingFoodT.loadFromRenderedText( rend, _texts._buildingFoodText, textColor, _font );
  _oTextures._buildingFoodT.render( rend, { 20, 800 }, 180, 40 );

  _oTextures._buildingMaterialsT.loadFromRenderedText( rend, _texts._buildingMaterialsText, textColor, _font );
  _oTextures._buildingMaterialsT.render( rend, { 20, 840 }, 180, 40 );

}