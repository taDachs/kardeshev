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

  constexpr const static double SIDEBAR_WIDTH_PERCENT = 0.3;
  constexpr const static double BOTTOM_HEIGHT_PERCENT = 0.3;

  std::shared_ptr<SDL_Rect> m_sidebar_viewport;
  std::shared_ptr<SDL_Rect> m_main_view_viewport;
  std::shared_ptr<SDL_Rect> m_bottom_bar_viewport;

  // std::shared_ptr<Artist> m_system_view_artist;
  // std::shared_ptr<Artist> m_galaxy_view_artist;
  void setViewports();


  View::Ptr m_galaxy_view;
  View::Ptr m_system_view;


public:
  void setupViews();
  void init();
  void kill();
  void display();
};
} // namespace kardeshev


#endif // !WINDOW_H
