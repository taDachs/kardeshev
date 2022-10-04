#ifndef SCREEN_H
#define SCREEN_H

#include "ui/assets.h"
#include "ui/render.h"
#include "ui/view.h"
#include "util/logger.h"
#include <SDL_events.h>

namespace kardeshev {

class Screen
{
public:
  using Ptr                              = std::shared_ptr<Screen>;
  virtual void draw()                    = 0;
  virtual bool handleEvent(SDL_Event* e) = 0;
  virtual void resize()                  = 0;
};

class MainScreen : public Screen
{
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

class SettingsScreen : public Screen
{
private:
  View::Ptr m_settings_view;
public:
  SettingsScreen();
  void draw() final;
  void resize() final;
  bool handleEvent(SDL_Event* e) final;
};


class LoadingScreen
  : public Screen
  , public LoggerOutput
{
public:
  using Ptr = std::shared_ptr<LoadingScreen>;

private:
  LoadingScreenView::Ptr m_loading_screen_view;

public:
  LoadingScreen() { m_loading_screen_view = std::make_shared<LoadingScreenView>(); };
  void draw() final;
  void resize() final{};
  bool handleEvent(SDL_Event* e) final { return false; };
  void logInfo(const std::string& s) override { m_loading_screen_view->setText(s); }
  void logError(const std::string& s) override { m_loading_screen_view->setText(s); }
  void logDebug(const std::string& s) override {}
};
} // namespace kardeshev

#endif // !SCREEN_H
