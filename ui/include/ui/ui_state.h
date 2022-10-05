#ifndef UI_STATE_H
#define UI_STATE_H

#include "lib/game.h"
#include "lib/planets.h"
#include "lib/solar_systems.h"
#include "ui/assets.h"
#include "util/logger.h"
#include <SDL_image.h>
#include <SDL_render.h>
#include <memory>

namespace kardeshev {

// TODO: should be own file
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

struct TTFException : public std::exception
{
  std::string s;
  TTFException()
    : s(SDL_GetError())
  {
  }
  TTFException(const std::string& msg)
    : s("Error: " + msg + ", TTF_GetError(): " + TTF_GetError())
  {
  }
  ~TTFException() noexcept override = default; // Updated
  const char* what() const noexcept override { return s.c_str(); }
};


struct IMGException : public std::exception
{
  std::string s;
  IMGException()
    : s(SDL_GetError())
  {
  }
  IMGException(const std::string& msg)
    : s("Error: " + msg + ", IMG_GetError(): " + IMG_GetError())
  {
  }
  ~IMGException() noexcept override = default; // Updated
  const char* what() const noexcept override { return s.c_str(); }
};

class Screen;

// TODO: clean this all this up
struct UIState
{
  using Ptr                                   = std::shared_ptr<UIState>;

  std::shared_ptr<Planet> focused_planet      = nullptr;
  std::shared_ptr<SolarSystem> focused_system = nullptr;
  std::shared_ptr<Screen> current_screen = nullptr;
  bool paused = true;
};

struct UISettings
{
  bool scan_lines = true;
  bool color_filter = false;
  std::map<std::string, bool*> toggle_options;
};

struct Settings
{
  UISettings ui_settings;
};

struct ScreenList {
  std::shared_ptr<Screen> main_screen;
  std::shared_ptr<Screen> loading_screen;
  std::shared_ptr<Screen> settings_screen;
  std::shared_ptr<Screen> main_menu_screen;
};

struct UI
{
  static std::shared_ptr<UIState> state;
  static SDL_Renderer* render;
  static SDL_Window* window;
  static AssetHandler::Ptr assets;
  static std::shared_ptr<SDL_Rect> current_viewport;
  static SDL_Rect window_size;
  static Game::Ptr game;
  static Logger::Ptr logger;
  static bool running;
  static Settings settings;
  static ScreenList screen_list;

  static SDL_Rect getRenderSize();
};


} // namespace kardeshev

#endif // !UI_STATE_H
