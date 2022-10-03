#include "ui/render.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "lib/planets.h"
#include "ui/window.h"
#include "util/util.h"
#include <algorithm>
#include <exception>
#include <glm/glm.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace kardeshev;

void kardeshev::drawCircle(
  const int centre_x, const int centre_y, const int radius, const Color& color, const bool filled)
{
  SDL_SetRenderDrawColor(UI::render, color.r, color.g, color.b, color.a);
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
      SDL_RenderDrawPoint(UI::render, centre_x + x, centre_y - y);
      SDL_RenderDrawPoint(UI::render, centre_x + x, centre_y + y);
      SDL_RenderDrawPoint(UI::render, centre_x - x, centre_y - y);
      SDL_RenderDrawPoint(UI::render, centre_x - x, centre_y + y);
      SDL_RenderDrawPoint(UI::render, centre_x + y, centre_y - x);
      SDL_RenderDrawPoint(UI::render, centre_x + y, centre_y + x);
      SDL_RenderDrawPoint(UI::render, centre_x - y, centre_y - x);
      SDL_RenderDrawPoint(UI::render, centre_x - y, centre_y + x);

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
  SDL_SetRenderDrawColor(UI::render, WHITE.r, WHITE.b, WHITE.g, WHITE.a);
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


void Render::display()
{
  SDL_RenderSetViewport(UI::render, m_viewport.get());
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
  Font font = UI::assets->getFont(Font::DEFAULT_FONT);
  SDL_Surface* surface_message =
    TTF_RenderText_Blended(font.medium, text.c_str(), {color.r, color.g, color.b});
  SDL_Texture* message = SDL_CreateTextureFromSurface(UI::render, surface_message);
  SDL_Rect message_rect; // create a rect
  message_rect.x = x;
  message_rect.y = y;
  message_rect.w = text.size() * h;
  message_rect.h = h;
  SDL_RenderCopy(UI::render, message, nullptr, &message_rect);
  SDL_FreeSurface(surface_message);

  SDL_DestroyTexture(message);
}

void Render::drawWrappedText(
  const int x, const int y, const int w, const int h, const std::string& text, const Color& color)
{
  Font font = UI::assets->getFont(Font::DEFAULT_FONT);
  SDL_Surface* surface_message =
    TTF_RenderText_Blended_Wrapped(font.medium, text.c_str(), {color.r, color.g, color.b}, w);

  SDL_Texture* message = SDL_CreateTextureFromSurface(UI::render, surface_message);
  SDL_Rect message_rect; // create a rect
  message_rect.x = x;
  message_rect.y = y;
  message_rect.w = w;
  message_rect.h = h;
  SDL_RenderCopy(UI::render, message, nullptr, &message_rect);
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

  SDL_SetRenderDrawColor(UI::render, color.r, color.b, color.g, color.a);

  if (filled)
  {
    SDL_RenderFillRect(UI::render, &rect);
  }
  else
  {
    SDL_RenderDrawRect(UI::render, &rect);
  }
  SDL_SetRenderDrawColor(UI::render, WHITE.r, WHITE.b, WHITE.g, WHITE.a);
}
