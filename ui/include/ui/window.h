#ifndef WINDOW_H
#define WINDOW_H
#include "lib/game.h"
#include "render.h"
#include "ui/assets.h"
#include "ui/ui_state.h"
#include "ui/view.h"

namespace kardeshev {

void initSDL();

class GameWindow
{
private:
  SDL_Window* m_window = nullptr;
  int m_window_width   = 1920;
  int m_window_height  = 1080;

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

  constexpr const static double SIDEBAR_WIDTH_PERCENT = 0.3;
  constexpr const static double BOTTOM_HEIGHT_PERCENT = 0.3;

  std::shared_ptr<SDL_Rect> m_sidebar_viewport;
  std::shared_ptr<SDL_Rect> m_main_view_viewport;
  std::shared_ptr<SDL_Rect> m_bottom_bar_viewport;

  void setViewports();


  View::Ptr m_galaxy_view;
  View::Ptr m_system_view;
  View::Ptr m_galaxy_info_view;
  View::Ptr m_bottom_bar_view;

  void renderScreen();
  void generateScanLineTex();
  void generateColorFilterTex();

public:
  void setupViews();
  void init();
  void kill();
  void display();
};
} // namespace kardeshev


#endif // !WINDOW_H
