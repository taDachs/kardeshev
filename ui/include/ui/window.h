#ifndef WINDOW_H
#define WINDOW_H
#include "lib/game.h"
#include "render.h"
#include "ui/assets.h"
#include "ui/screen.h"
#include "ui/ui_state.h"
#include "ui/view.h"

namespace kardeshev {

void initSDL();

class GameWindow
{
private:
  SDL_Texture* m_scan_line_texture = nullptr;
  unsigned int m_scan_line_step = 0;
  Color m_scan_lines_color = GRAY;
  int m_scan_lines_alpha = 64;
  int m_distance_between_scan_lines = 20;
  int m_scan_line_thickness = 20;
  double m_scan_line_speed = 0.5;

  SDL_Texture* m_color_filter_tex = nullptr;
  Color m_color_filter_color = DYSTOPIC_YELLOW;
  int m_color_filter_alpha = 32;
  Screen::Ptr m_last_screen = nullptr;

  void generateScanLineTex();
  void generateColorFilterTex();
public:
  GameWindow() {
    UI::screen_list.main_screen = std::make_shared<MainScreen>();
    LoadingScreen::Ptr loading_screen = std::make_shared<LoadingScreen>();
    UI::logger->addLogger(std::static_pointer_cast<LoggerOutput>(loading_screen));
    UI::screen_list.loading_screen = loading_screen;
    UI::screen_list.settings_screen = std::make_shared<SettingsScreen>();
    UI::state->current_screen = UI::screen_list.loading_screen;
  }
  void kill();
  void display();
  void handleEvents();
};
} // namespace kardeshev


#endif // !WINDOW_H
