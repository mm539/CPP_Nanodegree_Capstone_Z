#include "game.h"
#include <algorithm>
#include <time.h>
#include <random>
#include <iostream>
#include <thread>

enum EEndGameState;

Game::Game(int screen_width, int screen_height,
        int grid_width, int grid_height,
        PanelPD left_panel,
        PanelPD top_panel,
        PanelPD bottom_panel,
        PanelPD map_panel )
 : _screenWidth( screen_width ), _screenHeight( screen_height ),
   _gridWidth( grid_width ), _gridHeight( grid_height ),
   _leftPanel( left_panel ), _topPanel( top_panel ),
   _bottomPanel( bottom_panel ), _mapPanel( map_panel )
{
  makeBuildings();
  makeGSD(); // GameStatsDisplay
  makeButtons();
  makePlayer();
  _gameTime.setHours( 6 );
}


void Game::Run( Controller const &controller,
                Renderer &renderer,
                std::size_t target_frame_duration,
                Status &status )
{
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  Uint32 delay;
  // int frame_count = 0; // yet to be implemented feature for FPS calculation

  while( status.running && status.screen == EScreen::PLAYING )
  {
    frame_start = SDL_GetTicks();

    // INPUT, UPDATE, RENDER
    controller.handleEvent( status.running, _buildings, _buttons, _clickedButtonInfo, _clickedBuilding );
    update( status );
    renderer.renderAll( _buildings, _gameStatsDisplay, _buttons, _player );
    renderer.playMusic();
    if (_clickedButtonInfo._clicked == true){
      _clickedButtonInfo._clicked = false;
      renderer.playClickSound();
    }

    frame_end = SDL_GetTicks();

    // frame_count++;
    frame_duration = frame_end - frame_start;
    delay = target_frame_duration - frame_duration;

    if( frame_duration < target_frame_duration ) SDL_Delay( delay );

  }
  endGame();
}

void Game::endGame()
{
  if ( _player.getPlayerHealth() > 0 && _player.getHomeHealth() > 0 && _gameTime.winTimeMet() )
  {
    setEndGameState(EEndGameState::VICTORY);
  }
  else if ( _player.getPlayerHealth() <= 0 )
  {
    setEndGameState(EEndGameState::PLAYER_DEATH);
  }
  else if ( _player.getHomeHealth() <= 0 )
  {
    setEndGameState(EEndGameState::BASE_DEATH);
  }
}

void Game::makeBuildings()
{
  // x * y is the total number of buildings
  int x = _mapPanel._w / _gridWidth;
  int y = _mapPanel._h / _gridHeight;

  srand( time( NULL ) );
  int rando = rand() % 2;
  _homeID = ( ( x * y ) / 2 + 3 ) + rando; // set location of home base
  int id, xPos, yPos;
  ButtonType type;
  std::string imgPath;
  int food, materials, danger;
  bool build;
  
  for( int i = 0; i < y; i++ )
  {
    for( int j = 0; j < x; j++ )
    {
      id = i * y + j;
      xPos = _leftPanel._w + j * _gridWidth;
      yPos = _topPanel._h + i * _gridHeight;
      build = true;

      if( id == _homeID )
      {
        type = ButtonType::HOME;
        food = 0, materials = 0, danger = 0;
      }
      else if( ( rand() % 12 ) <= 7 )
      {
        type = ButtonType::HOUSE;
        food = 10 + rand() % 6;
        materials = 10 + rand() % 6;
        danger = 5 + rand() % 10;
      }
      else if( ( rand() % 12 ) <= 3 )
      {
        type = ButtonType::HARDWARE_STORE;
        food = 0;
        materials = 15 + rand() % 12;
        danger = 15 + rand() % 10;
      }
      else if( ( rand() % 12 ) <= 3 )
      {
        type = ButtonType::CONVENIENCE_STORE;
        food = 15 + rand() % 12;
        materials = 0;
        danger = 15 + rand() % 10;
      }
      else if( ( rand() % 12 ) <= 3 )
      {
        build = false;
      }

      if( build )
      {
        _buildings.emplace_back( 
          std::shared_ptr<Building> (
          new Building( id, type,
          _gridWidth , _gridHeight, xPos, yPos,
          food, materials, danger ) )
        );
      }

      if( id == _homeID ) _clickedBuilding = _buildings.back();
    }
  }
}

