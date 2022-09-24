#ifndef RENDER_H
#define RENDER_H
#include "SDL_ttf.h"
#include "galaxy.h"
#include "game.h"
#include "solar_systems.h"
#include <SDL.h>
#include <exception>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace kardeshev {

// it's either this or implementing a UI framework
struct UIState
{
  std::shared_ptr<Planet> focused_planet      = nullptr;
  std::shared_ptr<SolarSystem> focused_system = nullptr;
};

bool isInRect(int x, int y, SDL_Rect rect);

struct Fonts
{
  static TTF_Font* sans_small;
  static TTF_Font* sans_medium;
  static TTF_Font* sans_large;
  static void loadFonts();
};

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
protected:
  std::shared_ptr<UIState> m_state = nullptr;

public:
  UIElement(std::shared_ptr<UIState> state)
    : m_state(std::move(state))
  {
  }

  virtual bool handleEvent(SDL_Event* e)       = 0;
  virtual void display(Render& renderer) const = 0;
};

class PlanetUI : public UIElement
{
private:
  std::shared_ptr<Planet> m_planet;
  int m_x;
  int m_y;
  int m_total_x;
  int m_total_y;
  bool m_selected     = false;
  double m_zoom_level = 1;


public:
  PlanetUI(std::shared_ptr<UIState> state, std::shared_ptr<Planet> planet)
    : UIElement(std::move(state))
    , m_planet(std::move(planet))
  {
  }
  std::shared_ptr<Planet> getPlanet() const { return m_planet; }
  bool handleEvent(SDL_Event* e) override;
  void display(Render& renderer) const override;
  void setX(int x) { m_x = x; }
  void setY(int y) { m_y = y; }
  void setTotalX(int x) { m_total_x = x; }
  void setTotalY(int y) { m_total_y = y; }
  void setZoomLevel(double zoom_level) { m_zoom_level = zoom_level; }
};

class SystemUI : public UIElement
{
private:
  std::shared_ptr<SolarSystem> m_system;
  int m_x;
  int m_y;
  int m_total_x;
  int m_total_y;
  bool m_selected     = false;
  double m_zoom_level = 1;

public:
  SystemUI(std::shared_ptr<UIState> state, std::shared_ptr<SolarSystem> system)
    : UIElement(std::move(state))
    , m_system(std::move(system))
  {
  }
  std::shared_ptr<SolarSystem> getSystem() const { return m_system; }
  bool handleEvent(SDL_Event* e) override;
  void display(Render& renderer) const override;
  void setX(int x) { m_x = x; }
  void setY(int y) { m_y = y; }
  void setTotalX(int x) { m_total_x = x; }
  void setTotalY(int y) { m_total_y = y; }
  void setZoomLevel(double zoom_level) { m_zoom_level = zoom_level; }
};


class Artist
{
protected:
  std::shared_ptr<UIState> m_state = nullptr;

public:
  Artist(std::shared_ptr<UIState> state)
    : m_state(std::move(state)){};
  virtual void draw(Render& renderer)    = 0;
  virtual bool handleEvent(SDL_Event* e) = 0;
};

class Render
{
private:
  bool m_active;
  SDL_Renderer* m_renderer = nullptr;
  std::shared_ptr<Artist> m_artist;
  std::shared_ptr<SDL_Rect> m_viewport;

public:
  Render(SDL_Renderer* renderer, std::shared_ptr<SDL_Rect> viewport)
    : m_renderer(renderer)
    , m_viewport(std::move(viewport))
  {
  }
  void drawCircle(const int centre_x,
                  const int centre_y,
                  const int radius,
                  const Color& color = WHITE,
                  const bool filled  = false);
  void drawText(
    const int x, const int y, const int h, const std::string& text, const Color& color = WHITE);
  void drawWrappedText(const int x,
                       const int y,
                       const int w,
                       const int h,
                       const std::string& text,
                       const Color& color = WHITE);
  void drawRect(const int x,
                const int y,
                const int w,
                const int h,
                const bool filled  = false,
                const Color& color = WHITE);
  void display();
  void setArtist(std::shared_ptr<Artist> artist) { m_artist = std::move(artist); }
  bool handleEvent(SDL_Event* e);
  int getWidth() const { return m_viewport->w; }
  int getHeight() const { return m_viewport->h; }
  int getOriginX() const { return m_viewport->x; }
  int getOriginY() const { return m_viewport->y; }
};

