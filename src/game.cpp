#include "game.h"
#include <algorithm>
#include <time.h>
#include <random>
#include <iostream>

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
  _creditsMSG = TextDisplay( "filler", { 100, 100 } );
  makeBuildings();
  makeGSD(); // GameStatsDisplay
  makeButtons();
  makePlayer();
  _winTime = 24 * 4 + 24 * ( rand() % 3 ) + 9;
}


void Game::Run( Controller const &controller,
                Renderer &renderer,
                std::size_t target_frame_duration,
                Status &status )
{
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  // int frame_count = 0; // yet to be implemented feature for FPS calculation

  while( status.running && status.gameScreen )
  {
    frame_start = SDL_GetTicks();

    // INPUT, UPDATE, RENDER
    controller.handleEvent( status.running, _buildings, _buttons, _clickedButtonSprite, _clickedBuilding );
    update( status );
    renderer.renderAll( _buildings, _gameStatsDisplay, _buttons, _player ); 

    frame_end = SDL_GetTicks();

    // frame_count++;
    frame_duration = frame_end - frame_start;

    if( frame_duration < target_frame_duration ) SDL_Delay( target_frame_duration - frame_duration );

    if( !status.gameScreen )
    {
      endGame();
    }
  }

}

void Game::endGame()
{
  if ( _player.getPlayerHealth() > 0 && _player.getHomeHealth() > 0 && _time >= _winTime )
  {
    _creditsMSG.setText("You hear the sound of gunfire quickly approaching your location. As quickly as you can, you use your shirt and a long branch on the ground nearby to make a white flag to wave in the air. In the distance you hear faint shouts of what sounds like \"We've got another survivor! We've got another survivor!\" Soon, the soldiers make it to your location and beckon you onto a truck with what appears to be more survivors.\n  Congratulations! You survived!\n");
  }
  else if ( _player.getPlayerHealth() <= 0 )
  {
    _creditsMSG.setText("You've died! Better luck next time!\n");
    std::cout << "settext\n";
  }
  else if ( _player.getHomeHealth() <= 0 )
  {
    _creditsMSG.setText("Your base has been overun! Better luck next time!\n");
  }
  else
  {
    _creditsMSG.setText("You've quit the game.\n");
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
  BuildingSprite sprite;
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
        sprite = BuildingSprite::BUILDING_SPRITE_HOME;
        imgPath = "../img/buildings-home128.bmp";
        food = 0, materials = 0, danger = 0;
      }
      else if( ( rand() % 12 ) <= 7 )
      {
        sprite = BuildingSprite::BUILDING_SPRITE_HOUSE;
        imgPath = "../img/buildings-house128.bmp";
        food = 10 + rand() % 6;
        materials = 10 + rand() % 6;
        danger = 5 + rand() % 10;
      }
      else if( ( rand() % 12 ) <= 3 )
      {
        sprite = BuildingSprite::BUILDING_SPRITE_HSTORE;
        imgPath = "../img/buildings-hstore128.bmp";
        food = 0;
        materials = 15 + rand() % 12;
        danger = 15 + rand() % 10;
      }
      else if( ( rand() % 12 ) <= 3 )
      {
        sprite = BuildingSprite::BUILDING_SPRITE_CSTORE;
        imgPath = "../img/buildings-cstore128.bmp";
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
          new Building( id, sprite,
          _gridWidth , _gridHeight, xPos, yPos,
          imgPath, food, materials, danger ) )
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
  _buttons.push_back( Button( ButtonSprite::BUTTON_SPRITE_REST, 130, 60, "../img/01rest-130-60.bmp" ) );
  _buttons.push_back( Button( ButtonSprite::BUTTON_SPRITE_REPAIR, 130, 60, "../img/02repair-130-60.bmp" ) );
  _buttons.push_back( Button( ButtonSprite::BUTTON_SPRITE_GO, 130, 60, "../img/03go-130-60.bmp" ) );
  _buttons.push_back( Button( ButtonSprite::BUTTON_SPRITE_GO_HOME, 130, 60, "../img/04gohome-130-60.bmp" ) );
  _buttons.push_back( Button( ButtonSprite::BUTTON_SPRITE_SCOUT, 130, 60, "../img/05scout-130-60.bmp" ) );
  _buttons.push_back( Button( ButtonSprite::BUTTON_SPRITE_CLEAR, 130, 60, "../img/06clear-130-60.bmp" ) );
  _buttons.push_back( Button( ButtonSprite::BUTTON_SPRITE_SCAVENGE, 130, 60, "../img/07scavenge-130-60.bmp" ) );
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
  buttonAction(); // performs the action set in _clickedButtonSprite
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
  if( (_time % 24 ) == 5 )
  {
    if( _player.getHomeFood() > 10 )
    {
      _player.changeHomeFood( -10 );
    }
    else _player.changePlayerHealth( -20 );
    _time = _time + 1;
  }

  // if time is midnight, return player home and initiate a zombie attack, then set time to 5:00am
  if( ( _time % 24 ) == 0 )
  {
    SDL_Point pos = getBuildingCoord( _homeID );
    _player.setLocationID( _homeID );
    _player.setPosition( pos.x, pos.y );
    _player.changeHomeHealth( -( 5 + rand() % 25 ) );
    _actionResultText = "6:00 : returned home. zombies attacked.";
    _time = _time + 5;
  }

  // check if player has finished the game
  if( _player.getPlayerHealth() <= 0 || _player.getHomeHealth() <= 0 || _time >= _winTime )
  {
    status.gameScreen = false;
    status.creditScreen = true;
  }
}

