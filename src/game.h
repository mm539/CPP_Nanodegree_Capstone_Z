#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "status.h"
#include "player.h"
#include "sprites.h"
#include "controller.h"
#include "renderer.h"
#include <vector>
#include <memory>

class Game
{
 public:
  Game( int screen_width,
        int screen_height,
        int grid_width,
        int grid_height,
        PanelPD left_panel,
        PanelPD top_panel,
        PanelPD bottom_panel,
        PanelPD map_panel );
  void Run( Controller const &controller,
            Renderer &renderer,
            std::size_t target_frame_duration,
            Status &status );
  TextDisplay getCreditsMSG();

 private:
  // constructor functions
  void makeBuildings();
  void makeGSD(); // gameStatsDisplay
  void makeButtons();
  void makePlayer();

  // functions used to update the game state variables
  void endGame();
  void update( Status &status );
  void buttonAction();
  void updateButtons();
  void updateGSD();
  void clearBuilding();
  void scavengeBuilding();
  int computeTravelTime();

  SDL_Point getBuildingCoord( int id );

  // game state variables
  Player _player;
  std::vector<std::shared_ptr<Building>> _buildings;
  std::vector<Button> _buttons;

  ButtonSprite _clickedButtonSprite; // the most recently clicked button
  std::shared_ptr<Building> _clickedBuilding;
  int _time { 6 }; // game time. hours.
  int _winTime; // how many hours a player must survive to win the game.
  int _homeID; // the id of the home building

  // display information
  GameStatsDisplay _gameStatsDisplay;
  std::string _actionResultText { "Day 0" };

  int _screenWidth;
  int _screenHeight;
  int _gridWidth;
  int _gridHeight;
  PanelPD _leftPanel;
  PanelPD _bottomPanel;
  PanelPD _topPanel;
  PanelPD _mapPanel;

  TextDisplay _creditsMSG;
};

#endif