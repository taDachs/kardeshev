#ifndef WINDOW_H
#define WINDOW_H
#include "lib/game.h"
#include "render.h"

namespace kardeshev {
class GameWindow
{
private:
  SDL_Window* m_window             = nullptr;
  SDL_Renderer* m_renderer         = nullptr;
  int m_window_width               = 1920;
  int m_window_height              = 1080;
  std::shared_ptr<Game> m_game     = nullptr;
  std::shared_ptr<UIState> m_state = nullptr;

  constexpr const static double SIDEBAR_WIDTH_PERCENT = 0.3;
  constexpr const static double BOTTOM_HEIGHT_PERCENT = 0.3;

  std::shared_ptr<Render> m_sidebar_render;
  std::shared_ptr<Render> m_main_view_render;
  std::shared_ptr<Render> m_bottom_bar_render;

  std::shared_ptr<SDL_Rect> m_sidebar_viewport;
  std::shared_ptr<SDL_Rect> m_main_view_viewport;
  std::shared_ptr<SDL_Rect> m_bottom_bar_viewport;

  std::shared_ptr<Artist> m_system_view_artist;
  std::shared_ptr<Artist> m_galaxy_view_artist;
  void setViewports();

public:
  std::shared_ptr<Render> getSidebarRender() const { return m_sidebar_render; }
  std::shared_ptr<Render> getMainViewRender() const { return m_main_view_render; }
  std::shared_ptr<Render> getBottomBarRender() const { return m_bottom_bar_render; }
  void setSystemViewArtist(const std::shared_ptr<Artist>& system_view_artist)
  {
    m_system_view_artist = system_view_artist;
  }
  void setGalaxyViewArtist(const std::shared_ptr<Artist>& galaxy_view_artist)
  {
    m_galaxy_view_artist = galaxy_view_artist;
  }
  void setGame(std::shared_ptr<Game> game) { m_game = std::move(game); }
  void setUIState(std::shared_ptr<UIState> state) { m_state = std::move(state); }
  void init();
  void kill();
  void display();
};
} // namespace kardeshev


#endif // !WINDOW_H
