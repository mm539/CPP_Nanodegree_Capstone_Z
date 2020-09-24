#include "game.h"
#include <algorithm>
#include <time.h>
#include <random>
#include <iostream>

Game::Game(std::size_t screen_width, std::size_t screen_height,
        std::size_t grid_width, std::size_t grid_height,
        std::size_t left_panel_width, std::size_t top_panel_height,
        std::size_t bottom_panel_height )
 : _sWidth( screen_width ), _sHeight( screen_height ),
   _gWidth( grid_width ), _gHeight( grid_height ),
   _leftPanelWidth( left_panel_width ), _topPanelHeight( top_panel_height ),
   _bottomPanelHeight( bottom_panel_height )
{
  makeBuildings();
  makeOverlay();
  makeButtons();
  makePlayer();
  _winTime = 24 * 4 + 24 * ( rand() % 3 ) + 9;
}


void Game::Run( Controller const &controller,
                Renderer &renderer,
                std::size_t target_frame_duration )
{
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  // int frame_count = 0; // yet to be implemented feature for FPS calculation
  bool running = true;

  while( running )
  {
    frame_start = SDL_GetTicks();

    // INPUT, UPDATE, RENDER
    controller.handleEvent( running, _buildings, _buttons, _clickedButtonSprite, _clickedBuilding );
    update( running );
    renderer.renderAll( _buildings, _overlay, _buttons, _player ); 

    frame_end = SDL_GetTicks();

    // frame_count++;
    frame_duration = frame_end - frame_start;

    if( frame_duration < target_frame_duration ) SDL_Delay( target_frame_duration - frame_duration );

    if( !running )
      endGame();
  }

}

void Game::endGame()
{
  if ( _player.getPlayerHealth() > 0 && _player.getHomeHealth() > 0 && _time >= _winTime )
  {
    std::cout << "You hear the sound of gunfire quickly approaching your location. As quickly as you can, you use your shirt and a long branch on the ground nearby to make a white flag to wave in the air. In the distance you hear faint shouts of what sounds like \"We've got another survivor! We've got another survivor!\" Soon, the soldiers make it to your location and beckon you onto a truck with what appears to be more survivors.\n  Congratulations! You survived!\n";
  }
  else if ( _player.getPlayerHealth() <= 0 )
  {
    std::cout << "You've died! Better luck next time!\n";
  }
  else if ( _player.getHomeHealth() <= 0 )
  {
    std::cout << "Your base has been overun! Better luck next time!\n";
  }
  else
  {
    std::cout << "You've quit the game.\n";
  }
  
  
  std::cout << "Game has terminated successfully!\n";
}

void Game::makeBuildings()
{
  // x * y is the total number of buildings
  int x = ( _sWidth - _leftPanelWidth ) / _gWidth;
  int y = ( _sHeight - _topPanelHeight - _bottomPanelHeight ) / _gHeight;

  srand( time( NULL ) );
  int rando = rand() % 2;
  _homeID = ( ( x * y ) / 2 + 3 ) + rando; // set location of home base
  
  for( int i = 0; i < y; i++ )
  {
    for( int j = 0; j < x; j++ )
    {
      if( ( i * y + j ) == _homeID )
      {
        _buildings.emplace_back( 
        std::shared_ptr<Building> ( new Building( i * y + j,
                  BUILDING_SPRITE_HOME,
                  _gWidth , _gHeight,
                  _leftPanelWidth + j * _gWidth, _topPanelHeight + i * _gHeight,
                  "../img/buildings-home128.bmp", 0, 0, 0) )  // int food, int materials, int dangerLevel
        );
        _clickedBuilding = _buildings.back();
      }
      else if( ( rand() % 4 ) <= 2 )
      {
        if( ( rand() % 11 ) <= 7 ) 
        {
          _buildings.emplace_back( 
          std::shared_ptr<Building> ( new Building( i * y + j,
                  BUILDING_SPRITE_HOUSE,
                  _gWidth , _gHeight,
                  _leftPanelWidth + j * _gWidth, _topPanelHeight + i * _gHeight,
                  "../img/buildings-house128.bmp",
                  10 + rand() % 6, 10 + rand() % 6, 15 + rand() % 10 ) ) // int food, int materials, int dangerLevel
          );
        }
        else if( rand() % 3 <= 1 )
        {
          _buildings.emplace_back( 
          std::shared_ptr<Building> ( new Building( i * y + j,
                  BUILDING_SPRITE_HSTORE,
                  _gWidth , _gHeight,
                  _leftPanelWidth + j * _gWidth, _topPanelHeight + i * _gHeight,
                  "../img/buildings-hstore128.bmp",
                  0, 15 + rand() % 30, 20 + rand() % 10 ) )
          );
        }
        else
        {
          _buildings.emplace_back( 
          std::shared_ptr<Building> ( new Building( i * y + j,
                  BUILDING_SPRITE_CSTORE,
                  _gWidth , _gHeight,
                  _leftPanelWidth + j * _gWidth, _topPanelHeight + i * _gHeight,
                  "../img/buildings-cstore128.bmp",
                  15 + rand() % 30, 0, 20 + rand() % 10 ) )
          );
        }
        
      }
      
    }
  }
}

