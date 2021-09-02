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
  void renderRect( SDL_Renderer* rend, SDL_Point pos, int width, int height, SDL_Color color );
  void renderRect( SDL_Renderer* rend, SDL_Point pos, SDL_Color color );
  void renderRectOutline( SDL_Renderer* rend, SDL_Point pos, int width, int height );
  void setTextureWH( int width, int height );
  void setImgPath(std::string imgPath);
  void setPosition(int x, int y);

  int getWidth();
  int getHeight();
  SDL_Point getPosition();
  bool needsLoading();
  bool hasImagePath();
  std::string getImagePath();

 private:
  SDL_Texture* _texture;
  std::string _imgPath;
  SDL_Point _position {0, 0};
  int _width;
  int _height;

};

/********* 2. Generic Text Display  *********/

class TextDisplay
{
 public:
  TextDisplay(); // constructor
  TextDisplay( std::string text, SDL_Point position);
  ~TextDisplay(){} // destructor
  std::string getText();

  void setText( std::string text );
  void setTextureWH( int width, int height );
  void setTextHeight( int height);
  void setPosition( SDL_Point position);

  void render( SDL_Renderer* rend );
  void render( SDL_Renderer* rend, SDL_Point position);

 private:
  int determineWidth( std::string text );
  TTF_Font* _font = nullptr;
  LTexture _LTexture;
  SDL_Point _position;
  SDL_Color _textColor { 255, 255, 255 };
  int _textHeight { 40 };
  std::string _text;
};

/********* 3. Game Stats Display  *********/

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
  // GameStatsDisplays _gameStatsDisplays; // gameStatsTextures
  TextDisplay _clockD;

  TextDisplay _selectedBuildingD;

  TextDisplay _playerHealthD;
  TextDisplay _playerFoodD;
  TextDisplay _playerMaterialsD;
  
  TextDisplay _homeHealthD;
  TextDisplay _homeFoodD;
  TextDisplay _homeMaterialsD;

  TextDisplay _buildingFoodD;
  TextDisplay _buildingMaterialsD;
  TextDisplay _buildingScoutedD;
  TextDisplay _buildingDangerD;

  LTexture _leftPanel;
  LTexture _topPanel;
  TextDisplay _bottomPanel;
};

/********* 4. Panel Struct && Initialization Dimensions *********/

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
  
  const std::size_t kFPS{ 30 };
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


#endif