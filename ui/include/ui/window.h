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

void setupScreens();

class GameWindow
{
public:
  using Ptr = std::shared_ptr<GameWindow>;

private:
  SDL_Texture* m_scan_line_texture  = nullptr;
  unsigned int m_scan_line_step     = 0;
  Color m_scan_lines_color          = GRAY;
  int m_scan_lines_alpha            = 64;
  int m_distance_between_scan_lines = 20;
  int m_scan_line_thickness         = 20;
  double m_scan_line_speed          = 0.5;

  SDL_Texture* m_color_filter_tex = nullptr;
  Color m_color_filter_color      = DYSTOPIC_YELLOW;
  int m_color_filter_alpha        = 32;

  Screen::Ptr m_last_screen = nullptr;

  void generateScanLineTex();
  void generateColorFilterTex();

public:
  void kill();
  void display();
  void handleEvents();
};
} // namespace kardeshev


#endif // !WINDOW_H