void Game::makeGSD()
{
  _gameStatsDisplay = GameStatsDisplay();
  _gameStatsDisplay.setLeftPanelWH( _leftPanel._w, _leftPanel._h );
  _gameStatsDisplay.setTopPanelWH( _topPanel._w, _topPanel._h );
  _gameStatsDisplay.setBottomPanelWH( _bottomPanel._w, _bottomPanel._h );
}

void Game::makeButtons()
{
  _buttons.push_back( Button( ButtonType::REST, 130, 60 ) );
  _buttons.push_back( Button( ButtonType::REPAIR, 130, 60 ) );
  _buttons.push_back( Button( ButtonType::GO, 130, 60 ) );
  _buttons.push_back( Button( ButtonType::GO_HOME, 130, 60 ) );
  _buttons.push_back( Button( ButtonType::SCOUT, 130, 60 ) );
  _buttons.push_back( Button( ButtonType::CLEAR, 130, 60 ) );
  _buttons.push_back( Button( ButtonType::SCAVENGE, 130, 60 ) );
}

void Game::makePlayer()
{
  SDL_Point playerPosition = getBuildingCoord( _homeID );

  _player = Player();
  _player.setPosition( playerPosition.x, playerPosition.y );
  _player.setLocationID( _homeID );
}

void Game::update( Status &status )
{
  updateButtons(); // sets which buttons are visible, invisible, clickable according to 1. location of player 2. building clicked on
  buttonAction(); // performs the action set in _clickedButtonInfo
  updateGSD();
 
  // if player is at home and has supplies, transfer those materials to the base
  if( ( _player.getPlayerFood() > 0 || _player.getPlayerMaterials() > 0 ) && _player.getLocationID() == _homeID )
  {
    _player.changeHomeFood( _player.getPlayerFood() );
    _player.changeHomeMaterials( _player.getPlayerMaterials() );

    _player.changePlayerFood( -_player.getPlayerFood() );
    _player.changePlayerMaterials( -_player.getPlayerMaterials() );
  }

  // if time is 5:00am, consume some food. if not enought food, then take dmg. set time to 6:00am
  if( _gameTime.getHours() == 5 )
  {
    if( _player.getHomeFood() > 10 )
    {
      _player.changeHomeFood( -10 );
    }
    else _player.changePlayerHealth( -20 );
    _gameTime.setHours( 6 );
    _gameTime.setMinutes( 0 );
  }

  // if time is midnight, return player home and initiate a zombie attack, then set time to 5:00am
  if( _gameTime.getHours() >= 0 && _gameTime.getHours() <= 4 )
  {
    SDL_Point pos = getBuildingCoord( _homeID );
    _player.setLocationID( _homeID );
    _player.setPosition( pos.x, pos.y );
    _player.changeHomeHealth( -( 5 + rand() % 25 ) );
    _actionResultText = "6:00 : returned home. zombies attacked.";
    _gameTime.setHours( 5 );
  }

  // check if player has finished the game
  if( _player.getPlayerHealth() <= 0 || _player.getHomeHealth() <= 0 || _gameTime.winTimeMet() )
  {
    status.screen = EScreen::CREDIT;
  }
}

void Game::buttonAction()
{
  switch ( _clickedButtonInfo._buttonType )
  {
  case ButtonType::REST:
    _actionResultText = _gameTime.getActionMsg( " Rested for an hour. ");
    _gameTime.addTime( 1 );
    _player.changePlayerHealth( 5 );
    break;

  case ButtonType::REPAIR:
    _actionResultText = _gameTime.getActionMsg( " Repaired defenses for an hour. ");
    _gameTime.addTime( 1 );
    _player.changeHomeHealth( 10 );
    _player.changeHomeMaterials( - 5 );
    break;

  case ButtonType::GO:
    if( computeTravelTime() + _gameTime.getHours() <= 24 ) 
     {
       _actionResultText = _gameTime.getActionMsg( " Traveled to a building. ");
       _gameTime.addTime( computeTravelTime() ) ;
       _player.setPosition( _clickedBuilding->getBuildingCoord().x, _clickedBuilding->getBuildingCoord().y );
       _player.setLocationID( _clickedBuilding->getID() );
     }
     else
     {
       _actionResultText =  _gameTime.getActionMsg( " Didn't go to location. Too far. ");
     }
     
    break;

  case ButtonType::GO_HOME:
    if( computeTravelTime() + _gameTime.getHours() <= 24 )
    {
      _actionResultText = _gameTime.getActionMsg( " Went home. ");
      _gameTime.addTime( computeTravelTime() ) ;
      _player.setPosition( _clickedBuilding->getBuildingCoord().x, _clickedBuilding->getBuildingCoord().y );
      _player.setLocationID( _clickedBuilding->getID() );
    }
    else
    {
      _actionResultText =  _gameTime.getActionMsg( " Didn't go home. Go to a closer location first. ");
    }
    break;
// the next three cases use buildingStatus
  case ButtonType::SCOUT:
    _actionResultText = _gameTime.getActionMsg( " Scouted for an hour. ");
    _gameTime.addTime( 1 );
    _clickedBuilding->changeScouted( true );
    break;

  case ButtonType::CLEAR:
    clearBuilding();
    _actionResultText = _gameTime.getActionMsg( " Cleared some Zs. ");
    _gameTime.addTime( 1 );
    break;

  case ButtonType::SCAVENGE:
    scavengeBuilding();
    _actionResultText = _gameTime.getActionMsg( " Scavenged. ");
    _gameTime.addTime( 1 );
    break;

  
  default:
    break;
  }
  _clickedButtonInfo._buttonType = ButtonType::BNULL;
}


