#include "player.h"

Player::Player() : _width( 32 ), _height( 32 ) {}

void Player::setLocationID( int x )
{
  _locationID = x;
}

int Player::getLocationID()
{
  return _locationID;
}

void Player::setPosition( int x, int y )
{
  _position.x = x;
  _position.y = y;
}

SDL_Point Player::getPosition()
{
  return _position;
}

void Player::render( SDL_Renderer* rend )
{
  SDL_Point tempPos = _position;
  tempPos.x = _position.x + 32;
  tempPos.y = _position.y + 32;
  _LTexture.loadTextureFromBMP( rend, _imgPath );
  _LTexture.render( rend , tempPos, _width, _height );
}


void Player::changePlayerHealth( int x )
{
  if( _health + x > 100 ) _health = 100;
  else _health = _health + x;
}
void Player::changePlayerFood( int x )
{
  _food = _food + x;
}
void Player::changePlayerMaterials( int x )
{
  _materials = _materials + x;
}

void Player::changeHomeHealth( int x )
{
  _homeHealth = _homeHealth + x;
}
void Player::changeHomeFood( int x )
{
  _homeFood = _homeFood + x;
}
void Player::changeHomeMaterials( int x )
{
  _homeMaterials = _homeMaterials + x;
}


int Player::getPlayerHealth()
{
  return _health;
}
int Player::getPlayerFood()
{
  return _food;
}
int Player::getPlayerMaterials()
{
  return _materials;
}

int Player::getHomeHealth()
{
  return _homeHealth;
}
int Player::getHomeFood()
{
  return _homeFood;
}
int Player::getHomeMaterials()
{
  return _homeMaterials;
}
