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
  void setBlendModeForDraw( SDL_Renderer *rend, SDL_BlendMode blending ); 
  void setAlpha( Uint8 alpha );
  void render( SDL_Renderer* rend, SDL_Point pos, int width, int height );
  void renderRect( SDL_Renderer* rend, SDL_Point pos, SDL_Color color );
  void renderRectOutline( SDL_Renderer* rend, SDL_Point pos, int width, int height );
  void setTextureWH( int width, int height );
  int getWidth();
  int getHeight();

 private:
  SDL_Texture* _texture;
  int _width;
  int _height;

};

/********* 2. Game Stats Display  *********/

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
  LTexture _topPanel;
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
  void setTopPanelWH( int width, int height );
  void setBottomPanelWH( int width, int height );

  GameStatsTexts _gameStatsTexts;

 private:
  TTF_Font* _font = nullptr;
  GameStatsTextures _gameStatsTextures; // gameStatsTextures
};

/********* 3. Panel Struct && Initialization Dimensions *********/

struct PanelPD // Panel Position Dimensions
{
  PanelPD(){}
  PanelPD( int x, int y, int w, int h ): 
         _x( x ), _y( y ), _w( w ), _h( h ) {}
  void setPanelPD( int x, int y, int w, int h );
  int _x { 0 };
  int _y { 0 };
  int _w { 0 };
  int _h { 0 };
};

struct InitDimen
{
  InitDimen();
  
  const std::size_t kFPS{ 60 };
  const std::size_t kMsPerFrame{ 1000 / kFPS };
  const int kSCREEN_WIDTH{ 960 };
  const int kSCREEN_HEIGHT{ 880};
  const int kGRID_WIDTH{ 90 }; // 720 360 180 90
  const int kGRID_HEIGHT{ 85 }; // 680 340 170 85

  // 
  int leftPanelWidth = 240;
  int topPanelHeight = 120;
  int bottomPanelHeight = 80; 

  // x, y, w, h
  PanelPD leftPanelPD;
  PanelPD topPanelPD;
  PanelPD bottomPanelPD;
  PanelPD mapPanelPD;
};

/********* 4. Generic Text Display  *********/

class TextDisplay
{
 public:
  TextDisplay(); // constructor
  TextDisplay( std::string text, SDL_Point position);
  ~TextDisplay(){} // destructor
  void setText( std::string text );
  void render( SDL_Renderer* rend );

 private:
  TTF_Font* _font = nullptr;
  LTexture _LTexture;
  SDL_Point _position;
  std::string _text;
};

#endif