void Game::updateButtons()
{
  /* update _buttonState of the buttons in _buttons according to:
    1. _clickedBuilding
    2. _player._locationID
    3. _homeID
  */
  int playerLocationID = _player.getLocationID();
  int buildingID = _clickedBuilding->getID();

  // declare and initialize bools for different cases
  bool atHomeANDclickBuilding = false;
  bool atHomeANDclickHome = false;
  bool atBuildingANDclickHome = false;
  bool atBuildingANDclickOtherBuilding = false;
  bool atBuildingANDclickSameBuilding = false;

  bool scouted = _clickedBuilding->getScouted();
  int foodAmt = _clickedBuilding->getFoodAmt();
  int matAmt = _clickedBuilding->getMaterialsAmt();
  int dangerLvl = _clickedBuilding->getDangerLvl();
  int playerHealth = _player.getPlayerHealth();
  int homeHealth = _player.getHomeHealth();

  // set the bools that will determine which buttons are visible
  if( playerLocationID == _homeID && buildingID != _homeID ) atHomeANDclickBuilding = true;
  else if ( playerLocationID == _homeID && buildingID == _homeID ) atHomeANDclickHome = true;
  else if ( playerLocationID != _homeID && buildingID == _homeID ) atBuildingANDclickHome = true;
  else if ( playerLocationID != buildingID ) atBuildingANDclickOtherBuilding = true;
  else if ( playerLocationID == buildingID) atBuildingANDclickSameBuilding = true;

  // set the buttons to visible, invisible, or unclickable
  std::for_each( _buttons.begin(), _buttons.end(), [
    &atHomeANDclickBuilding,
    &atHomeANDclickHome,
    &atBuildingANDclickHome,
    &atBuildingANDclickOtherBuilding,
    &atBuildingANDclickSameBuilding,
    &scouted, &foodAmt, &matAmt, &dangerLvl, &playerHealth, &homeHealth
  ]( Button& b )
  {
    if( atHomeANDclickBuilding )
    {
      if( b.getType() == ButtonType::GO ) b.setButtonState( ButtonState::BUTTON_VISIBLE );
      else b.setButtonState( ButtonState::BUTTON_INVISIBLE );
    }
    else if( atHomeANDclickHome )
    {
      if( b.getType() == ButtonType::REST )
      {
        b.setButtonState( ButtonState::BUTTON_VISIBLE );
      }
      else if( b.getType() == ButtonType::REPAIR )
      {
        if( homeHealth < 100) b.setButtonState( ButtonState::BUTTON_VISIBLE );
        else b.setButtonState( ButtonState::BUTTON_UNCLICKABLE );
      }
      else b.setButtonState( ButtonState::BUTTON_INVISIBLE );
    }
    else if( atBuildingANDclickHome )
    {
      if( b.getType() == ButtonType::GO_HOME ) 
      b.setButtonState( ButtonState::BUTTON_VISIBLE );
      else b.setButtonState( ButtonState::BUTTON_INVISIBLE );
    }
    else if( atBuildingANDclickOtherBuilding )
    {
      if( b.getType() == ButtonType::GO ) 
      b.setButtonState( ButtonState::BUTTON_VISIBLE );
      else b.setButtonState( ButtonState::BUTTON_INVISIBLE );
    }
    else if( atBuildingANDclickSameBuilding )
    {
      if( b.getType() == ButtonType::SCOUT && scouted ) b.setButtonState( ButtonState::BUTTON_UNCLICKABLE );
      else if( b.getType() == ButtonType::SCAVENGE && foodAmt == 0 && matAmt == 0 ) b.setButtonState( ButtonState::BUTTON_UNCLICKABLE );
      else if( b.getType() == ButtonType::CLEAR && dangerLvl == 0 ) b.setButtonState( ButtonState::BUTTON_UNCLICKABLE );      
      else if( b.getType() == ButtonType::SCOUT ||
          b.getType() == ButtonType::SCAVENGE ||
          b.getType() == ButtonType::CLEAR ) 
      b.setButtonState( ButtonState::BUTTON_VISIBLE );
      else b.setButtonState( ButtonState::BUTTON_INVISIBLE );
    }
  });
}

