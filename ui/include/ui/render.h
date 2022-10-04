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

} // namespace kardeshev


#endif // !RENDER_H
