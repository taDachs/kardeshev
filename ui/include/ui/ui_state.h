#ifndef UI_STATE_H
#define UI_STATE_H

#include "lib/game.h"
#include "lib/planets.h"
#include "lib/solar_systems.h"
#include "ui/assets.h"
#include "ui/render.h"
#include "util/logger.h"
#include <SDL_image.h>
#include <SDL_render.h>
#include <memory>

namespace kardeshev {
namespace ui {

class Screen;

// TODO: clean this all this up
struct UIState
{
  using Ptr = std::shared_ptr<UIState>;

  std::shared_ptr<lib::Planet> focused_planet      = nullptr;
  std::shared_ptr<lib::SolarSystem> focused_system = nullptr;
  std::shared_ptr<Screen> current_screen           = nullptr;
  bool paused                                      = true;
};

struct UISettings
{
  bool scan_lines   = true;
  bool color_filter = false;
  std::map<std::string, bool*> toggle_options;
};

struct Settings
{
  UISettings ui_settings;
};

struct ScreenList
{
  std::shared_ptr<Screen> main_screen;
  std::shared_ptr<Screen> loading_screen;
  std::shared_ptr<Screen> settings_screen;
  std::shared_ptr<Screen> main_menu_screen;
};

struct UI
{
  static std::shared_ptr<UIState> state;
  static Render::Ptr render;
  static SDL_Window* window;
  static AssetHandler::Ptr assets;
  static SDL_Rect window_size;
  static lib::Game::Ptr game;
  static util::Logger::Ptr logger;
  static bool running;
  static Settings settings;
  static ScreenList screen_list;

  static SDL_Rect getRenderSize();
};


} // namespace ui
} // namespace kardeshev

#endif // !UI_STATE_H
