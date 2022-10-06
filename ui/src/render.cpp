#include "ui/render.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "lib/planets.h"
#include "ui/ui_state.h"
#include "util/util.h"
#include <algorithm>
#include <exception>
#include <glm/glm.hpp>
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
                      const Font::Size size,
                      SDL_Rect dst,
                      const bool centered,
                      const bool wrapped,
                      const Color& color)
{
  TTF_Font* font_scaled = font.getFont(size);

  SDL_Surface* surface_message;

  if (wrapped)
  {
    surface_message =
      TTF_RenderText_Blended_Wrapped(font_scaled, text.c_str(), {color.r, color.g, color.b}, dst.w);
  }
  else
  {
    surface_message =
      TTF_RenderText_Blended(font_scaled, text.c_str(), {color.r, color.g, color.b, color.a});
  }
  if (surface_message == nullptr)
  {
    throw TTFException("Falied to render text to surface");
  }
  if (centered)
  {
    dst.x = dst.x + (dst.w / 2) - std::min(surface_message->w, dst.w) / 2;
    dst.y = dst.y + (dst.h / 2) - std::min(surface_message->h, dst.h) / 2;
  }
  dst.w                = std::min(surface_message->w, dst.w);
  dst.h                = std::min(surface_message->h, dst.h);
  SDL_Texture* message = SDL_CreateTextureFromSurface(m_render, surface_message);
  if (message == nullptr)
  {
    throw SDLException("Failed to create texture from text surface");
  }
  Texture tex(message);
  copyTexture(tex, nullptr, &dst);

  SDL_FreeSurface(surface_message);
  tex.closeTexture();
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

Font Render::loadFont(const std::string& path,
                      const int small_size,
                      const int medium_size,
                      const int large_size)
{
  TTF_Font* small  = TTF_OpenFont(path.c_str(), small_size);
  TTF_Font* medium = TTF_OpenFont(path.c_str(), medium_size);
  TTF_Font* large  = TTF_OpenFont(path.c_str(), large_size);
  Font f(small, medium, large);
  return f;
}

SDL_Rect
Render::getExpectedTextSize(const Font& font, const Font::Size size, const std::string& text)
{
  SDL_Rect dst;
  dst.x = 0;
  dst.y = 0;
  TTF_SizeText(font.getFont(size), text.c_str(), &dst.w, &dst.h);
  return dst;
}

SDL_Rect
Render::getExpectedWrappedTextSize(const Font& font, const Font::Size size, const std::string& text, int w)
{
  SDL_Rect dst;
  dst.x                 = 0;
  dst.y                 = 0;
  TTF_Font* font_scaled = font.getFont(size);

  SDL_Surface* surface_message;

  surface_message =
    TTF_RenderText_Blended_Wrapped(font_scaled, text.c_str(), {BLACK.r, BLACK.g, BLACK.b}, w);
  if (surface_message == nullptr) {
    throw TTFException("Failed rendering wrapped text for size estimation");
  }
  dst.w = surface_message->w;
  dst.h = surface_message->h;
  SDL_FreeSurface(surface_message);
  return dst;
}
