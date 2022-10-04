#include "ui/entities.h"
#include "ui/window.h"
#include "util/util.h"
#include <sstream>

using namespace kardeshev;

const std::string SystemEntity::SYSTEM_SPRITE            = "system_simple";
const int SystemEntity::SYSTEM_SPRITE_SELECTED_FRAME     = 2;
const int SystemEntity::SYSTEM_SPRITE_NOT_SELECTED_FRAME = 0;

void SystemEntity::update()
{
  m_position.x = m_offset.x + m_scale * m_system->getInfo()->pos.x;
  m_position.y = m_offset.y + m_scale * m_system->getInfo()->pos.y;
  SDL_Rect icon_dst;
  icon_dst.w = icon_dst.h = std::max(m_scale * 5, 5.0);
  icon_dst.x              = m_position.x - icon_dst.w / 2;
  icon_dst.y              = m_position.y - icon_dst.h / 2;
  m_system_selected_icon->setDst(icon_dst);
  m_system_selected_icon->setDepth(250);
  m_system_not_selected_icon->setDst(icon_dst);
  m_system_not_selected_icon->setDepth(250);

  SDL_Rect label_dst;
  label_dst.x = m_position.x;
  label_dst.y = m_position.y - 20;
  label_dst.w = 100;
  label_dst.h = 20;
  m_system_name_label->setDst(label_dst);
  label_dst.y -= 20;
  m_number_planets_label->setDst(label_dst);

  m_system_name_label->setAlive(m_selected);
  m_number_planets_label->setAlive(m_selected);

  m_system_selected_icon->setAlive(m_selected);
  m_system_not_selected_icon->setAlive(!m_selected);
};

bool SystemEntity::handleEvent(SDL_Event* e)
{
  SDL_Point mouse;
  SDL_GetMouseState(&mouse.x, &mouse.y);

  if (UI::current_viewport != nullptr)
  {
    mouse.x -= UI::current_viewport->x;
    mouse.y -= UI::current_viewport->y;
  }
  // std::cout << "mouse x: " << mouse.x << ", y: " << mouse.y << std::endl;
  bool selected = m_system_not_selected_icon->isUnderMouse(mouse.x, mouse.y);

  if (e->type == SDL_MOUSEMOTION)
  {
    // Get mouse position
    m_selected = selected;

    // should not be counted as handled
    return false;
  }
  if (e->type == SDL_MOUSEBUTTONUP)
  {
    if (selected)
    {
      UI::state->focused_system = m_system;
      return true;
    }
  }
  return false;
}

const std::string PlanetEntity::PLANET_SPRITE            = "planet_simple";
const int PlanetEntity::PLANET_SPRITE_SELECTED_FRAME     = 2;
const int PlanetEntity::PLANET_SPRITE_FOCUSED_FRAME      = 1;
const int PlanetEntity::PLANET_SPRITE_NOT_SELECTED_FRAME = 0;

void PlanetEntity::update()
{
  bool selected    = m_selected || UI::state->focused_planet == m_planet;
  int orbit_radius = m_planet->getInfo()->orbit_distance * m_scale;
  glm::vec2 cors =
    polarToCart(orbit_radius, m_planet->getInfo()->getCurrentAngle(UI::game->getTime()));
  m_position.x = cors.x;
  m_position.y = cors.y;
  SDL_Rect icon_dst;
  icon_dst.w = icon_dst.h = m_scale * 20;
  icon_dst.x              = m_offset.x + m_position.x - icon_dst.w / 2;
  icon_dst.y              = m_offset.y + m_position.y - icon_dst.h / 2;

  m_selected_icon->setDst(icon_dst);
  m_selected_icon->setDepth(250);
  m_not_selected_icon->setDst(icon_dst);
  m_not_selected_icon->setDepth(250);
  m_focused_icon->setDst(icon_dst);
  m_focused_icon->setDepth(250);

  if (m_planet == UI::state->focused_planet)
  {
    m_selected_icon->setAlive(m_selected);
    m_not_selected_icon->setAlive(false);
    m_focused_icon->setAlive(!m_selected);
  }
  else
  {
    m_selected_icon->setAlive(m_selected);
    m_not_selected_icon->setAlive(!m_selected);
    m_focused_icon->setAlive(false);
  }

  SDL_Rect description_dst;
  description_dst.x = icon_dst.x + m_scale * 20 + 10;
  description_dst.y = icon_dst.y - 50;
  description_dst.w = 300;
  description_dst.h = 300;
  m_description_box->setDst(description_dst);
  m_description_box->setAlive(selected);

  SDL_Rect label_dst;
  label_dst.x = icon_dst.x + m_scale * 20 + 10;
  label_dst.y = icon_dst.y - 70;
  label_dst.w = 200;
  label_dst.h = 20;
  m_planet_name_label->setDst(label_dst);
  m_planet_name_label->setAlive(selected);

  SDL_Rect orbit_ring_dst;
  orbit_ring_dst.x = m_offset.x - orbit_radius;
  orbit_ring_dst.y = m_offset.y - orbit_radius;
  orbit_ring_dst.w = orbit_ring_dst.h = 2 * orbit_radius;
  m_orbit_ring->setDst(orbit_ring_dst);
}

