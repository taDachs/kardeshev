#include "render.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "planets.h"
#include "util.h"
#include <algorithm>
#include <exception>
#include <glm/glm.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace kardeshev;

TTF_Font* Fonts::sans_large = nullptr;
TTF_Font* Fonts::sans_medium = nullptr;
TTF_Font* Fonts::sans_small = nullptr;

void Fonts::loadFonts() {
  Fonts::sans_large = TTF_OpenFont("/home/max/.local/share/fonts/Ubuntu Mono Nerd Font Complete Mono.ttf", 52);
  Fonts::sans_medium = TTF_OpenFont("/home/max/.local/share/fonts/Ubuntu Mono Nerd Font Complete Mono.ttf", 26);
  Fonts::sans_small = TTF_OpenFont("/home/max/.local/share/fonts/Ubuntu Mono Nerd Font Complete Mono.ttf", 12);
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

void GameWindow::init()
{
  if (TTF_Init() < 0)
  {
    std::cout << "TTF init failed" << std::endl;
    throw SDLException();
  }

  Fonts::loadFonts();

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
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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

  m_sidebar_viewport = std::make_shared<SDL_Rect>();
  m_sidebar_render   = std::make_shared<Render>(m_renderer, m_sidebar_viewport);

  m_main_view_viewport = std::make_shared<SDL_Rect>();
  m_main_view_render  = std::make_shared<Render>(m_renderer, m_main_view_viewport);

  m_bottom_bar_viewport = std::make_shared<SDL_Rect>();
  m_bottom_bar_render  = std::make_shared<Render>(m_renderer, m_bottom_bar_viewport);
  setViewports();
}

void GameWindow::setViewports() {
  m_sidebar_viewport->x = 0;
  m_sidebar_viewport->y = 0;
  m_sidebar_viewport->w = static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  m_sidebar_viewport->h = m_window_height;

  m_main_view_viewport->x = static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  m_main_view_viewport->y = 0;
  m_main_view_viewport->w = m_window_width - static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  m_main_view_viewport->h = m_window_height - static_cast<int>(m_window_height * BOTTOM_HEIGHT_PERCENT);

  m_bottom_bar_viewport->x = static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  m_bottom_bar_viewport->y =
    m_window_height - static_cast<int>(m_window_height * BOTTOM_HEIGHT_PERCENT);
  m_bottom_bar_viewport->w = m_window_width - static_cast<int>(m_window_width * SIDEBAR_WIDTH_PERCENT);
  m_bottom_bar_viewport->h = static_cast<int>(m_window_height * BOTTOM_HEIGHT_PERCENT);
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
        if (e.type == SDL_WINDOWEVENT) {
          if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
              std::cout << "MESSAGE:Resizing window..." << std::endl;
              /* resizeWindow(e.window.data1, e.window.data2); */
              m_window_width = e.window.data1;
              m_window_height = e.window.data2;
              setViewports();
          }
          break;
        }
        if (m_sidebar_render->handleEvent(&e))
        {
          break;
        }
        if (m_main_view_render->handleEvent(&e))
        {
          break;
        }
        if (m_bottom_bar_render->handleEvent(&e))
        {
          break;
        }
      }
    }
    if (m_state->focused_system == nullptr) {
      m_main_view_render->setArtist(m_galaxy_view_artist);
    } else {
      m_main_view_render->setArtist(m_system_view_artist);
    }
    m_game->step(Duration(1));
    SDL_SetRenderDrawColor(m_renderer, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
    SDL_RenderClear(m_renderer);
    m_sidebar_render->display();
    m_main_view_render->display();
    m_bottom_bar_render->display();
    SDL_RenderPresent(m_renderer);
    /* SDL_Delay(10); */
  }
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
    const int x, const int y, const int w, const int h, const std::string& text, const Color& color) {
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

void SystemView::draw(Render& renderer)
{
  if (m_current_system != m_state->focused_system) {
    m_current_system = m_state->focused_system;
    resetSystem();
  }
  std::vector<std::shared_ptr<Planet> > planets = m_current_system->getPlanets();
  int orbit                                     = 75;
  int mid_x                                     = renderer.getWidth() / 2 + m_offset.x;
  int mid_y                                     = renderer.getHeight() / 2 + m_offset.y;
  // draw sun
  renderer.drawCircle(mid_x, mid_y, 20 * m_zoom_level, RED);
  for (const auto& p : planets)
  {
    // draw orbit rings
    renderer.drawCircle(mid_x, mid_y, orbit * m_zoom_level, GRAY);
    // draws planet
    glm::vec2 cors = polarToCart(static_cast<double>(orbit * m_zoom_level),
                                 p->getInfo()->getCurrentAngle(m_game->getTime()));
    for (auto& uip : m_planets)
    {
      if (*uip.getPlanet() == *p)
      {
        uip.setX(mid_x + cors.x);
        uip.setY(mid_y + cors.y);
        uip.setTotalX(renderer.getOriginX() + mid_x + cors.x);
        uip.setTotalY(renderer.getOriginY() + mid_y + cors.y);
        uip.setZoomLevel(m_zoom_level);
        uip.display(renderer);
        break;
      }
    }

    orbit *= 1.5;
  }
  renderer.drawRect(0, 0, renderer.getWidth(), renderer.getHeight(), false, WHITE);
}

bool SystemView::handleEvent(SDL_Event* e)
{
  for (auto& pui : m_planets)
  {
    if (pui.handleEvent(e))
    {
      return true;
    }
  }
  if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT)
  {
    m_left_mouse_pressed = false;
    return true;
  }
  if (e->type == SDL_MOUSEWHEEL)
  {
    if (e->wheel.y > 0)
    {
      m_zoom_level *= 1.1;
      m_zoom_level = std::min(5.0, m_zoom_level);
    }
    else if (e->wheel.y < 0)
    {
      m_zoom_level /= 1.1;
      m_zoom_level = std::max(0.3, m_zoom_level);
    }
    return true;
  }
  if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
  {
    if (!m_left_mouse_pressed)
    {
      m_left_mouse_pressed = true;
      int x;
      int y;
      SDL_GetMouseState(&x, &y);
      m_mouse_pos_on_click.x = x;
      m_mouse_pos_on_click.y = y;
    }
    return true;
  }
  if (e->type == SDL_MOUSEMOTION && m_left_mouse_pressed)
  {
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    m_offset.x             += x - m_mouse_pos_on_click.x;
    m_offset.y             += y - m_mouse_pos_on_click.y;
    m_mouse_pos_on_click.x = x;
    m_mouse_pos_on_click.y = y;
    return true;
  }
  return false;
}


