#include "button.h"

std::string getImagePath(ButtonType type){
  std::map<ButtonType, std::string> paths = {
    {ButtonType::BNULL, ""},
    {ButtonType::QUIT, "../img/xx-quit-130-60.bmp"},
    {ButtonType::REST, "../img/01rest-130-60.bmp"},
    {ButtonType::REPAIR, "../img/02repair-130-60.bmp"},
    {ButtonType::GO, "../img/03go-130-60.bmp" },
    {ButtonType::GO_HOME, "../img/04gohome-130-60.bmp"},
    {ButtonType::SCOUT, "../img/05scout-130-60.bmp"},
    {ButtonType::CLEAR, "../img/06clear-130-60.bmp"},
    {ButtonType::SCAVENGE, "../img/07scavenge-130-60.bmp"},
    {ButtonType::START, "../img/xx-start-130-60.bmp"},
    {ButtonType::HOME, "../img/buildings-home128.bmp"},
    {ButtonType::HOUSE, "../img/buildings-house128.bmp"},
    {ButtonType::HARDWARE_STORE, "../img/buildings-hstore128.bmp"},
    {ButtonType::CONVENIENCE_STORE, "../img/buildings-cstore128.bmp"}
  };

  std::string path;

  try
  {
    path = paths.at(type);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  
  return path;
}

/*
Button
*/

bool Button::handleEvent( SDL_Event* e )
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

void Button::setPosition( int x, int y )
{
  _position.x = x;
  _position.y = y;
}

/*
BUTTON
*/

Button::Button( ButtonType type, int width, int height )
{
  _width = width;
  _height = height;
  _position.x = 0;
  _position.y = 0;

  _buttonType = type;
  _imgPath = getImagePath(type);
  _mouseState = STATE_MOUSE_OUT;
}

void Button::render(SDL_Renderer* rend)
{
  if (_LTexture.needsLoading() ) _LTexture.loadTextureFromBMP( rend, _imgPath );
  if( _buttonState == ButtonState::BUTTON_UNCLICKABLE )
  {
    _LTexture.setColor( 0x7f, 0x7f, 0x7f );
  }
  else if( _mouseState == CSMouseState::STATE_MOUSE_OVER )
  {
    _LTexture.setColor( 0x46, 0xcf, 0x1b ); 
  }
  else if( _mouseState == CSMouseState::STATE_MOUSE_OUT )
  {
    _LTexture.setColor( 0xFF, 0xFF, 0xFF ); 
  }
  else if( _mouseState == CSMouseState::STATE_MOUSE_DOWN )
  {
    _LTexture.setColor( 0x12, 0x88, 0x12 ); 
  }
  _LTexture.render( rend , _position, _width, _height );
}

ButtonType Button::getType()
{
  return _buttonType;
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

Building::Building( int id, ButtonType type, 
                    int width, int height, 
                    int xpos, int ypos,
                    int food, int materials, int dangerLevel )
{
  _id = id;
  _width = width;
  _height = height;
  _position.x = xpos;
  _position.y = ypos;

  _buttonType = type;
  _imgPath = getImagePath(type);
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
  if (_LTexture.needsLoading() ) _LTexture.loadTextureFromBMP( rend, _imgPath );
  if( _selected == true )
  {
    _LTexture.render( rend , _position, _width, _height );
    _LTexture.setBlendModeForDraw( rend, SDL_BLENDMODE_BLEND );
    _LTexture.setAlpha( 0 );
    _LTexture.renderRect( rend, _position, _width, _height, { 128, 0, 128, 112 } );
    return;
  }
  else if( _mouseState == CSMouseState::STATE_MOUSE_OVER)
  {
    _LTexture.setColor( 0xdd, 0xb1, 0xd9 ); 
  }
  else if( _mouseState == CSMouseState::STATE_MOUSE_OUT)
  {
    _LTexture.setColor( 0xFF, 0xFF, 0xFF ); 
  }
  _LTexture.render( rend , _position, _width, _height );
  
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