#include "ui/render.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "lib/planets.h"
#include "ui/ui_state.h"
#include "util/logger.h"
#include "util/util.h"
#include <algorithm>
#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>

using std::endl;

using namespace kardeshev;
using namespace ui;

void kardeshev::ui::initSDL()
{
  if (TTF_Init() < 0)
  {
    throw TTFException("TTF init failed");
  }
  if (IMG_Init(IMG_INIT_PNG) < 0)
  {
    throw IMGException("Image init failed");
  }
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    throw SDLException("SDL init failed");
  }
  UI::window = SDL_CreateWindow("Kardeshev",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                UI::window_size.w,
                                UI::window_size.h,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (UI::window == nullptr)
  {
    throw SDLException("Window creation failed");
  }


  UI::render = std::make_shared<Render>(
    SDL_CreateRenderer(UI::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
}

void Render::drawCircle(const int centre_x, const int centre_y, const int radius, const bool filled)
{
  int r = radius;


  while (r)
  {
    int32_t diameter = (r * 2);
    int32_t x        = (r - 1);
    int32_t y        = 0;
    int32_t tx       = 1;
    int32_t ty       = 1;
    int32_t error    = (tx - diameter);
    while (x >= y)
    {
      //  Each of the following renders an octant of the circle
      drawPoint(centre_x + x, centre_y - y);
      drawPoint(centre_x + x, centre_y + y);
      drawPoint(centre_x - x, centre_y - y);
      drawPoint(centre_x - x, centre_y + y);
      drawPoint(centre_x + y, centre_y - x);
      drawPoint(centre_x + y, centre_y + x);
      drawPoint(centre_x - y, centre_y - x);
      drawPoint(centre_x - y, centre_y + x);

      if (error <= 0)
      {
        ++y;
        error += ty;
        ty += 2;
      }

      if (error > 0)
      {
        --x;
        tx += 2;
        error += (tx - diameter);
      }
    }
    if (!filled)
    {
      break;
    }
    r--;
  }
}

void Render::drawPoint(const int x, const int y)
{
  SDL_RenderDrawPoint(m_render, x, y);
}

void Render::setViewport(const std::shared_ptr<SDL_Rect>& viewport)
{
  if (SDL_RenderSetViewport(m_render, viewport.get()))
  {
    throw SDLException("Setting Viewport failed");
  }
  m_viewport = viewport;
}

void Render::copyTexture(const Texture& tex, SDL_Rect* src, SDL_Rect* dst)
{
  if (SDL_RenderCopy(m_render, tex.getTexture(), src, dst))
  {
    throw SDLException("Copy operation failed");
  }
}

void Render::drawText(const std::string& text,
                      const Font& font,
                      SDL_Rect dst,
                      const bool wrapped,
                      const Color& color)
{
  if (wrapped) {
    drawWrappedText(text, font, dst, color);
  } else {
    drawLabelText(text, font ,dst, color);
  }
}
void Render::drawLabelText(const std::string& text,
                           const Font& font,
                           SDL_Rect dst,
                           const Color& color)
{
  SDL_Rect expected_size = getExpectedTextSize(font, text);
  double size_scalar     = 1.0;
  // if (expected_size.w > dst.w)
  // {
  //   size_scalar = static_cast<double>(dst.w) / static_cast<double>(expected_size.w);
  // }
  // if (expected_size.h > dst.h)
  // {
  //   double height_scalar = static_cast<double>(dst.h) / static_cast<double>(expected_size.h);
  //   if (height_scalar < size_scalar)
  //   {
  //     size_scalar = height_scalar;
  //   }
  // }

  SDL_Rect c_dst;
  c_dst.x                  = dst.x;
  c_dst.y                  = dst.y;
  c_dst.w                  = 0;
  c_dst.h                  = 0;
  SDL_Texture* glyph_atlas = font.getAtlas();
  SDL_SetTextureColorMod(glyph_atlas, color.r, color.g, color.b);

  SDL_Rect src = font.getGlyphSrc('A');
  for (const auto& c : text)
  {
    if (c < ' ')
    {
      c_dst.x += src.w * size_scalar;
      continue;
    }
    src = font.getGlyphSrc(c);
    c_dst.w      = src.w * size_scalar;
    c_dst.h      = src.h * size_scalar;
    SDL_RenderCopy(m_render, glyph_atlas, &src, &c_dst);
    c_dst.x += c_dst.w * size_scalar;
  }
}

std::vector<std::string> Render::splitIntoLinesByLength(const std::string& text, const Font& font, int w) const {
  std::vector<std::string> lines;
  const SDL_Rect space = font.getGlyphSrc(' ');
  for (const auto& line :  util::splitString(text, "\n")) {
    int line_length = 0;
    std::vector<std::string> words;
    for (const auto& word : util::splitString(line, " ")) {
      int word_length = 0;
      for (const auto& c: word) {
        word_length += font.getGlyphSrc(c).w + m_character_spacing;
      }

      line_length += word_length + space.w;;
      if (line_length > w) {
        line_length = word_length;
        lines.push_back(util::joinLines(words, " "));
        words.clear();
      }
      words.push_back(word);
    }
    if (!words.empty()) {
      lines.push_back(util::joinLines(words, " "));
      words.clear();
    }
  }
  return lines;
}

std::string rectToString(SDL_Rect r) {
  return "x: " + std::to_string(r.x) + " y: " + std::to_string(r.y) + " w: " + std::to_string(r.w) + " h: " + std::to_string(r.h);
}

void Render::drawWrappedText(const std::string& text,
                             const Font& font,
                             SDL_Rect dst,
                             const Color& color)
{
  SDL_Rect expected_size = getExpectedWrappedTextSize(font, text, dst.w);
  // std::cout << "Text: " << text << std::endl;
  // std::cout << "Expected Size: " << rectToString(expected_size) << std::endl;
  // std::cout << "Target Size: " << rectToString(dst) << std::endl;
  double size_scalar     = 1.0;

  // if (expected_size.w > dst.w)
  // {
  //   size_scalar = static_cast<double>(dst.w) / static_cast<double>(expected_size.w);
  // }
  // if (expected_size.h > dst.h)
  // {
  //   double height_scalar = static_cast<double>(dst.h) / static_cast<double>(expected_size.h);
  //   if (height_scalar < size_scalar)
  //   {
  //     size_scalar = height_scalar;
  //   }
  // }
  // expected_size = getExpectedWrappedTextSize(font, text, dst.w * size_scalar);
  // std::cout << "Size Scalar: " << size_scalar << std::endl;

  // if (centered)
  // {
  //   dst.x = dst.x + (dst.w / 2) - std::min(expected_size.w, dst.w) / 2;
  //   dst.y = dst.y + (dst.h / 2) - std::min(expected_size.h, dst.h) / 2;
  // }
  // dst.w = std::min(expected_size.w, dst.w);
  // dst.h = std::min(expected_size.h, dst.h);

  SDL_Rect c_dst;
  c_dst.x                  = dst.x;
  c_dst.y                  = dst.y;
  c_dst.w                  = 0;
  c_dst.h                  = 0;
  SDL_Texture* glyph_atlas = font.getAtlas();
  SDL_SetTextureColorMod(glyph_atlas, color.r, color.g, color.b);

  SDL_Rect src = font.getGlyphSrc('A');
  for (const auto& line : splitIntoLinesByLength(text, font, dst.w)) {
    for (const auto& c : line)
    {
      if (c < ' ')
      {
        c_dst.x += src.w * size_scalar;
        continue;
      }
      src = font.getGlyphSrc(c);
      c_dst.w      = src.w * size_scalar;
      c_dst.h      = src.h * size_scalar;
      SDL_RenderCopy(m_render, glyph_atlas, &src, &c_dst);
      c_dst.x += c_dst.w * size_scalar;
    }
    c_dst.x = dst.x;
    c_dst.y += src.h * size_scalar;
  }
}

SDL_Rect Render::getExpectedTextSize(const Font& font, const std::string& text)
{
  SDL_Rect size;
  size.x = size.y = size.w = size.h = 0;

  SDL_Rect glyph;
  glyph.w = 0;
  for (const auto& c : text)
  {
    if (c < ' ')
    {
      size.w += glyph.w + m_character_spacing;
      continue;
    }
    glyph = font.getGlyphSrc(c);
    size.w += glyph.w;
    if (glyph.h > size.h)
    {
      size.h = glyph.h;
    }
  }
  return size;
}

SDL_Rect Render::getExpectedWrappedTextSize(const Font& font, const std::string& text, int w)
{
  SDL_Rect size;
  size.x = size.y = size.h = 0;
  size.w                   = w;

  SDL_Rect glyph = font.getGlyphSrc('A');
  size.h = splitIntoLinesByLength(text, font, w).size() * glyph.h;
  return size;
}
void Render::renderPresent()
{
  SDL_RenderPresent(m_render);
}

Texture Render::createBlankTexture(const int w, const int h)
{
  SDL_Texture* tex =
    SDL_CreateTexture(m_render, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);
  if (tex == nullptr)
  {
    throw SDLException("Blank Texture Creation failed");
  }
  return {tex};
}

void Render::setTarget(const Texture& tex)
{
  if (SDL_SetRenderTarget(m_render, tex.getTexture()))
  {
    throw SDLException("Setting render target failed");
  }
}

void Render::resetTarget()
{
  if (SDL_SetRenderTarget(m_render, nullptr))
  {
    throw SDLException("Resetting render target failed");
  }
}

void Render::clear()
{
  if (SDL_RenderClear(m_render))
  {
    throw SDLException("Clearing failed");
  }
}

void Render::drawLine(const int x1, const int y1, const int x2, const int y2)
{
  if (SDL_RenderDrawLine(m_render, x1, y1, x2, y2))
  {
    throw SDLException("Drawing line failed");
  }
}

Texture Render::loadTexture(const std::string& path)
{
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  if (loaded_surface == nullptr)
  {
    throw std::invalid_argument(path);
  }
  SDL_Texture* loaded_tex = SDL_CreateTextureFromSurface(m_render, loaded_surface);
  if (loaded_tex == nullptr)
  {
    throw SDLException("Creating texture from surface failed");
  }
  SDL_FreeSurface(loaded_surface);
  return {loaded_tex};
}

void Render::drawRect(const SDL_Rect& rect, const bool filled)
{
  if (filled)
  {
    if (SDL_RenderFillRect(m_render, &rect))
    {
      throw SDLException("Drawing rect failed");
    }
  }
  else
  {
    if (SDL_RenderDrawRect(m_render, &rect))
    {
      throw SDLException("Drawing rect failed");
    }
  }
}

Font Render::loadFont(const std::string& path, const int size)
{
  TTF_Font* font = TTF_OpenFont(path.c_str(), size);
  Font f(font, m_render);
  return f;
}
