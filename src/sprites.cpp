#include "sprites.h"

/*
Sprite
*/

bool Sprite::handleEvent( SDL_Event* e )
{
  int x, y;
  bool inside = true;
  SDL_GetMouseState( &x, &y );

  if( x < _position.x ) inside = false;
  else if( x > _position.x + _width ) inside = false;
  else if( y < _position.y ) inside = false;
  else if( y > _position.y + _height ) inside = false;

  if( !inside )
  {
    _mouseState = STATE_MOUSE_OUT;
    _mousedDown = false;
  } 
  else
  {
    switch ( e->type )
    {
      case SDL_MOUSEMOTION:
      _mouseState = STATE_MOUSE_OVER;
      break;

      case SDL_MOUSEBUTTONDOWN:
      _mouseState = STATE_MOUSE_DOWN;
      _mousedDown = true;
      break;

      case SDL_MOUSEBUTTONUP:
      _mouseState = STATE_MOUSE_OVER;
      if( _mousedDown == true )
      {
        _mousedDown = false;
        _mouseState = STATE_MOUSE_OVER;
        /*std::cout << "clicked!\n";
        std::cout << _imgPath << std::endl;*/
        return true;
      }
      break;
    }
  }
  return false;
}

void Sprite::setPosition( int x, int y )
{
  _position.x = x;
  _position.y = y;
}

/*
BUTTON
*/

Button::Button( ButtonSprite sprite, int width, int height, std::string imgPath )
{
  _width = width;
  _height = height;
  _position.x = 0;
  _position.y = 0;

  _sprite = sprite;
  _imgPath = imgPath;
  _mouseState = STATE_MOUSE_OUT;
}

void Button::render(SDL_Renderer* rend)
{
  _LTexture.loadTextureFromBMP( rend, _imgPath );
  _LTexture.render( rend , _position, _width, _height );
  // std::cout << "rendered building #: " << _id << std::endl;
}

ButtonSprite Button::getSprite()
{
  return _sprite;
}

ButtonState Button::getButtonState()
{
  return _buttonState;
}

void Button::setButtonState( ButtonState buttonState )
{
  _buttonState = buttonState;
}

void Button::update( ) const
{
  //
}

/*
BUILDING
*/

Building::Building( int id, BuildingSprite sprite, 
                    int width, int height, 
                    int xpos, int ypos,
                    std::string imgPath )
: _id( id )
{
  _width = width;
  _height = height;
  _position.x = xpos;
  _position.y = ypos;

  _sprite = sprite;
  _imgPath = imgPath;
  _mouseState = STATE_MOUSE_OUT;
}

int Building::getID()
{
  return _id;
}

SDL_Point Building::getBuildingCoord()
{
  return _position;
}
void Building::render(SDL_Renderer* rend)
{
  /* TODO: hover effects
  if( _mouseState == STATE_MOUSE_OUT ) path = "../img/house128.bmp";
  else if( _mouseState == STATE_MOUSE_OVER ) path = "../img/z128.bmp";
  else if( _mouseState == STATE_MOUSE_DOWN ) path = "../img/z128red.bmp";
  else if( _mouseState == STATE_MOUSE_UP ) path = "../img/z128blue.bmp";
  */
  

  _LTexture.loadTextureFromBMP( rend, _imgPath );
  _LTexture.render( rend , _position, _width, _height );
  // std::cout << "rendered building #: " << _id << std::endl;
}

void Building::update() const
{
  //overlay->activateButtons( this );
}