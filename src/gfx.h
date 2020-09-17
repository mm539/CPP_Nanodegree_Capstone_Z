#ifndef GFX_H
#define GFX_H
#include "SDL.h"
#include <iostream>
#include <string>
#include "SDL_image.h"
#include "SDL_ttf.h"

/********* 1. LTexture  *********/

class LTexture
{
 public:
  LTexture();
  ~LTexture();
  void loadTextureFromBMP( SDL_Renderer* rend, std::string path ); // loads image at specified path
  void loadFromRenderedText( SDL_Renderer* rend, std::string text, SDL_Color textColor, TTF_Font* _font );
  void free(); // deallocates memory
  void setColor( Uint8 red, Uint8 green, Uint8 blue ); // texture rgb
  void setBlendMode( SDL_BlendMode blending ); // set blending
  void setAlpha( Uint8 alpha );
  void render( SDL_Renderer* rend, SDL_Point pos, int width, int height );
  void renderRect( SDL_Renderer* rend, SDL_Point pos, int width, int height );
  int getWidth();
  int getHeight();

 private:
  SDL_Texture* _texture;
  int _width;
  int _height;

};

/********* 2. Overlay  *********/

struct OTextures // overlay textures
{
  LTexture _clockT;

  LTexture _selectedBuildingT;

  LTexture _playerHealthT;
  LTexture _playerFoodT;
  LTexture _playerMaterialsT;
  
  LTexture _homeHealthT;
  LTexture _homeFoodT;
  LTexture _homeMaterialsT;
};

struct OverlayTexts
{
  std::string _clockText;
  std::string _buildingText;
  
  std::string _playerHealthText;
  std::string _playerFoodText;
  std::string _playerMaterialsText;

  std::string _homeHealthText;
  std::string _homeFoodText;
  std::string _homeMaterialsText;

  // bottom panel
  std::string _actionResultText;
};

class Overlay
{
 public:
  // constructors
  Overlay(){}
  Overlay( SDL_Point pos, int width, int height );

  // destructor
  ~Overlay();
  
  void render( SDL_Renderer* rend );
  OverlayTexts _texts;

 private:
  TTF_Font* _font = nullptr;
  LTexture _leftPanel;
  OTextures _oTextures; // overlay textures
  LTexture _bottomPanel;

  
  SDL_Point _position;
  int _width;
  int _height;
};

#endif