void Game::makeOverlay()
{
  _overlay = Overlay( {0, 0 }, _leftPanelWidth, _sHeight );
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

void Game::update( bool& running )
{
  updateButtons(); // sets which buttons are visible, invisible, clickable according to 1. location of player 2. building clicked on
  buttonAction(); // performs the action set in _clickedButtonSprite
  updateOverlay();
 
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

  // check if player has won or lost the game
  if( _player.getPlayerHealth() <= 0 || _player.getHomeHealth() <= 0 ) running = false;
  else if ( _time >= _winTime ) running = false;
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
    _actionResultText = std::to_string( _time % 24 ) + " :00 : traveled to a building.";
    _time = _time + computeTravelTime();    
     _player.setPosition( _clickedBuilding->getBuildingCoord().x, _clickedBuilding->getBuildingCoord().y );
     _player.setLocationID( _clickedBuilding->getID() );
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
        if( playerHealth < 100) b.setButtonState( ButtonState::BUTTON_VISIBLE );
        else b.setButtonState( ButtonState::BUTTON_UNCLICKABLE );
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

void Game::updateOverlay()
{
  // time
  _overlay._texts._clockText = "Day : " + std::to_string( _time / 24 ) + "  Time : " + std::to_string( _time % 24 ) + " : 00";
  _overlay._texts._buildingText = std::to_string( 999 );

  // player stats
  _overlay._texts._playerHealthText = "Health : " + std::to_string( _player.getPlayerHealth() ) + " / 100" ;
  _overlay._texts._playerFoodText = "Food: " + std::to_string( _player.getPlayerFood() );
  _overlay._texts._playerMaterialsText = "Materials : " + std::to_string( _player.getPlayerMaterials() );

  // base stats
  _overlay._texts._homeHealthText = "Defenses : " + std::to_string( _player.getHomeHealth() ) + " / 100";
  _overlay._texts._homeFoodText = "Food : " + std::to_string( _player.getHomeFood() );
  _overlay._texts._homeMaterialsText = "Materials : " + std::to_string( _player.getHomeMaterials() );

  // selected building stats
  if( _player.getLocationID() == _homeID )
  {
    _overlay._texts._buildingDangerText = " ";
    _overlay._texts._buildingScoutedText = " ";
    _overlay._texts._buildingFoodText = " ";
    _overlay._texts._buildingMaterialsText = " ";
  }
  else
  {
    _overlay._texts._buildingDangerText = "Danger : " +  _clickedBuilding->getDangerText();
    _overlay._texts._buildingScoutedText = "Scouted : " + _clickedBuilding->getScoutedText();
    _overlay._texts._buildingFoodText = "Food : " +  _clickedBuilding->getFoodText();
    _overlay._texts._buildingMaterialsText = "Materials : " +  _clickedBuilding->getMaterialsText();
  }

  //  bottom panel display text
  _overlay._texts._actionResultText = _actionResultText;
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
  

  return xDif > yDif ? xDif / _gWidth : yDif / _gHeight;
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