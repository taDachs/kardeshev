#include "ui/view.h"
#include "ui/entities.h"
#include "ui/window.h"

using namespace kardeshev;

bool MoveableView::handleEvent(SDL_Event* e)
{
  if (View::handleEvent(e))
  {
    return true;
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
      m_scale *= 1.1;
      if (m_scale < 5.0) {
        m_offset.x *= 1.1;
        m_offset.y *= 1.1;
      } else {
        m_scale = 5.0;
      }
    }
    else if (e->wheel.y < 0)
    {
      m_scale /= 1.1;
      if (m_scale > 0.3) {
        m_offset.x /= 1.1;
        m_offset.y /= 1.1;
      } else {
        m_scale = 0.3;
      }
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
    unsigned int pressed = SDL_GetMouseState(&x, &y);
    if (!(pressed & SDL_BUTTON_LEFT))
    {
      m_left_mouse_pressed = false;
      return false;
    }
    m_offset.x += x - m_mouse_pos_on_click.x;
    m_offset.y += y - m_mouse_pos_on_click.y;
    m_mouse_pos_on_click.x = x;
    m_mouse_pos_on_click.y = y;
    return true;
  }

  return false;
}

void GalaxyView::drawView()
{
  View::drawView();
  Texture border = UI::assets->getTexture("main_view_border");
  SDL_RenderCopy(UI::render, border.getTexture(), nullptr, nullptr);
}

void GalaxyView::updateView()
{
  if (m_entities.empty())
  {
    std::vector<std::shared_ptr<SolarSystem> > systems = UI::game->getGalaxy()->getSystems();
    for (const auto& s : systems)
    {
      auto e = std::make_shared<SystemEntity>(s);
      m_entities.push_back(e);
      e->setOffset(getCenterOffset());
      e->setScale(m_scale);
      e->update();
    }
  }
  else
  {
    for (auto& e : m_entities)
    {
      e->setOffset(getCenterOffset());
      e->setScale(m_scale);
      e->update();
    }
  }
}

void SystemView::drawView()
{
  View::drawView();
  Texture border = UI::assets->getTexture("main_view_border");
  SDL_RenderCopy(UI::render, border.getTexture(), nullptr, nullptr);
}

void SystemView::updateView()
{
  if (m_current_system != UI::state->focused_system) {
    m_entities.clear();
    m_current_system = UI::state->focused_system;
    m_scale = 1;
    m_offset.x = 0;
    m_offset.y = 0;
  }
  if (m_entities.empty())
  {
    Star::Ptr s = UI::state->focused_system->getStar();
    Entity::Ptr e      = std::make_shared<StarEntity>(s);
    m_entities.push_back(e);
    e->setOffset(getCenterOffset());
    e->setScale(m_scale);
    e->update();
    std::vector<Planet::Ptr> planets = UI::state->focused_system->getPlanets();
    for (const auto& p : planets)
    {
      auto e = std::make_shared<PlanetEntity>(p);
      m_entities.push_back(e);
      e->setOffset(getCenterOffset());
      e->setScale(m_scale);
      e->update();
    }

    e = std::make_shared<BackButtonEntity>();
    m_entities.push_back(e);
    e->setOffset(getCenterOffset());
    e->setScale(m_scale);
    e->update();

  }
  else
  {
    for (auto& e : m_entities)
    {
      e->setOffset(getCenterOffset());
      e->setScale(m_scale);
      e->update();
    }
  }
}

void GalaxyInfoView::updateView() {
  if (m_entities.empty()) {
    Entity::Ptr text = std::make_shared<GalaxyInfoEntity>();
    m_entities.push_back(text);
  }
  for (auto& e : m_entities) {
    e->update();
  }
}
