#ifndef SCREEN_H
#define SCREEN_H

#include "SDL.h"
#include "status.h"
#include "player.h"
#include "button.h"
#include "controller.h"
#include "renderer.h"
#include <vector>
#include <memory>
#include <algorithm>

/*
  MASTER CLASS
*/
class Screen
{
 public:
  Screen(){}
  virtual void childRender( Renderer &renderer ) = 0;
  void Run( Controller const &controller,
            Renderer &renderer,
            std::size_t target_frame_duration,
            Status &status,
            EScreen desiredScreen );
  void update( Status &status );
  
 protected:
  std::size_t _screenWidth;
  std::size_t _screenHeight;
  virtual void makeButtons() = 0;
  virtual void updateButtons() = 0;
  virtual void buttonAction( Status &status) = 0;

  std::vector<Button> _buttons;
  ButtonType _clickedButtonType;
};

/*
  Derived Classes
*/

class MainMenu: public Screen
{
  public:
    MainMenu( std::size_t screen_width, std::size_t screen_height );
    void childRender( Renderer &renderer );
    void makeButtons() override;
    void updateButtons() override;
    void buttonAction( Status &status) override;
  private:
    TextDisplay _title;
};

class LoadingScreen: public Screen
{
  public:
    LoadingScreen( std::size_t screen_width, std::size_t screen_height );
    void childRender( Renderer &renderer );
    void makeButtons() override;
    void updateButtons() override;
    void buttonAction( Status &status) override;
  private:
    TextDisplay _msg;
};

class CreditScreen: public Screen
{
  public:
    CreditScreen( std::size_t screen_width, std::size_t screen_height, TextDisplay msg );
    void childRender( Renderer &renderer );
    void makeButtons() override;
    void updateButtons() override;
    void buttonAction( Status &status) override;
  private:
    TextDisplay _msg;
};

#endif