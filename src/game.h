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
  Game( std::size_t screen_width,
        std::size_t screen_height,
        std::size_t grid_width,
        std::size_t grid_height,
        std::size_t left_panel_width,
        std::size_t top_panel_height,
        std::size_t bottom_panel_height );
  void Run( Controller const &controller,
            Renderer &renderer,
            std::size_t target_frame_duration,
            Status &status );

 private:
  // constructor functions
  void makeBuildings();
  void makeOverlay();
  void makeButtons();
  void makePlayer();

  // functions used to update the game state variables
  void endGame();
  void update( Status &status );
  void buttonAction();
  void updateButtons();
  void updateOverlay();
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
  Overlay _overlay;
  std::string _actionResultText { "Day 0" };

  std::size_t _sWidth;
  std::size_t _sHeight;
  std::size_t _gWidth;
  std::size_t _gHeight;
  std::size_t _leftPanelWidth;
  std::size_t _topPanelHeight;
  std::size_t _bottomPanelHeight;
};

#endif