bool PlanetEntity::handleEvent(SDL_Event* e)
{
  SDL_Point mouse;
  SDL_GetMouseState(&mouse.x, &mouse.y);

  mouse.x -= UI::current_viewport->x;
  mouse.y -= UI::current_viewport->y;
  bool selected = m_not_selected_icon->isUnderMouse(mouse.x, mouse.y);

  if (e->type == SDL_MOUSEMOTION)
  {
    // Get mouse position
    m_selected = selected;

    // should not be counted as handled
    return false;
  }
  if (e->type == SDL_MOUSEBUTTONDOWN)
  {
    if (selected)
    {
      if (UI::state->focused_planet == m_planet)
      {
        UI::state->focused_planet = nullptr;
      }
      else
      {
        UI::state->focused_planet = m_planet;
      }
      return true;
    }
  }
  return false;
}

const std::string StarEntity::STAR_SPRITE            = "star_simple";
const int StarEntity::STAR_SPRITE_SELECTED_FRAME     = 2;
const int StarEntity::STAR_SPRITE_NOT_SELECTED_FRAME = 0;

void StarEntity::update()
{
  int size     = m_scale * 80;
  m_position.x = 0;
  m_position.y = 0;

  SDL_Rect icon_dst;
  icon_dst.w = icon_dst.h = size;
  icon_dst.x              = m_offset.x + m_position.x - size / 2;
  icon_dst.y              = m_offset.y + m_position.y - size / 2;
  m_selected_icon->setDst(icon_dst);
  m_selected_icon->setAlive(m_selected);
  m_not_selected_icon->setDst(icon_dst);
  m_not_selected_icon->setAlive(!m_selected);

  SDL_Rect label_dst;
  label_dst.x = icon_dst.x;
  label_dst.y = icon_dst.y - 20;
  label_dst.w = 200;
  label_dst.h = 20;
  m_star_name_label->setDst(label_dst);
  m_star_name_label->setAlive(m_selected);
}

bool StarEntity::handleEvent(SDL_Event* e)
{
  SDL_Point mouse;
  SDL_GetMouseState(&mouse.x, &mouse.y);

  if (UI::current_viewport != nullptr)
  {
    mouse.x -= UI::current_viewport->x;
    mouse.y -= UI::current_viewport->y;
  }
  // std::cout << "mouse x: " << mouse.x << ", y: " << mouse.y << std::endl;
  bool selected = m_selected_icon->isUnderMouse(mouse.x, mouse.y);

  if (e->type == SDL_MOUSEMOTION)
  {
    m_selected = selected;

    // should not be counted as handled
    return false;
  }
  return false;
}

void BackButtonEntity::update()
{
  SDL_Rect screen;

  if (UI::current_viewport != nullptr)
  {
    screen = *UI::current_viewport;
  }
  else
  {
    screen = UI::window_size;
  }

  int padding = 10;
  SDL_Rect button_dst;
  button_dst.y = screen.h - 64 - padding;
  button_dst.x = padding;
  button_dst.w = 128;
  button_dst.h = 64;
  m_button_not_selected->setDst(button_dst);
  m_button_selected->setDst(button_dst);

  m_button_not_selected->setDepth(0);
  m_button_selected->setDepth(0);

  m_button_selected->setAlive(m_selected);
  m_button_not_selected->setAlive(!m_selected);
}

