#include "ui/screen.h"

using namespace kardeshev;

MainScreen::MainScreen() {
  m_main_view_viewport = std::make_shared<SDL_Rect>();
  m_galaxy_view        = std::make_shared<GalaxyView>();
  m_galaxy_view->setViewport(m_main_view_viewport);
  m_system_view = std::make_shared<SystemView>();
  m_system_view->setViewport(m_main_view_viewport);

  m_sidebar_viewport = std::make_shared<SDL_Rect>();
  m_galaxy_info_view = std::make_shared<GalaxyInfoView>();
  m_galaxy_info_view->setViewport(m_sidebar_viewport);

  m_bottom_bar_viewport = std::make_shared<SDL_Rect>();
  m_bottom_bar_view     = std::make_shared<PlanetInfoView>();
  m_bottom_bar_view->setViewport(m_bottom_bar_viewport);

  resize();
}

void MainScreen::resize()
{
  m_sidebar_viewport->x = 0;
  m_sidebar_viewport->y = 0;
  m_sidebar_viewport->w = static_cast<int>(UI::window_size.w * SIDEBAR_WIDTH_PERCENT);
  m_sidebar_viewport->h = UI::window_size.h;

  m_main_view_viewport->x = static_cast<int>(UI::window_size.w * SIDEBAR_WIDTH_PERCENT);
  m_main_view_viewport->y = 0;
  m_main_view_viewport->w =
    UI::window_size.w - static_cast<int>(UI::window_size.w * SIDEBAR_WIDTH_PERCENT);
  m_main_view_viewport->h =
    UI::window_size.h - static_cast<int>(UI::window_size.h * BOTTOM_HEIGHT_PERCENT);

  m_bottom_bar_viewport->x = static_cast<int>(UI::window_size.w * SIDEBAR_WIDTH_PERCENT);
  m_bottom_bar_viewport->y =
    UI::window_size.h - static_cast<int>(UI::window_size.h * BOTTOM_HEIGHT_PERCENT);
  m_bottom_bar_viewport->w =
    UI::window_size.w - static_cast<int>(UI::window_size.w * SIDEBAR_WIDTH_PERCENT);
  m_bottom_bar_viewport->h = static_cast<int>(UI::window_size.h * BOTTOM_HEIGHT_PERCENT);
}

void MainScreen::draw()
{
  if (UI::state->focused_system == nullptr)
  {
    m_galaxy_view->update();
    m_galaxy_view->draw();
  }
  else
  {
    m_system_view->update();
    m_system_view->draw();
  }

  m_galaxy_info_view->update();
  m_galaxy_info_view->draw();
  m_bottom_bar_view->update();
  m_bottom_bar_view->draw();
}

bool MainScreen::handleEvent(SDL_Event* e) {
  bool handeled;
  if (UI::state->focused_system == nullptr)
  {
    handeled = m_galaxy_view->handleEvent(e);
  }
  else
  {
    m_system_view->handleEvent(e);
  }
  if (!handeled)
  {
    handeled = m_galaxy_info_view->handleEvent(e);
  }
  if (!handeled)
  {
    handeled = m_bottom_bar_view->handleEvent(e);
  }
  return handeled;
}

