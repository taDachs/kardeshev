#ifndef RENDER_H
#define RENDER_H
#include "SDL_ttf.h"
#include "game.h"
#include "solar_systems.h"
#include <SDL.h>
#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace kardeshev {

struct Color
{
  unsigned char r, g, b;
  unsigned char a = 0xFF;
  Color(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_ = 0xFF)
    : r(r_)
    , g(g_)
    , b(b_)
    , a(a_){};
};

const Color WHITE(0xFF, 0xFF, 0xFF);
const Color GREEN(0x00, 0xFF, 0x00);
const Color GRAY(15, 15, 15);
const Color RED(0xFF, 0, 0);
const Color BLACK(0, 0, 0);
const Color DYSTOPIC_GREEN(16, 69, 17);
const Color DYSTOPIC_YELLOW(204, 163, 27);

struct SDLException : public std::exception
{
  std::string s;
  SDLException()
    : s(SDL_GetError())
  {
  }
  ~SDLException() noexcept override = default; // Updated
  const char* what() const noexcept override { return s.c_str(); }
};


class Render;

class UIElement
{
public:
  virtual void handleEvent(SDL_Event* e) = 0;
  virtual void display(Render& renderer) const           = 0;
};

class PlanetUI : public UIElement
{
private:
  std::shared_ptr<Planet> m_planet;
  int m_x;
  int m_y;
  int m_total_x;
  int m_total_y;
  bool m_selected = false;

public:
  PlanetUI(std::shared_ptr<Planet> planet)
    : m_planet(std::move(planet))
  {
  }
  std::shared_ptr<Planet> getPlanet() const { return m_planet; }
  void handleEvent(SDL_Event* e) override;
  void display(Render& renderer) const override;
  void setX(int x) { m_x = x; }
  void setY(int y) { m_y = y; }
  void setTotalX(int x) { m_total_x = x; }
  void setTotalY(int y) { m_total_y = y; }
};

class Artist
{
public:
  virtual void draw(Render& renderer) = 0;
  virtual void handleEvent(SDL_Event* e) = 0;
};

class Render
{
private:
  bool m_active;
  SDL_Renderer* m_renderer = nullptr;
  SDL_Rect m_viewport;
  std::shared_ptr<Artist> m_artist;
  TTF_Font* m_sans =
    TTF_OpenFont("/home/max/.local/share/fonts/Ubuntu Mono Nerd Font Complete Mono.ttf", 52);

public:
  Render(SDL_Renderer* renderer, const SDL_Rect& viewport)
    : m_renderer(renderer)
    , m_viewport(viewport)
  {
  }
  void drawCircle(const int centre_x,
                  const int centre_y,
                  const int radius,
                  const Color& color = WHITE,
                  const bool filled  = false);
  void drawText(
    const int x, const int y, const int h, const std::string& text, const Color& color = WHITE);
  void drawRect(const int x,
                const int y,
                const int w,
                const int h,
                const bool filled  = false,
                const Color& color = WHITE);
  void display();
  void setArtist(std::shared_ptr<Artist> artist) { m_artist = std::move(artist); }
  void handleEvent(SDL_Event* e);
  int getWidth() const { return m_viewport.w; }
  int getHeight() const { return m_viewport.h; }
  int getOriginX() const { return m_viewport.x; }
  int getOriginY() const { return m_viewport.y; }
};

class SystemView : public Artist
{
private:
  std::shared_ptr<SolarSystem> m_system;
  std::shared_ptr<Game> m_game;
  std::vector<PlanetUI> m_planets;

public:
  SystemView(std::shared_ptr<Game> game)
    : m_game(std::move(game))
  {
  }
  void draw(Render& renderer) override;
  void setSystem(std::shared_ptr<SolarSystem> system) {
    m_system = std::move(system);
    m_planets.clear();
    for (auto& p : m_system->getPlanets()) {
      m_planets.emplace_back(p);
    }
  };
  void handleEvent(SDL_Event* e) override;
};

class SystemInfoViewArtist : public Artist
{
private:
  std::shared_ptr<Render> m_render;
  std::shared_ptr<SolarSystem> m_system;
  std::shared_ptr<Game> m_game;

public:
  SystemInfoViewArtist(std::shared_ptr<Game> game)
    : m_game(std::move(game))
  {
  }
  void draw(Render& renderer) override;
  void setSystem(std::shared_ptr<SolarSystem> system) { m_system = std::move(system); };
  void handleEvent(SDL_Event* e) override;
};

class GameWindow
{
private:
  SDL_Window* m_window         = nullptr;
  SDL_Renderer* m_renderer     = nullptr;
  int m_window_width           = 1280;
  int m_window_height          = 760;
  std::shared_ptr<Game> m_game = nullptr;

  constexpr const static double SIDEBAR_WIDTH_PERCENT = 0.3;
  constexpr const static double BOTTOM_HEIGHT_PERCENT = 0.3;

  std::shared_ptr<Render> m_sidebar_render;
  std::shared_ptr<Render> m_main_view_render;
  std::shared_ptr<Render> m_bottom_bar_render;

public:
  std::shared_ptr<Render> getSidebarRender() const { return m_sidebar_render; }
  std::shared_ptr<Render> getMainViewRender() const { return m_main_view_render; }
  std::shared_ptr<Render> getBottomBarRender() const { return m_bottom_bar_render; }
  void setGame(std::shared_ptr<Game> game) { m_game = std::move(game); }
  void init();
  void kill();
  void display();
};
} // namespace kardeshev


#endif // !RENDER_H
