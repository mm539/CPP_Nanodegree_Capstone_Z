#ifndef BUTTON_H
#define BUTTON_H

#include "SDL.h"
#include "gfx.h"
#include "SDL_ttf.h"
#include <map>
#include <iostream>
#include <vector>
#include <string>

/* 

  1. foward declarations
  2. helper functions
  3. enums
  4. classes
    note: class Building inherits from class Button
*/

/********* 1. forward declarations  *********/
class GameStatsDisplay;

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

// gameplay button types
enum ButtonType{
  BNULL = 0,
  REST = 1,
  REPAIR = 2,
  GO = 3,
  GO_HOME = 4,
  SCOUT = 5,
  CLEAR = 6,
  SCAVENGE = 7,
  START = 8,
  QUIT = 9,
  HOME = 10,
  HARDWARE_STORE = 11, 
  CONVENIENCE_STORE = 12, 
  HOUSE = 13,
  COUNT = 14
};

// used to determine how to display a button
enum ButtonState
{
  BUTTON_INVISIBLE = 0, // not visible, unclickable
  BUTTON_VISIBLE = 1, // visible, clickable
  BUTTON_UNCLICKABLE = 2 // visible, unclickable
};

/********* 3. helper functions  *********/

std::string getImagePath(ButtonType type);


/********* 4. classes and structs *********/

struct FButtonInfo{
  ButtonType _buttonType;
  bool _clicked {false};
};

class Button
{
  public:
    Button(){}
    Button( ButtonType type, int width, int height); // constructor

    bool handleEvent( SDL_Event* e );
    void render(SDL_Renderer* rend);

    // getters and setters
    ButtonType getType();
    ButtonState getButtonState();
    void setPosition( int x, int y );
    void setButtonState( ButtonState buttonState );


  protected:
    ButtonType _buttonType;
    SDL_Point _position;
    CSMouseState _mouseState;
    ButtonState _buttonState { BUTTON_INVISIBLE };
    bool _mousedDown; // used to detect a full click
    int _width;
    int _height;
    std::string _imgPath;
    LTexture _LTexture;
};

class Building: public Button
{
  public:
    // Constructors
    Building( int id, ButtonType type,
              int width, int height, 
              int xpos, int ypos,
              int food, int materials, int dangerLevel );

    void render( SDL_Renderer* rend );

    SDL_Point getBuildingCoord();
    int getID();
    
    // functions for accessing food, materials, danger level, scouted, selected
    int getFoodAmt();
    int getMaterialsAmt();
    int getDangerLvl();
    bool getScouted();
    bool getSelected();

    void changeFoodAmt( int x );
    void changeMaterialsAmt( int x );
    void changeDangerLvl( int x );
    void changeScouted( bool x );
    void changeSelected( bool x );

    std::string getFoodText();
    std::string getMaterialsText();
    std::string getDangerText();
    std::string getScoutedText();
  
  private:
    int _id; // building ID

    int _food;
    int _materials;
    int _dangerLevel;
    bool _scouted;
    bool _selected;
};



#endif