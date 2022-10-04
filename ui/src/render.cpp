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
