#include "ui/render.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "lib/planets.h"
#include "util.h"
#include <algorithm>
#include <exception>
#include <glm/glm.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace kardeshev;

TTF_Font* Fonts::sans_large  = nullptr;
TTF_Font* Fonts::sans_medium = nullptr;
TTF_Font* Fonts::sans_small  = nullptr;

void Fonts::loadFonts()
{
  Fonts::sans_large =
    TTF_OpenFont("/home/max/.local/share/fonts/Ubuntu Mono Nerd Font Complete Mono.ttf", 52);
  Fonts::sans_medium =
    TTF_OpenFont("/home/max/.local/share/fonts/Ubuntu Mono Nerd Font Complete Mono.ttf", 26);
  Fonts::sans_small =
    TTF_OpenFont("/home/max/.local/share/fonts/Ubuntu Mono Nerd Font Complete Mono.ttf", 12);
}


bool kardeshev::isInRect(int x, int y, SDL_Rect rect)
{
  if (x < rect.x || x > rect.x + rect.w)
  {
    return false;
  }
  if (y < rect.y || y > rect.y + rect.h)
  {
    return false;
  }
  return true;
}


void Render::drawCircle(const int32_t centre_x,
                        const int32_t centre_y,
                        const int32_t radius,
                        const Color& color,
                        const bool filled)
{
  SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
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
      SDL_RenderDrawPoint(m_renderer, centre_x + x, centre_y - y);
      SDL_RenderDrawPoint(m_renderer, centre_x + x, centre_y + y);
      SDL_RenderDrawPoint(m_renderer, centre_x - x, centre_y - y);
      SDL_RenderDrawPoint(m_renderer, centre_x - x, centre_y + y);
      SDL_RenderDrawPoint(m_renderer, centre_x + y, centre_y - x);
      SDL_RenderDrawPoint(m_renderer, centre_x + y, centre_y + x);
      SDL_RenderDrawPoint(m_renderer, centre_x - y, centre_y - x);
      SDL_RenderDrawPoint(m_renderer, centre_x - y, centre_y + x);

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
  SDL_SetRenderDrawColor(m_renderer, WHITE.r, WHITE.b, WHITE.g, WHITE.a);
}

void Render::display()
{
  SDL_RenderSetViewport(m_renderer, m_viewport.get());
  if (m_artist == nullptr)
  {
    std::cout << "No artist set" << std::endl;
    return;
  }
  m_artist->draw(*this);
}

bool Render::handleEvent(SDL_Event* e)
{
  int x;
  int y;
  SDL_GetMouseState(&x, &y);
  if (!isInRect(x, y, *m_viewport))
  {
    return false;
  }
  return m_artist->handleEvent(e);
}


void Render::drawText(
  const int x, const int y, const int h, const std::string& text, const Color& color)
{
  SDL_Surface* surface_message =
    TTF_RenderText_Blended(Fonts::sans_medium, text.c_str(), {color.r, color.g, color.b});
  SDL_Texture* message = SDL_CreateTextureFromSurface(m_renderer, surface_message);
  SDL_Rect message_rect; // create a rect
  message_rect.x = x;
  message_rect.y = y;
  message_rect.w = text.size() * h;
  message_rect.h = h;
  SDL_RenderCopy(m_renderer, message, nullptr, &message_rect);
  SDL_FreeSurface(surface_message);

  SDL_DestroyTexture(message);
}
void Render::drawWrappedText(
  const int x, const int y, const int w, const int h, const std::string& text, const Color& color)
{
  SDL_Surface* surface_message =
    TTF_RenderText_Blended_Wrapped(Fonts::sans_small, text.c_str(), {color.r, color.g, color.b}, w);

  SDL_Texture* message = SDL_CreateTextureFromSurface(m_renderer, surface_message);
  SDL_Rect message_rect; // create a rect
  message_rect.x = x;
  message_rect.y = y;
  message_rect.w = w;
  message_rect.h = h;
  SDL_RenderCopy(m_renderer, message, nullptr, &message_rect);
  SDL_FreeSurface(surface_message);
}

void Render::drawRect(
  const int x, const int y, const int w, const int h, const bool filled, const Color& color)
{
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  SDL_SetRenderDrawColor(m_renderer, color.r, color.b, color.g, color.a);

  if (filled)
  {
    SDL_RenderFillRect(m_renderer, &rect);
  }
  else
  {
    SDL_RenderDrawRect(m_renderer, &rect);
  }
  SDL_SetRenderDrawColor(m_renderer, WHITE.r, WHITE.b, WHITE.g, WHITE.a);
}
