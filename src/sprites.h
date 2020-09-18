#ifndef SPRITES_H
#define SPRITES_H

#include "SDL.h"
#include "gfx.h"
#include "SDL_ttf.h"
#include <map>
#include <iostream>
#include <vector>
#include <string>

/********* 1. Forward declaration  *********/
class Overlay;

/********* 2. ENUMS  *********/

// used in event handling
enum CSMouseState 
{
  STATE_MOUSE_OUT = 0,
  STATE_MOUSE_OVER = 1,
  STATE_MOUSE_DOWN = 2,
  STATE_MOUSE_UP = 3,
  STATE_TOTAL = 4
};

// gameplay button sprites
enum ButtonSprite
{
  BUTTON_SPRITE_NULL = 0,
  BUTTON_SPRITE_REST = 1,
  BUTTON_SPRITE_REPAIR = 2,
  BUTTON_SPRITE_GO = 3,
  BUTTON_SPRITE_GO_HOME = 4,
  BUTTON_SPRITE_SCOUT = 5,
  BUTTON_SPRITE_CLEAR = 6,
  BUTTON_SPRITE_SCAVENGE = 7,
  BUTTON_TOTAL = 8
};

// used to determine how to display a button
enum ButtonState
{
  BUTTON_INVISIBLE = 0, // not visible, unclickable
  BUTTON_VISIBLE = 1, // visible, clickable
  BUTTON_UNCLICKABLE = 2 // visible, unclickable
};

// used to determine which image to render for a building
enum BuildingSprite
{
  BUILDING_SPRITE_HOME = 0,
  BUILDING_SPRITE_HSTORE = 1, // hardware store
  BUILDING_SPRITE_CSTORE = 2, // convenience store
  BUILDING_SPRITE_HOUSE = 3,
  BUILDING_SPRITE_TOTAL = 4
};



/********* 3. Sprites  *********/

class Sprite
{
  public:
    // constructor
    Sprite(){}

    bool handleEvent( SDL_Event* e );
    void render(SDL_Renderer* rend);
    virtual void update() const = 0; // pure virtual function.

    CSMouseState getMouseState();
    void setPosition( int x, int y );

  protected:
    SDL_Point _position;
    CSMouseState _mouseState;
    bool _mousedDown; // used to detect a full click
    int _width;
    int _height;
    std::string _imgPath;
};

class Button: public Sprite
{
  public:
    // constructor
    Button( ButtonSprite sprite, int width, int height, std::string imgPath );

    void render( SDL_Renderer* rend );
    void update() const;

    ButtonSprite getSprite();
    ButtonState getButtonState();

    void setButtonState( ButtonState buttonState );

  private:
    ButtonSprite _sprite;
    ButtonState _buttonState { BUTTON_INVISIBLE };
    LTexture _LTexture;
};

class Building: public Sprite
{
  public:
    // Constructors
    Building(){}
    Building( int id, BuildingSprite sprite,
              int width, int height, 
              int xpos, int ypos,
              std::string _imgPath,
              int food, int materials, int dangerLevel );

    void update() const;
    void render( SDL_Renderer* rend );

    SDL_Point getBuildingCoord();
    int getID();
    
    // functions for accessing food, materials, danger level, and scouted
    int getFoodAmt();
    int getMaterialsAmt();
    int getDangerLvl();
    bool getScouted();

    void changeFoodAmt( int x );
    void changeMaterialsAmt( int x );
    void changeDangerLvl( int x );
    void changeScouted( bool x );

    std::string getFoodText();
    std::string getMaterialsText();
    std::string getDangerText();
    std::string getScoutedText();
  
  private:
    int _id; // building ID
    BuildingSprite _sprite;
    LTexture _LTexture;

    int _food;
    int _materials;
    int _dangerLevel;
    bool _scouted;
};



#endif