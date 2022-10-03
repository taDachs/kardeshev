#ifndef UI_STATE_H
#define UI_STATE_H

#include "lib/game.h"
#include "lib/planets.h"
#include "lib/solar_systems.h"
#include "ui/assets.h"
#include <SDL_render.h>
#include <memory>

namespace kardeshev {

struct SDLException : public std::exception
{
  std::string s;
  SDLException()
    : s(SDL_GetError())
  {
  }
  SDLException(const std::string& msg)
    : s("Error: " + msg + ", SDL_GetError(): " + SDL_GetError())
  {
  }
  ~SDLException() noexcept override = default; // Updated
  const char* what() const noexcept override { return s.c_str(); }
};

struct UIState
{
  using Ptr                                   = std::shared_ptr<UIState>;
  std::shared_ptr<Planet> focused_planet      = nullptr;
  std::shared_ptr<SolarSystem> focused_system = nullptr;
};

struct Context
{
  using Ptr = std::shared_ptr<Context>;
  double zoom_level;
  SDL_Rect camera_rect;
};

struct UI
{
  static std::shared_ptr<UIState> state;
  static SDL_Renderer* render;
  static AssetHandler::Ptr assets;
  static std::shared_ptr<SDL_Rect> current_viewport;
  static SDL_Rect window_size;
  static Game::Ptr game;
  static Context::Ptr current_context;
  static double zoom_level;
};


} // namespace kardeshev

#endif // !UI_STATE_H
