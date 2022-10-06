#ifndef WINDOW_H
#define WINDOW_H
#include "lib/game.h"
#include "render.h"
#include "ui/assets.h"
#include "ui/screen.h"
#include "ui/ui_state.h"
#include "ui/view.h"

namespace kardeshev {
namespace ui {


void setupScreens();

class GameWindow
{
public:
  using Ptr = std::shared_ptr<GameWindow>;

private:
  std::shared_ptr<Texture> m_scan_lines_tex;
  int m_scan_lines_step             = 0;
  Color m_scan_lines_color          = GRAY;
  unsigned char m_scan_lines_alpha  = 64;
  int m_distance_between_scan_lines = 20;
  int m_scan_lines_thickness        = 20;
  double m_scan_lines_speed         = 0.5;

  std::shared_ptr<Texture> m_color_filter_tex;
  Color m_color_filter_color         = DYSTOPIC_YELLOW;
  unsigned char m_color_filter_alpha = 32;

  Screen::Ptr m_last_screen = nullptr;

  void generateScanLineTex();
  void generateColorFilterTex();

public:
  void kill();
  void display();
  void handleEvents();
};
} // namespace ui
} // namespace kardeshev


#endif // !WINDOW_H
