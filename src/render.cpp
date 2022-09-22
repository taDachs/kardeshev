#include "render.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "planets.h"
#include "util.h"
#include <exception>
#include <glm/glm.hpp>
#include <iostream>

using namespace kardeshev;

void Render::init()
{
  if (TTF_Init() < 0)
  {
    std::cout << "TTF init failed" << std::endl;
    throw SDLException();
  }

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    std::cout << "SDL init failed" << std::endl;
    throw SDLException();
  }

  m_window = SDL_CreateWindow("Kardeshev",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              m_window_width,
                              m_window_height,
                              SDL_WINDOW_SHOWN);

  if (m_window == nullptr)
  {
    std::cout << "Window creation failed" << std::endl;
    throw SDLException();
  }
  m_renderer =
    SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (m_renderer == nullptr)
  {
    std::cout << "renderer creation failed" << std::endl;
    throw SDLException();
  }
}

void Render::kill()
{
  SDL_DestroyWindow(m_window);
  SDL_Quit();
}

void Render::drawCircle(const int32_t centre_x,
                        const int32_t centre_y,
                        const int32_t radius,
                        const Color& color,
                        const bool filled)
{
  SDL_SetRenderDrawColor(m_renderer, color.r, color.b, color.g, color.a);
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
  SDL_RenderClear(m_renderer);
  m_artist->draw(*this);
  SDL_RenderPresent(m_renderer);
  SDL_Event e;
  bool quit = false;
  while (!quit)
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
      {
        quit = true;
      }
    }
  }
}

void Render::drawText(
  const int x, const int y, const int w, const int h, const std::string& text, const Color& color)
{
  TTF_Font* sans =
    TTF_OpenFont("/home/max/.local/share/fonts/Ubuntu Mono Nerd Font Complete Mono.ttf", 52);
  std::cout << sans << std::endl;
  SDL_Surface* surfaceMessage =
    TTF_RenderText_Blended(sans, text.substr(0, 10).c_str(), {color.r, color.g, color.b});
  SDL_Texture* message = SDL_CreateTextureFromSurface(m_renderer, surfaceMessage);
  SDL_Rect message_rect; // create a rect
  /* surfaceMessage = TTF_RenderText_Shaded(sans, text.substr(0, 10).c_str(), {color.r, color.g,
   * color.b}, {0, 0, 0}); */
  message_rect.x = x;
  message_rect.y = y;
  message_rect.w = w;
  message_rect.h = h;
  SDL_RenderCopy(m_renderer, message, nullptr, &message_rect);
  SDL_FreeSurface(surfaceMessage);

  SDL_DestroyTexture(message);
}

void SystemView::draw(Render& renderer) const
{
  std::vector<std::shared_ptr<Planet> > planets = m_system->getPlanets();
  int orbit                                     = 75;
  int size                                      = 5;
  int mid_x                                     = renderer.getWidth() / 2;
  int mid_y                                     = renderer.getHeight() / 2;
  // draw sun
  renderer.drawCircle(mid_x, mid_y, 20, RED, true);
  for (const auto& p : planets)
  {
    // draw orbit rings
    renderer.drawCircle(mid_x, mid_y, orbit, GRAY);
    // draws planet
    int angle      = RandomDistribution::sample(0, 361);
    glm::vec2 cors = polarToCart(static_cast<double>(orbit), angle);
    renderer.drawCircle(mid_x + cors.x, mid_y + cors.y, size, WHITE, true);
    renderer.drawText(mid_x + cors.x, mid_y + cors.y, 100, 50, p->getIdAsString(), WHITE);
    size += 2;

    orbit *= 1.25;
  }
}
