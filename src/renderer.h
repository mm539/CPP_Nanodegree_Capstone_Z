#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "gfx.h"
#include "sprites.h"
#include "player.h"
#include <vector>
#include <memory>

class Renderer
{
 public:
  Renderer( const std::size_t screen_width, const std::size_t screen_height, const std::size_t grid_width, const std::size_t grid_height, 
  const std::size_t left_panel_width, const std::size_t top_panel_height,
  const std::size_t bottom_panel_height );
  ~Renderer();

  void renderAll( std::vector<std::shared_ptr<Building>>& buildings, Overlay& overlay, std::vector<Button>& buttons, Player& player );

 private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  const std::size_t left_panel_width;
  const std::size_t top_panel_height;
  const std::size_t bottom_panel_height;

};

#endif