void Game::updateGSD()
{
  // time
  _gameStatsDisplay._gameStatsTexts._clockText = 
    "Day : " + std::to_string( _gameTime.getDays() ) + 
    " Time : " + std::to_string( _gameTime.getHours() ) + 
    " : " + _gameTime.getMinutesS() ;
  _gameStatsDisplay._gameStatsTexts._buildingText = std::to_string( 999 );

  // player stats
  _gameStatsDisplay._gameStatsTexts._playerHealthText = "Health : " + std::to_string( _player.getPlayerHealth() ) + " / 100" ;
  _gameStatsDisplay._gameStatsTexts._playerFoodText = "Food: " + std::to_string( _player.getPlayerFood() );
  _gameStatsDisplay._gameStatsTexts._playerMaterialsText = "Materials : " + std::to_string( _player.getPlayerMaterials() );

  // base stats
  _gameStatsDisplay._gameStatsTexts._homeHealthText = "Defenses : " + std::to_string( _player.getHomeHealth() ) + " / 100";
  _gameStatsDisplay._gameStatsTexts._homeFoodText = "Food : " + std::to_string( _player.getHomeFood() );
  _gameStatsDisplay._gameStatsTexts._homeMaterialsText = "Materials : " + std::to_string( _player.getHomeMaterials() );

  // selected building stats
  if( _clickedBuilding->getID() == _homeID )
  {
    _gameStatsDisplay._gameStatsTexts._buildingDangerText = " ";
    _gameStatsDisplay._gameStatsTexts._buildingScoutedText = " ";
    _gameStatsDisplay._gameStatsTexts._buildingFoodText = " ";
    _gameStatsDisplay._gameStatsTexts._buildingMaterialsText = " ";
  }
  else
  {
    _gameStatsDisplay._gameStatsTexts._buildingDangerText = "Danger : " +  _clickedBuilding->getDangerText();
    _gameStatsDisplay._gameStatsTexts._buildingScoutedText = "Scouted : " + _clickedBuilding->getScoutedText();
    _gameStatsDisplay._gameStatsTexts._buildingFoodText = "Food : " +  _clickedBuilding->getFoodText();
    _gameStatsDisplay._gameStatsTexts._buildingMaterialsText = "Materials : " +  _clickedBuilding->getMaterialsText();
  }

  //  bottom panel display text
  _gameStatsDisplay._gameStatsTexts._actionResultText = _actionResultText;
}

SDL_Point Game::getBuildingCoord( int id )
{
  for( int i = 0; i < _buildings.size(); i++ )
  {
    if( _buildings[ i ]->getID() == id )
    {
      return _buildings[ i ]->getBuildingCoord();
    }
  }
  return SDL_Point( { 0 , 0 } );
}

float Game::computeTravelTime()
{
  SDL_Point buildingCoord = _clickedBuilding->getBuildingCoord();
  SDL_Point playerCoord = _player.getPosition();

  float xDiffSqd = pow( ( playerCoord.x - buildingCoord.x ) / _gridWidth, 2) ;
  float yDiffSqd = pow( ( playerCoord.y - buildingCoord.y ) / _gridHeight, 2 );

  return sqrt( xDiffSqd + yDiffSqd );
}

void Game::clearBuilding()
{
  // _clickedBuilding, _player
  int dmgToPlayer = -(5 + ( rand() % 5 ) * 2);
  int dmgToZs = -(5 + ( rand() % 5 ) * 2);
  _player.changePlayerHealth( dmgToPlayer );
  _clickedBuilding->changeDangerLvl( dmgToZs );
  if( _clickedBuilding->getDangerLvl() < 0 )
  {
    int zero = -( _clickedBuilding->getDangerLvl() );
    _clickedBuilding->changeDangerLvl( zero );
  }
}