void GalaxyViewArtist::draw(Render& renderer)
{
  std::vector<std::shared_ptr<SolarSystem> > systems = m_galaxy->getSystems();
  int mid_x                                     = renderer.getWidth() / 2 + m_offset.x;
  int mid_y                                     = renderer.getHeight() / 2 + m_offset.y;
  for (const auto& s : systems)
  {
    // draws planet
    glm::vec2 cors = s->getInfo()->pos;
    for (auto& sip : m_systems)
    {
      if (*sip.getSystem() == *s)
      {
        sip.setX(mid_x + cors.x * m_zoom_level);
        sip.setY(mid_y + cors.y * m_zoom_level);
        sip.setTotalX(renderer.getOriginX() + mid_x + cors.x * m_zoom_level);
        sip.setTotalY(renderer.getOriginY() + mid_y + cors.y * m_zoom_level);
        sip.setZoomLevel(m_zoom_level);
        sip.display(renderer);
        break;
      }
    }

  }
  renderer.drawRect(0, 0, renderer.getWidth(), renderer.getHeight(), false, WHITE);
}

bool GalaxyViewArtist::handleEvent(SDL_Event* e)
{
  for (auto& pui : m_systems)
  {
    if (pui.handleEvent(e))
    {
      return true;
    }
  }
  if (e->type == SDL_MOUSEWHEEL)
  {
    if (e->wheel.y > 0)
    {
      m_zoom_level *= 1.1;
      m_zoom_level = std::min(10.0, m_zoom_level);
    }
    else if (e->wheel.y < 0)
    {
      m_zoom_level /= 1.1;
      m_zoom_level = std::max(1.0, m_zoom_level);
    }
    return true;
  }
  if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT)
  {
    m_left_mouse_pressed = false;
    return true;
  }
  if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
  {
    if (!m_left_mouse_pressed)
    {
      m_left_mouse_pressed = true;
      int x;
      int y;
      SDL_GetMouseState(&x, &y);
      m_mouse_pos_on_click.x = x;
      m_mouse_pos_on_click.y = y;
    }
    return true;
  }
  if (e->type == SDL_MOUSEMOTION && m_left_mouse_pressed)
  {
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    m_offset.x             += x - m_mouse_pos_on_click.x;
    m_offset.y             += y - m_mouse_pos_on_click.y;
    m_mouse_pos_on_click.x = x;
    m_mouse_pos_on_click.y = y;
    return true;
  }

  return false;
}

void SystemInfoViewArtist::draw(Render& renderer)
{
  renderer.drawText(
    10, 20, 15, "Time: " + std::to_string(m_game->getTime().getTicks()), DYSTOPIC_YELLOW);

  std::shared_ptr<SolarSystem> system = m_state->focused_system;

  if (system == nullptr) {
    renderer.drawText(
      10, 40, 20, "NO SYSTEM SELECTED", DYSTOPIC_YELLOW);
    return;
  }
  renderer.drawText(10,
                    40,
                    15,
                    "System Name: " + system->getInfo()->getNameOrId().substr(0, 10),
                    DYSTOPIC_YELLOW);

  std::vector<std::shared_ptr<Planet> > planets = system->getPlanets();
  renderer.drawText(
    10, 60, 15, "Number of Planets: " + std::to_string(planets.size()), DYSTOPIC_YELLOW);
  renderer.drawText(10, 80, 15, "Planets:", DYSTOPIC_YELLOW);

  int y = 100;
  for (const auto& p : planets)
  {
    renderer.drawText(40, y, 15, p->getInfo()->getNameOrId().substr(0, 10), DYSTOPIC_YELLOW);
    y += 20;
  }
  renderer.drawRect(0, 0, renderer.getWidth(), renderer.getHeight(), false, WHITE);
}

