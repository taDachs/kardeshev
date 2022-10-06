#ifndef RENDER_H
#define RENDER_H
#include "SDL_ttf.h"
#include <SDL.h>
#include <glm/fwd.hpp>
#include <memory>
#include <string>
#include <ui/exceptions.h>
#include <vector>

namespace kardeshev {
namespace ui {

void initSDL();

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
const Color DARK_BLUE(14, 11, 97);
const Color TRANSPARENT(0, 0, 0, 0);

class Texture
{
private:
  SDL_Texture* m_texture;
  std::vector<SDL_Rect> m_frames;
  int m_w;
  int m_h;

public:
  Texture(SDL_Texture* texture)
    : m_texture(texture)
  {
    SDL_QueryTexture(texture, nullptr, nullptr, &m_w, &m_h);
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = m_w;
    r.h = m_h;
    m_frames.push_back(r);
  }

  Texture(SDL_Texture* texture, int w, int h, int frames)
    : m_texture(texture)
  {
    m_w = w;
    m_h = h;
    for (int i = 0; i < frames; i++)
    {
      SDL_Rect r;
      r.x = 0;
      r.y = i * h;
      r.h = h;
      r.w = w;
      m_frames.push_back(r);
    }
  }

  SDL_Texture* getTexture() const { return m_texture; }

  SDL_Rect getFrame(const int i) const { return m_frames.at(i); }

  void setTextureBlendMode(const SDL_BlendMode blend_mode)
  {
    if (SDL_SetTextureBlendMode(m_texture, blend_mode))
    {
      throw SDLException();
    }
  }

  void closeTexture() { SDL_DestroyTexture(m_texture); }
};

struct Font
{
  const static std::string DEFAULT_FONT;

  enum Size
  {
    SMALL,
    MEDIUM,
    LARGE
  };


  TTF_Font* small;
  TTF_Font* medium;
  TTF_Font* large;

  Font(TTF_Font* small_, TTF_Font* medium_, TTF_Font* large_)
    : small(small_)
    , medium(medium_)
    , large(large_)
  {
  }
  TTF_Font* getFont(Font::Size size) const
  {
    TTF_Font* f;
    switch (size)
    {
      case Font::Size::SMALL:
        f = small;
        break;
      case Font::Size::MEDIUM:
        f = medium;
        break;
      case Font::Size::LARGE:
        f = large;
        break;
    }
    return f;
  }
  void closeFont()
  {
    TTF_CloseFont(small);
    small = nullptr;
    TTF_CloseFont(medium);
    medium = nullptr;
    TTF_CloseFont(large);
    large = nullptr;
  }
};

class Render
{
public:
  using Ptr = std::shared_ptr<Render>;

private:
  SDL_Renderer* m_render;
  Color m_color = WHITE;
  std::shared_ptr<SDL_Rect> m_viewport;

public:
  Render(SDL_Renderer* render)
    : m_render(render)
  {
  }
  ~Render() { SDL_DestroyRenderer(m_render); }
  void setColor(const Color& color)
  {
    m_color = color;
    SDL_SetRenderDrawColor(m_render, m_color.r, m_color.g, m_color.b, m_color.a);
  }
  void drawPoint(const int x, const int y);
  void setViewport(const std::shared_ptr<SDL_Rect>& viewport);
  void copyTexture(const Texture& tex, SDL_Rect* src, SDL_Rect* dst);
  std::shared_ptr<SDL_Rect> getViewport() const { return m_viewport; }
  void drawText(const std::string& text,
                const Font& font,
                const Font::Size size,
                SDL_Rect dst,
                const bool centered = false,
                const bool wrapped  = false,
                const Color& color  = WHITE);
  void renderPresent();
  Texture createBlankTexture(const int w, const int h);
  void setTarget(const Texture& tex);
  void resetTarget();
  void clear();
  void drawLine(const int x1, const int y1, const int x2, const int y2);
  void
  drawCircle(const int centre_x, const int centre_y, const int radius, const bool filled = false);

  Texture loadTexture(const std::string& path);
  Font loadFont(const std::string& path,
                const int small_size,
                const int medium_size,
                const int large_size);
  void drawRect(const SDL_Rect& rect, const bool filled = false);
  SDL_Rect getExpectedTextSize(const Font& font, const Font::Size size, const std::string& text);
  SDL_Rect getExpectedWrappedTextSize(const Font& font, const Font::Size size, const std::string& text, int w);
};

} // namespace ui
} // namespace kardeshev


#endif // !RENDER_H
