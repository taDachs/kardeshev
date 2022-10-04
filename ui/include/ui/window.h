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
  bool m_scan_lines = true;
  SDL_Texture* m_scan_line_texture = nullptr;
  unsigned int m_scan_line_step = 0;
  Color m_scan_lines_color = GRAY;
  int m_scan_lines_alpha = 64;
  int m_distance_between_scan_lines = 20;
  int m_scan_line_thickness = 20;
  double m_scan_line_speed = 0.5;

  bool m_color_filter = false;
  SDL_Texture* m_color_filter_tex = nullptr;
  Color m_color_filter_color = DYSTOPIC_YELLOW;
  int m_color_filter_alpha = 32;

  Screen::Ptr m_main_screen;
  Screen::Ptr m_current_view;

  void generateScanLineTex();
  void generateColorFilterTex();
public:
  GameWindow() {
    m_main_screen = std::make_shared<MainScreen>();
    m_current_view = m_main_screen;
  }
  void kill();
  void display();
};
} // namespace kardeshev


#endif // !WINDOW_H
