#include "ui/artists.h"
#include <iomanip>
#include <sstream>

using namespace kardeshev;

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

void SystemInfoViewArtist::draw(Render& renderer)
{
  renderer.drawText(
    10, 20, 15, "Time: " + std::to_string(m_game->getTime().getTicks()), DYSTOPIC_YELLOW);

  std::shared_ptr<SolarSystem> system = m_state->focused_system;

  if (system == nullptr)
  {
    renderer.drawText(10, 40, 20, "NO SYSTEM SELECTED", DYSTOPIC_YELLOW);
    return;
  }
  renderer.drawText(
    10, 40, 15, "System Name: " + system->getInfo()->getNameOrId().substr(0, 10), DYSTOPIC_YELLOW);

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

void SystemView::draw(Render& renderer)
{
  if (m_current_system != m_state->focused_system)
  {
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
    m_offset.x += x - m_mouse_pos_on_click.x;
    m_offset.y += y - m_mouse_pos_on_click.y;
    m_mouse_pos_on_click.x = x;
    m_mouse_pos_on_click.y = y;
    return true;
  }
  return false;
}


void GalaxyViewArtist::draw(Render& renderer)
{
  std::vector<std::shared_ptr<SolarSystem> > systems = m_galaxy->getSystems();
  int mid_x                                          = renderer.getWidth() / 2 + m_offset.x;
  int mid_y                                          = renderer.getHeight() / 2 + m_offset.y;
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
    m_offset.x += x - m_mouse_pos_on_click.x;
    m_offset.y += y - m_mouse_pos_on_click.y;
    m_mouse_pos_on_click.x = x;
    m_mouse_pos_on_click.y = y;
    return true;
  }

  return false;
}
