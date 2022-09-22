#ifndef RENDER_H
#define RENDER_H
#include "solar_systems.h"
#include <SDL.h>
#include <exception>
#include <memory>
#include <string>
#include <utility>

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
const Color GRAY(15, 15, 15);
const Color RED(0xFF, 0, 0);

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

class Artist
{
public:
  virtual void draw(Render& renderer) const = 0;
};

class GameWindow {
public:
  void init();
  void kill();
  void display();
};

class Render
{
private:
  bool m_active;
  int m_window_width       = 800;
  int m_window_height      = 800;
  SDL_Window* m_window     = nullptr;
  SDL_Surface* m_surface   = nullptr;
  SDL_Renderer* m_renderer = nullptr;
  std::shared_ptr<Artist> m_artist;

public:
  void drawCircle(const int centre_x,
                  const int centre_y,
                  const int radius,
                  const Color& color = WHITE,
                  const bool filled  = false);
  void drawText(const int x,
                const int y,
                const int w,
                const int h,
                const std::string& text,
                const Color& color = WHITE);
  void init();
  void kill();
  void display();
  void setArtist(std::shared_ptr<Artist> artist) { m_artist = std::move(artist); }
  int getWidth() const { return m_window_width; }
  int getHeight() const { return m_window_height; }
};

class SystemView : public Artist
{
private:
  std::shared_ptr<Render> m_render;
  std::shared_ptr<SolarSystem> m_system;

public:
  void draw(Render& renderer) const override;
  void setSystem(std::shared_ptr<SolarSystem> system) { m_system = std::move(system); };
};
} // namespace kardeshev

#endif // !RENDER_H