bool BackButtonEntity::handleEvent(SDL_Event* e)
{
  SDL_Point mouse;
  SDL_GetMouseState(&mouse.x, &mouse.y);

  if (UI::current_viewport != nullptr)
  {
    mouse.x -= UI::current_viewport->x;
    mouse.y -= UI::current_viewport->y;
  }
  // std::cout << "mouse x: " << mouse.x << ", y: " << mouse.y << std::endl;
  bool selected = m_button_not_selected->isUnderMouse(mouse.x, mouse.y);

  if (e->type == SDL_MOUSEMOTION)
  {
    m_selected = selected;

    // should not be counted as handled
    return false;
  }
  if (e->type == SDL_MOUSEBUTTONUP)
  {
    if (selected)
    {
      UI::state->focused_system = nullptr;
      return true;
    }
  }
  return false;
}

void PlanetInfoEntity::generateText() {
  PlanetInfo::Ptr info = m_current_planet->getInfo();
  std::stringstream s;
  s << "Planet Class: " << info->planet_class.getName() << "\n";
  s << "Class Description: " << info->planet_class.getDescription() << "\n";
  s << "Temperature: " << info->temperature - 273.0 << " C\n";
  s << "Orbit Duration: " << info->orbit_duration.getTicks() << " Days\n";
  s << "Orbit Distance: " << info->orbit_distance << " km\n";
  m_text = s.str();
}

void PlanetInfoEntity::update() {
  if (UI::state->focused_planet != m_current_planet) {
    m_current_planet = UI::state->focused_planet;
    if (m_current_planet != nullptr) {
      m_planet_name_label->setText("Planet Name: " + m_current_planet->getInfo()->getNameOrId().substr(0, 10));
      generateText();
      m_info_box->setText(m_text);
    }
  }
  if (UI::state->focused_planet != nullptr) {
    SDL_Rect size = UI::getRenderSize();
    SDL_Rect name_dst;
    name_dst.x = name_dst.y = 0;
    name_dst.w              = size.w / 2;
    name_dst.h              = size.h * 0.1;
    m_planet_name_label->setDst(name_dst);
    m_planet_name_label->setAlive(true);

    SDL_Rect info_dst;
    info_dst.x = 0;
    info_dst.y = name_dst.h;
    info_dst.w = size.w / 2;
    info_dst.h = size.h - name_dst.h;
    m_info_box->setDst(info_dst);
    m_info_box->setAlive(true);

    SDL_Rect portrait_dst;
    portrait_dst.w = portrait_dst.h = std::min(size.w / 2, size.h);
    portrait_dst.x = size.w - portrait_dst.w;
    portrait_dst.y = 0;
    m_planet_portrait->setDst(portrait_dst);
    m_planet_portrait->setAlive(true);

    m_nothing_selected_label->setAlive(false);
  } else {
    m_planet_name_label->setAlive(false);
    m_info_box->setAlive(false);
    SDL_Rect nothing_label_dst = UI::getRenderSize();
    nothing_label_dst.x = nothing_label_dst.y = 0;
    m_nothing_selected_label->setDst(nothing_label_dst);
    m_nothing_selected_label->setAlive(true);
  }
}

void LoadingTextEntity::update() {
  SDL_Rect size = UI::getRenderSize();
  SDL_Rect loading_label_dst;
  loading_label_dst.x = 0.05 * size.w;
  loading_label_dst.y = 0.95 * size.h;
  loading_label_dst.w = 0.3 * size.w;
  loading_label_dst.h = 0.05 * size.h;
  m_loading_label->setDst(loading_label_dst);
  m_loading_label->setAlive(true);

  SDL_Rect title_label_dst;
  title_label_dst.x = 0.4 * size.w;
  title_label_dst.y = 0.4 * size.h;
  title_label_dst.w = 0.6 * size.w;
  title_label_dst.h = 0.6 * size.h;
  m_title_label->setDst(title_label_dst);
  m_title_label->setAlive(true);
}
