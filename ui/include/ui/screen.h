#ifndef SCREEN_H
#define SCREEN_H

#include <SDL_events.h>
#include "ui/view.h"

namespace kardeshev {

class Screen {
public:
  using Ptr = std::shared_ptr<Screen>;
  virtual void draw() = 0;
  virtual bool handleEvent(SDL_Event* e) = 0;
  virtual void resize() = 0;
};

class MainScreen : public Screen {
private:
  constexpr const static double SIDEBAR_WIDTH_PERCENT = 0.3;
  constexpr const static double BOTTOM_HEIGHT_PERCENT = 0.3;

  std::shared_ptr<SDL_Rect> m_sidebar_viewport;
  std::shared_ptr<SDL_Rect> m_main_view_viewport;
  std::shared_ptr<SDL_Rect> m_bottom_bar_viewport;

  View::Ptr m_galaxy_view;
  View::Ptr m_system_view;
  View::Ptr m_galaxy_info_view;
  View::Ptr m_bottom_bar_view;
public:
  MainScreen();
  void draw() final;
  void resize() final;
  bool handleEvent(SDL_Event* e) final;
};

}

#endif // !SCREEN_H
