#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "gfx.h"
#include "sprites.h"
#include "player.h"
#include "status.h"
#include <vector>
#include <memory>

class Renderer
{
 public:
  Renderer( const int screen_width, const int screen_height, const int grid_width, const int grid_height );
  ~Renderer();

  void renderAll( std::vector<std::shared_ptr<Building>>& buildings, GameStatsDisplay& stats, std::vector<Button>& buttons, Player& player );
  void renderAll( std::vector<Button> &buttons );

 private:
  SDL_Window* _window;
  SDL_Renderer* _renderer;

  const int screen_width;
  const int screen_height;
  const int grid_width;
  const int grid_height;
};

#endif