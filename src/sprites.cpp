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
  if( _buttonState == ButtonState::BUTTON_UNCLICKABLE )
  {
    _LTexture.setColor( 0x7f, 0x7f, 0x7f );
  }
  else if( _mouseState == CSMouseState::STATE_MOUSE_OVER )
  {
    _LTexture.setColor( 0x46, 0xcf, 0x1b ); 
  }
  else if( _mouseState == CSMouseState::STATE_MOUSE_DOWN )
  {
    _LTexture.setColor( 0x12, 0x88, 0x12 ); 
  }
  _LTexture.render( rend , _position, _width, _height );
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

/*
BUILDING
*/

Building::Building( int id, BuildingSprite sprite, 
                    int width, int height, 
                    int xpos, int ypos,
                    std::string imgPath,
                    int food, int materials, int dangerLevel )
: _id( id )
{
  _width = width;
  _height = height;
  _position.x = xpos;
  _position.y = ypos;

  _sprite = sprite;
  _imgPath = imgPath;
  _mouseState = STATE_MOUSE_OUT;
  _scouted = false;
  _food = food;
  _materials = materials;
  _dangerLevel = dangerLevel;
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
  
  _LTexture.loadTextureFromBMP( rend, _imgPath );
  if( _selected == true )
  {
    _LTexture.render( rend , _position, _width, _height );
    _LTexture.setTextureWH( _width, _height );
    _LTexture.setBlendModeForDraw( rend, SDL_BLENDMODE_BLEND );
    _LTexture.setAlpha( 0 );
    _LTexture.renderRect( rend, _position, { 128, 0, 128, 112 } );
  }
  else if( _mouseState == CSMouseState::STATE_MOUSE_OVER)
  {
    _LTexture.setColor( 0xdd, 0xb1, 0xd9 ); 
    _LTexture.render( rend , _position, _width, _height );
  }
  else
  {
    _LTexture.render( rend , _position, _width, _height );
  }
  
}

// functions for accessing food, materials, danger level, and scouted

int Building::getFoodAmt()
{
  return _food;
}

int Building::getMaterialsAmt()
{
  return _materials;
}

int Building::getDangerLvl()
{
  return _dangerLevel;
}

bool Building::getScouted()
{
  return _scouted;
}

bool Building::getSelected()
{
  return _selected;
}

void Building::changeFoodAmt( int x )
{
  _food = _food + x;
}

void Building::changeMaterialsAmt( int x )
{
  _materials = _materials + x;
}

void Building::changeDangerLvl( int x )
{
  _dangerLevel = _dangerLevel + x;
}

void Building::changeScouted( bool x )
{
  _scouted = x;
}

void Building::changeSelected( bool x )
{
  _selected = x;
}

std::string Building::getFoodText()
{
  if( getScouted() )
  {
    return std::to_string( getFoodAmt() );
  }
  return "unknown";
}

std::string Building::getMaterialsText()
{
  if( getScouted() )
  {
    return std::to_string( getMaterialsAmt() );
  }
  return "unknown";
}

std::string Building::getDangerText()
{
  if( getScouted() )
  {
    return std::to_string( getDangerLvl() );
  }
  return "unknown";
}

std::string Building::getScoutedText()
{
  if( getScouted() )
  {
    return "Yes";
  }
  return "No";
}