bool SystemInfoViewArtist::handleEvent(SDL_Event* e)
{
  return false;
}

bool PlanetUI::handleEvent(SDL_Event* e)
{
  int x;
  int y;
  SDL_GetMouseState(&x, &y);
  int dx = m_total_x - x;
  int dy = m_total_y - y;
  int d  = dx * dx + dy * dy;
  int r  = 10 * m_zoom_level;
  if (e->type == SDL_MOUSEMOTION)
  {
    // Get mouse position
    m_selected = d <= r * r;
    // should not be counted as handled
    return false;
  }
  if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
  {
    if (d <= r * r)
    {
      m_state->focused_planet = m_planet;
      return true;
    }
  }
  return false;
}

void PlanetUI::display(Render& renderer) const
{
  if (m_selected)
  {
    renderer.drawCircle(m_x, m_y, 10 * m_zoom_level, GREEN);
    renderer.drawText(m_x, m_y, 20, m_planet->getIdAsString().substr(0, 10), WHITE);
  }
  else if (m_planet == m_state->focused_planet)
  {
    renderer.drawCircle(m_x, m_y, 10 * m_zoom_level, DYSTOPIC_YELLOW);
  }
  else
  {
    renderer.drawCircle(m_x, m_y, 10 * m_zoom_level, WHITE);
  }
}

bool SystemUI::handleEvent(SDL_Event* e)
{
  int x;
  int y;
  SDL_GetMouseState(&x, &y);
  int dx = m_total_x - x;
  int dy = m_total_y - y;
  int d  = dx * dx + dy * dy;
  int r  = 1 * m_zoom_level;
  if (e->type == SDL_MOUSEMOTION)
  {
    // Get mouse position
    m_selected = d <= r * r;
    // should not be counted as handled
    return false;
  }
  if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
  {
    if (d <= r * r)
    {
      m_state->focused_system = m_system;
      return true;
    }
  }
  return false;
}

void SystemUI::display(Render& renderer) const
{
  if (m_selected)
  {
    renderer.drawCircle(m_x, m_y, 1 * m_zoom_level, GREEN);
    renderer.drawText(m_x, m_y - 40, 20, m_system->getIdAsString().substr(0, 10), DYSTOPIC_YELLOW);
    renderer.drawText(m_x, m_y - 20, 20, "Num Planets: " + std::to_string(m_system->getPlanets().size()), DYSTOPIC_YELLOW);
  }
  else
  {
    renderer.drawCircle(m_x, m_y, 1 * m_zoom_level, WHITE);
  }
}


void PlanetInfoViewArtist::draw(Render& renderer)
{
  if (m_state->focused_planet == nullptr)
  {
    renderer.drawText(10, 20, 40, "No planet selected", DYSTOPIC_YELLOW);
    return;
  }
  renderer.drawText(10,
                    20,
                    15,
                    "Planet Name: " +
                      m_state->focused_planet->getInfo()->getNameOrId().substr(0, 10),
                    DYSTOPIC_YELLOW);
  renderer.drawText(10,
                    40,
                    15,
                    "Planet Class: " + m_state->focused_planet->getInfo()->planet_class.getName(),
                    DYSTOPIC_YELLOW);
  std::vector<Population> pops = m_state->focused_planet->getPops();
  renderer.drawText(10, 60, 15, "Population Size: " + std::to_string(pops.size()), DYSTOPIC_YELLOW);
  sort(pops.begin(), pops.end(), [](const Population& x, const Population& y) {
    return (x.getReproductionProgress() > y.getReproductionProgress());
  });
  for (int i = 0; i < std::min(static_cast<int>(pops.size()), 6); ++i)
  {
    std::ostringstream rp_stream;
    rp_stream << "Species: " << pops[i].getSpecies().getName();
    rp_stream << std::fixed;
    rp_stream << std::setprecision(4);
    rp_stream << " Progress: " << pops[i].getReproductionProgress();
    renderer.drawText(10, 80 + i * 10, 12, rp_stream.str(), DYSTOPIC_YELLOW);
  }
  renderer.drawWrappedText(renderer.getWidth() * 0.5,
                    30,
                    renderer.getWidth() * 0.4,
                    100,
                    m_state->focused_planet->getInfo()->planet_class.getDescription(),
                    DYSTOPIC_YELLOW);

  renderer.drawRect(0, 0, renderer.getWidth(), renderer.getHeight(), false, WHITE);
}

bool PlanetInfoViewArtist::handleEvent(SDL_Event* e)
{
  if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
  {
    m_state->focused_system = nullptr;
    return true;
  }
  return false;
}