void Game::buttonAction()
{
  switch ( _clickedButtonSprite )
  {
  case ButtonSprite::BUTTON_SPRITE_REST:
    _actionResultText = std::to_string( _time % 24 ) + " :00 : Rested for an hour.";
    _time = _time + 1;
    _player.changePlayerHealth( 5 );
    break;

  case ButtonSprite::BUTTON_SPRITE_REPAIR:
    _actionResultText = std::to_string( _time % 24 ) + " :00 : Repaired defenses for an hour.";
    _time = _time + 1;
    _player.changeHomeHealth( 10 );
    _player.changeHomeMaterials( - 5 );
    break;

  case ButtonSprite::BUTTON_SPRITE_GO:
    if( computeTravelTime() + _time % 24 <= 24 && ( computeTravelTime() + _time - 1 ) / 24 == _time / 24 )
     {
       _actionResultText = std::to_string( _time % 24 ) + " :00 : traveled to a building.";
       _time = _time + computeTravelTime();    
       _player.setPosition( _clickedBuilding->getBuildingCoord().x, _clickedBuilding->getBuildingCoord().y );
       _player.setLocationID( _clickedBuilding->getID() );
     }
     else
     {
       _actionResultText = std::to_string( _time % 24 ) + " :00 : Didn't go. Location is too far.";
     }
     
    break;

  case ButtonSprite::BUTTON_SPRITE_GO_HOME:
    _actionResultText = std::to_string( _time % 24 ) + " :00 : traveled home.";
    _time = _time + computeTravelTime();
    _player.setPosition( _clickedBuilding->getBuildingCoord().x, _clickedBuilding->getBuildingCoord().y );
     _player.setLocationID( _clickedBuilding->getID() );
    break;
// the next three cases use buildingStatus
  case ButtonSprite::BUTTON_SPRITE_SCOUT:
    _actionResultText = std::to_string( _time % 24 ) + " :00 : scouted.";
    _time = _time + 1;
    _clickedBuilding->changeScouted( true );
    break;

  case ButtonSprite::BUTTON_SPRITE_CLEAR:
    clearBuilding();
    _actionResultText = std::to_string( _time % 24 ) + " :00 : cleared some Zs";
    _time = _time + 1;
    break;

  case ButtonSprite::BUTTON_SPRITE_SCAVENGE:
    scavengeBuilding();
    _actionResultText = std::to_string( _time % 24 ) + " :00 : scavenged.";
    _time = _time + 1;
    break;

  
  default:
    break;
  }
  _clickedButtonSprite = ButtonSprite::BUTTON_SPRITE_NULL;
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
      if( b.getSprite() == ButtonSprite::BUTTON_SPRITE_GO ) b.setButtonState( ButtonState::BUTTON_VISIBLE );
      else b.setButtonState( ButtonState::BUTTON_INVISIBLE );
    }
    else if( atHomeANDclickHome )
    {
      if( b.getSprite() == ButtonSprite::BUTTON_SPRITE_REST )
      {
        b.setButtonState( ButtonState::BUTTON_VISIBLE );
      }
      else if( b.getSprite() == ButtonSprite::BUTTON_SPRITE_REPAIR )
      {
        if( homeHealth < 100) b.setButtonState( ButtonState::BUTTON_VISIBLE );
        else b.setButtonState( ButtonState::BUTTON_UNCLICKABLE );
      }
      else b.setButtonState( ButtonState::BUTTON_INVISIBLE );
    }
    else if( atBuildingANDclickHome )
    {
      if( b.getSprite() == ButtonSprite::BUTTON_SPRITE_GO_HOME ) 
      b.setButtonState( ButtonState::BUTTON_VISIBLE );
      else b.setButtonState( ButtonState::BUTTON_INVISIBLE );
    }
    else if( atBuildingANDclickOtherBuilding )
    {
      if( b.getSprite() == ButtonSprite::BUTTON_SPRITE_GO ) 
      b.setButtonState( ButtonState::BUTTON_VISIBLE );
      else b.setButtonState( ButtonState::BUTTON_INVISIBLE );
    }
    else if( atBuildingANDclickSameBuilding )
    {
      if( b.getSprite() == ButtonSprite::BUTTON_SPRITE_SCOUT && scouted ) b.setButtonState( ButtonState::BUTTON_UNCLICKABLE );
      else if( b.getSprite() == ButtonSprite::BUTTON_SPRITE_SCAVENGE && foodAmt == 0 && matAmt == 0 ) b.setButtonState( ButtonState::BUTTON_UNCLICKABLE );
      else if( b.getSprite() == ButtonSprite::BUTTON_SPRITE_CLEAR && dangerLvl == 0 ) b.setButtonState( ButtonState::BUTTON_UNCLICKABLE );      
      else if( b.getSprite() == ButtonSprite::BUTTON_SPRITE_SCOUT ||
          b.getSprite() == ButtonSprite::BUTTON_SPRITE_SCAVENGE ||
          b.getSprite() == ButtonSprite::BUTTON_SPRITE_CLEAR ) 
      b.setButtonState( ButtonState::BUTTON_VISIBLE );
      else b.setButtonState( ButtonState::BUTTON_INVISIBLE );
    }
  });
}

void Game::updateGSD()
{
  // time
  _gameStatsDisplay._gameStatsTexts._clockText = "Day : " + std::to_string( _time / 24 ) + "  Time : " + std::to_string( _time % 24 ) + " : 00";
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

int Game::computeTravelTime()
{
  SDL_Point buildingCoord = _clickedBuilding->getBuildingCoord();

  int xDif = abs( _player.getPosition().x - buildingCoord.x );
  int yDif = abs( _player.getPosition().y - buildingCoord.y );
  

  return xDif > yDif ? xDif / _gridWidth : yDif / _gridHeight;
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

TextDisplay Game::getCreditsMSG()
{
  return _creditsMSG;
}