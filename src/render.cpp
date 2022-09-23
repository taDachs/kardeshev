#include "render.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "planets.h"
#include "util.h"
#include <exception>
#include <glm/glm.hpp>
#include <iostream>

using namespace kardeshev;

void GameWindow::init()
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
    std::cout << "sidebar renderer creation failed" << std::endl;
    throw SDLException();
  }

  SDL_Rect sidebar_viewport;
  sidebar_viewport.x = 0;
  sidebar_viewport.y = 0;
  sidebar_viewport.w = static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  sidebar_viewport.h = m_window_height;
  m_sidebar_render   = std::make_shared<Render>(m_renderer, sidebar_viewport);

  SDL_Rect mainview_viewport;
  mainview_viewport.x = static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  mainview_viewport.y = 0;
  mainview_viewport.w = m_window_width - static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  mainview_viewport.h = m_window_height - static_cast<int>(m_window_height * BOTTOM_HEIGHT_PERCENT);
  m_main_view_render  = std::make_shared<Render>(m_renderer, mainview_viewport);

  SDL_Rect bottombar_viewport;
  bottombar_viewport.x = static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  bottombar_viewport.y = m_window_height - static_cast<int>(m_window_height * BOTTOM_HEIGHT_PERCENT);
  bottombar_viewport.w = m_window_width - static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  bottombar_viewport.h = static_cast<int>(m_window_height * BOTTOM_HEIGHT_PERCENT);
  m_bottom_bar_render  = std::make_shared<Render>(m_renderer, bottombar_viewport);
}


void GameWindow::kill()
{
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
  SDL_Quit();
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
  SDL_RenderSetViewport(m_renderer, &m_viewport);
  if (m_artist == nullptr)
  {
    std::cout << "No artist set" << std::endl;
    return;
  }
  m_artist->draw(*this);
}

void Render::handleEvent(SDL_Event* e)
{
  m_artist->handleEvent(e);
}

void GameWindow::display()
{
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
      else
      {
        m_sidebar_render->handleEvent(&e);
        m_main_view_render->handleEvent(&e);
      }
    }
    m_game->step(Duration(1));
    SDL_SetRenderDrawColor(m_renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
    SDL_RenderClear(m_renderer);
    m_sidebar_render->display();
    m_main_view_render->display();
    SDL_RenderPresent(m_renderer);
    SDL_Delay(10);
  }
}

void Render::drawText(
  const int x, const int y, const int h, const std::string& text, const Color& color)
{
  SDL_Surface* surface_message =
    TTF_RenderText_Blended(m_sans, text.c_str(), {color.r, color.g, color.b});
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

void SystemView::draw(Render& renderer)
{
  std::vector<std::shared_ptr<Planet> > planets = m_system->getPlanets();
  int orbit                                     = 75;
  /* int size                                      = 5; */
  int mid_x = renderer.getWidth() / 2;
  int mid_y = renderer.getHeight() / 2;
  // draw sun
  renderer.drawCircle(mid_x, mid_y, 20, RED);
  for (const auto& p : planets)
  {
    // draw orbit rings
    renderer.drawCircle(mid_x, mid_y, orbit, GRAY);
    // draws planet
    glm::vec2 cors =
      polarToCart(static_cast<double>(orbit), p->getInfo()->getCurrentAngle(m_game->getTime()));
    for (auto& uip : m_planets)
    {
      if (*uip.getPlanet() == *p)
      {
        uip.setX(mid_x + cors.x);
        uip.setY(mid_y + cors.y);
        uip.setTotalX(renderer.getOriginX() + mid_x + cors.x);
        uip.setTotalY(renderer.getOriginY() + mid_y + cors.y);
        uip.display(renderer);
        break;
      }
    }
    /* renderer.drawCircle(mid_x + cors.x, mid_y + cors.y, size, WHITE); */
    /* renderer.drawText(mid_x + cors.x, mid_y + cors.y, 20, p->getIdAsString().substr(0, 10),
     * WHITE); */
    /* size += 2; */

    orbit *= 1.25;
  }
  renderer.drawRect(0, 0, renderer.getWidth(), renderer.getHeight(), false, WHITE);
}

void SystemView::handleEvent(SDL_Event* e)
{
  for (auto& pui : m_planets)
  {
    pui.handleEvent(e);
  }
}

void SystemInfoViewArtist::draw(Render& renderer)
{
  renderer.drawText(10, 20, 15, "Time: " + std::to_string(m_game->getTime().getTicks()), DYSTOPIC_YELLOW);
  renderer.drawText(10, 40, 15, "System Name: " + m_system->getInfo()->getNameOrId().substr(0, 10), DYSTOPIC_YELLOW);

  std::vector<std::shared_ptr<Planet> > planets = m_system->getPlanets();
  renderer.drawText(10, 60, 15, "Number of Planets: " + std::to_string(planets.size()), DYSTOPIC_YELLOW);
  renderer.drawText(10, 80, 15, "Planets:", DYSTOPIC_YELLOW);

  int y = 100;
  for (const auto& p : planets)
  {
    renderer.drawText(40, y, 15, p->getInfo()->getNameOrId().substr(0, 10), DYSTOPIC_YELLOW);
    y += 20;
  }
  renderer.drawRect(0, 0, renderer.getWidth(), renderer.getHeight(), false, WHITE);
}

void SystemInfoViewArtist::handleEvent(SDL_Event* e) {}

void PlanetUI::handleEvent(SDL_Event* e)
{
  if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
  {
    // Get mouse position
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    int dx = m_total_x - x;
    int dy = m_total_y - y;
    int d = dx * dx + dy * dy;
    m_selected = d <= 10 * 10;
  }
}

void PlanetUI::display(Render& renderer) const
{
  if (m_selected) {
    renderer.drawCircle(m_x, m_y, 10, GREEN);
    renderer.drawText(m_x, m_y, 20, m_planet->getIdAsString().substr(0, 10), WHITE);
  } else {
    renderer.drawCircle(m_x, m_y, 10, WHITE);
  }
}
