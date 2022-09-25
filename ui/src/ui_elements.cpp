#include "ui/ui_elements.h"

using namespace kardeshev;

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
    renderer.drawText(m_x,
                      m_y - 20,
                      20,
                      "Num Planets: " + std::to_string(m_system->getPlanets().size()),
                      DYSTOPIC_YELLOW);
  }
  else
  {
    renderer.drawCircle(m_x, m_y, 1 * m_zoom_level, WHITE);
  }
}
