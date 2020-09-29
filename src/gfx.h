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
  void renderRect( SDL_Renderer* rend, SDL_Point pos );
  void renderRectOutline( SDL_Renderer* rend, SDL_Point pos, int width, int height );
  void setTextureWH( int width, int height );
  int getWidth();
  int getHeight();

 private:
  SDL_Texture* _texture;
  int _width;
  int _height;

};

/********* 2. Overlay  *********/

struct GameStatsTextures // overlay textures
{
  LTexture _clockT;

  LTexture _selectedBuildingT;

  LTexture _playerHealthT;
  LTexture _playerFoodT;
  LTexture _playerMaterialsT;
  
  LTexture _homeHealthT;
  LTexture _homeFoodT;
  LTexture _homeMaterialsT;

  LTexture _buildingFoodT;
  LTexture _buildingMaterialsT;
  LTexture _buildingScoutedT;
  LTexture _buildingDangerT;

  LTexture _leftPanel;
  LTexture _bottomPanel;
};

struct GameStatsTexts
{
  std::string _clockText;
  std::string _buildingText;
  
  std::string _playerHealthText;
  std::string _playerFoodText;
  std::string _playerMaterialsText;

  std::string _homeHealthText;
  std::string _homeFoodText;
  std::string _homeMaterialsText;

  std::string _buildingFoodText;
  std::string _buildingMaterialsText;
  std::string _buildingScoutedText;
  std::string _buildingDangerText;

  // bottom panel
  std::string _actionResultText;
};

int determineWidth( std::string& text );

class GameStatsDisplay
{
 public:
  GameStatsDisplay(); // constructor
  ~GameStatsDisplay(){} // destructor
  
  void render( SDL_Renderer* rend );
  void setLeftPanelWH( int width, int height );
  void setBottomPanelWH( int width, int height );

  GameStatsTexts _gameStatsTexts;

 private:
  TTF_Font* _font = nullptr;
  GameStatsTextures _gameStatsTextures; // gameStatsTextures
};

#endif