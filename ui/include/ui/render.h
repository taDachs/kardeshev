#ifndef RENDER_H
#define RENDER_H
#include "SDL_ttf.h"
#include "lib/game.h"
#include "lib/planets.h"
#include "lib/solar_systems.h"
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

bool isInRect(int x, int y, SDL_Rect rect);

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
const Color GRAY(30, 30, 30);
const Color SCAN_LINE_GRAY(128, 128, 128, 16);
const Color RED(0xFF, 0, 0);
const Color BLACK(0, 0, 0);
const Color DYSTOPIC_GREEN(16, 69, 17);
const Color DYSTOPIC_YELLOW(204, 163, 27);

void drawCircle(const int centre_x,
                const int centre_y,
                const int radius,
                const Color& color = WHITE,
                const bool filled  = false);

class Render;

class Artist
{
public:
  virtual void draw(Render& renderer)    = 0;
  virtual bool handleEvent(SDL_Event* e) = 0;
};

class Render
{
private:
  bool m_active;
  std::shared_ptr<Artist> m_artist;
  std::shared_ptr<SDL_Rect> m_viewport;

public:
  Render(std::shared_ptr<SDL_Rect> viewport)
    : m_viewport(std::move(viewport))
  {
  }
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

} // namespace kardeshev


#endif // !RENDER_H