void Game::scavengeBuilding()
{
  int buildingFood = _clickedBuilding->getFoodAmt();
  int buildingMat = _clickedBuilding->getMaterialsAmt();

  bool caseFM = ( buildingFood > 0 ) && ( buildingMat > 0 );
  bool caseF = ( buildingFood > 0 ) && ( buildingMat == 0 );
  bool caseM = ( buildingFood == 0 ) && ( buildingMat >= 0 );

  if( caseFM )
  {
    if( buildingFood >= 5 )
    {
      _player.changePlayerFood( 5 );
      _clickedBuilding->changeFoodAmt( -5 );
    }
    else // 
    {
      _player.changePlayerFood( _clickedBuilding->getFoodAmt() );
      _clickedBuilding->changeFoodAmt( -_clickedBuilding->getFoodAmt() );
    }
    
    if( buildingMat >= 5 )
    {
      _player.changePlayerMaterials( 5 );
      _clickedBuilding->changeMaterialsAmt( -5 );
    }
    else // 
    {
      _player.changePlayerMaterials( _clickedBuilding->getMaterialsAmt() );
      _clickedBuilding->changeMaterialsAmt( -_clickedBuilding->getMaterialsAmt() );
    }
  }
  else if( caseF )
  {
    if( buildingFood >= 10 )
    {
      _player.changePlayerFood( 10 );
      _clickedBuilding->changeFoodAmt( -10 );
    }
    else 
    {
      _player.changePlayerFood( _clickedBuilding->getFoodAmt() );
      _clickedBuilding->changeFoodAmt( -_clickedBuilding->getFoodAmt() );
    }
  }
  else if( caseM )
  {
    if( buildingMat >= 10 )
    {
      _player.changePlayerMaterials( 10 );
      _clickedBuilding->changeMaterialsAmt( -10 );
    }
    else 
    {
      _player.changePlayerMaterials( _clickedBuilding->getMaterialsAmt() );
      _clickedBuilding->changeMaterialsAmt( -_clickedBuilding->getMaterialsAmt() );
    }
  }

  if( _clickedBuilding->getDangerLvl() > 0 )
  {
    _player.changePlayerHealth( -( 10 + rand() % 3 ) );
  }
}

EEndGameState Game::getEndGameState()
{
  return _endGameState;
}

void Game::setEndGameState(EEndGameState endGameState)
{
  _endGameState = endGameState;
}

GameTime::GameTime()
{
  _winTime = 24 * 4 + 24 * ( rand() % 3 ) + 9;
}
GameTime::GameTime( int days, int hours, int minutes )
{
  setDays( days );
  setHours( hours );
  setMinutes( minutes );
  _winTime = 24 * 4 + 24 * ( rand() % 3 ) + 9;
}

void GameTime::setDays( int days )
{
  _days = days;
}

void GameTime::setHours( int hours )
{
  _hours = hours;
}

void GameTime::setMinutes( int minutes )
{
  _minutes = minutes;
}

void GameTime::addTime( float time ) // time in hours
{
  int hours = time;
  int minutes = ( time - hours ) * 60; // ( 22.4 - 22 ) * 60
  int minuteHolder = _minutes + minutes;
  int hourHolder = _hours + hours;

  // add minutes
  if( minuteHolder >= 60 )
  {
    hourHolder++;
    _minutes = _minutes + minutes - 60;
  }
  else
  {
    _minutes = _minutes + minutes;
  }

  // add hours
  if( hourHolder >= 24 )
  {
    _days++;
    _hours = _hours + hours - 24;
  }
  else
  {
    _hours = _hours + hours;
  }
  
  
}

int GameTime::getDays()
{
  return _days;
}

int GameTime::getHours()
{
  return _hours;
}

int GameTime::getMinutes()
{
  return _minutes;
}

float GameTime::getMinutesD()
{
  float minutes = _minutes / 60;
  return minutes;
}

bool GameTime::winTimeMet()
{
  float m = _minutes / 60 ;
  float time = _days * 24 + _hours + m;
  if( time >= _winTime ) return true;
  return false;
}

std::string GameTime::getActionMsg( std::string action )
{
  std::string msg = 
    std::to_string( _hours ) + " : " +
    getMinutesS() + " : " + action;
  return msg;
}

std::string GameTime::getMinutesS()
{
  std::string m = std::to_string( _minutes );
  if( m.length() == 1 )
  {
    m = "0" + m;
  }
  return m;
}