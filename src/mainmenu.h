#ifndef MAINMENU_H
#define MAINMENU_H

#include "SDL.h"
#include "status.h"
#include "player.h"
#include "button.h"
#include "controller.h"
#include "renderer.h"
#include <vector>
#include <memory>
#include <algorithm>

class MainMenu
{
 public:
  MainMenu( std::size_t screen_width, std::size_t screen_height );
  void Run( Controller const &controller,
            Renderer &renderer,
            std::size_t target_frame_duration,
            Status &status );
  void update( Status &status );
 private:
  std::size_t _screenWidth;
  std::size_t _screenHeight;
  TextDisplay _title;

  void makeButtons();
  void updateButtons();
  void buttonAction( Status &status);

  std::vector<Button> _buttons;
  ButtonType _clickedButtonType;
};

#endif