class SystemView : public Artist
{
private:
  std::shared_ptr<Game> m_game;
  std::shared_ptr<SolarSystem> m_current_system;
  std::vector<PlanetUI> m_planets;
  double m_zoom_level            = 1;
  glm::vec2 m_offset             = {0, 0};
  glm::vec2 m_mouse_pos_on_click = {0, 0};
  bool m_left_mouse_pressed      = false;
  void resetSystem()
  {
    m_planets.clear();
    for (auto& p : m_state->focused_system->getPlanets())
    {
      m_planets.emplace_back(m_state, p);
    }
  }

public:
  SystemView(std::shared_ptr<UIState> state, std::shared_ptr<Game> game)
    : Artist(std::move(state))
    , m_game(std::move(game))
  {
  }
  void draw(Render& renderer) override;
  bool handleEvent(SDL_Event* e) override;
};

class GalaxyViewArtist : public Artist
{
private:
  std::shared_ptr<Galaxy> m_galaxy;
  std::shared_ptr<Game> m_game;
  std::vector<SystemUI> m_systems;
  double m_zoom_level            = 1;
  glm::vec2 m_offset             = {0, 0};
  glm::vec2 m_mouse_pos_on_click = {0, 0};
  bool m_left_mouse_pressed      = false;

public:
  GalaxyViewArtist(std::shared_ptr<UIState> state, std::shared_ptr<Game> game)
    : Artist(std::move(state))
    , m_game(std::move(game))
  {
  }
  void draw(Render& renderer) override;
  void setGalaxy(std::shared_ptr<Galaxy> galaxy)
  {
    m_galaxy = std::move(galaxy);
    m_systems.clear();
    for (auto& s : m_galaxy->getSystems())
    {
      m_systems.emplace_back(m_state, s);
    }
  };
  bool handleEvent(SDL_Event* e) override;
};


class SystemInfoViewArtist : public Artist
{
private:
  std::shared_ptr<Game> m_game;

public:
  SystemInfoViewArtist(std::shared_ptr<UIState> state, std::shared_ptr<Game> game)
    : Artist(std::move(state))
    , m_game(std::move(game))
  {
  }
  void draw(Render& renderer) override;
  bool handleEvent(SDL_Event* e) override;
};

class PlanetInfoViewArtist : public Artist
{
private:
  std::shared_ptr<Game> m_game;

public:
  PlanetInfoViewArtist(std::shared_ptr<UIState> state, std::shared_ptr<Game> game)
    : Artist(std::move(state))
    , m_game(std::move(game))
  {
  }
  void draw(Render& renderer) override;
  bool handleEvent(SDL_Event* e) override;
};

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

  std::shared_ptr<SystemView> m_system_view_artist;
  std::shared_ptr<GalaxyViewArtist> m_galaxy_view_artist;
  void setViewports();

public:
  std::shared_ptr<Render> getSidebarRender() const { return m_sidebar_render; }
  std::shared_ptr<Render> getMainViewRender() const { return m_main_view_render; }
  std::shared_ptr<Render> getBottomBarRender() const { return m_bottom_bar_render; }
  void setSystemViewArtist(const std::shared_ptr<SystemView>& system_view_artist)
  {
    m_system_view_artist = system_view_artist;
  }
  void setGalaxyViewArtist(const std::shared_ptr<GalaxyViewArtist>& galaxy_view_artist)
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


#endif // !RENDER_H
