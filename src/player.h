#ifndef PLAYER_H
#define PLAYER_H

#include "gfx.h"

class Player
{
 public:
  // constructor
  Player();

  // rendering
  void render( SDL_Renderer* rend );

  int getLocationID();
  SDL_Point getPosition();

  void setLocationID( int x );
  void setPosition( int x, int y );

  // functions for getting and setting Health, Food, Materials
  void changePlayerHealth( int x );
  void changePlayerFood( int x );
  void changePlayerMaterials( int x );

  void changeHomeHealth( int x );
  void changeHomeFood( int x );
  void changeHomeMaterials( int x );

  int getPlayerHealth();
  int getPlayerFood();
  int getPlayerMaterials();

  int getHomeHealth();
  int getHomeFood();
  int getHomeMaterials();
  
 private:
  // player token variables
  LTexture _LTexture;
  int _width;
  int _height;
  std::string _imgPath { "../img/player_token.bmp" };
  // std::string _portraitPath { "" };

  // gameplay mechanics
  SDL_Point _position;
  int _locationID; // building id
  int _home; // building id of home building

  // player stats
  int _health { 100 };
  int _materials { 0 };
  int _food { 0 };

  // home stats
  int _homeMaterials { 25 };
  int _homeHealth { 100 }; // defense
  int _homeFood { 20 };

  
};

#endif