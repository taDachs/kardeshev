#include "ui/entities.h"
#include "ui/render.h"
#include "ui/window.h"
#include "util/logger.h"
#include "util/util.h"
#include <sstream>

using namespace kardeshev;
using namespace ui;

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
  label_dst.x = m_position.x + icon_dst.w;
  label_dst.y = m_position.y - 20;
  label_dst.w = 300;
  label_dst.h = 30;
  m_number_planets_label->setDst(label_dst);
  label_dst.y -= 30;
  m_system_name_label->setDst(label_dst);

  m_system_name_label->setAlive(m_selected);
  m_number_planets_label->setAlive(m_selected);

  m_system_name_label->setBoxed(true);
  m_system_name_label->setBoxBorder(true);
  m_system_name_label->setBoxColor(WHITE);
  m_system_name_label->setBoxBorderColor(WHITE);
  m_system_name_label->setColor(BLACK);

  m_number_planets_label->setBoxed(true);
  m_number_planets_label->setBoxBorder(true);
  m_number_planets_label->setBoxColor(BLACK);
  m_number_planets_label->setBoxBorderColor(WHITE);
  m_number_planets_label->setFont(UI::assets->getFont(Font::DEFAULT_FONT_NOT_SO_SMALL));
  m_number_planets_label->setColor(WHITE);

  m_system_selected_icon->setAlive(m_selected);
  m_system_not_selected_icon->setAlive(!m_selected);
};

bool SystemEntity::handleEvent(SDL_Event* e)
{
  SDL_Point mouse;
  SDL_GetMouseState(&mouse.x, &mouse.y);

  SDL_Rect viewport = UI::getRenderSize();
  mouse.x -= viewport.x;
  mouse.y -= viewport.y;
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
  int orbit_radius = m_planet->getOrbitDistance().getInAU() * m_scale * AU_TO_PIXEL_SCALING;
  glm::vec2 cors =
    util::polarToCart(orbit_radius, m_planet->getCurrentAngle(UI::game->getTime()));
  m_position.x = cors.x;
  m_position.y = cors.y;
  int size = m_scale * m_planet->getMass().getInEarthMasses() * EARTH_MASS_TO_PIXEL_SCALING;
  SDL_Rect icon_dst;
  icon_dst.w = icon_dst.h = std::max(static_cast<double>(size), 5.0);
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

  SDL_Rect label_dst;
  label_dst.x = icon_dst.x + icon_dst.w + 10;
  label_dst.y = icon_dst.y - 80;
  label_dst.w = 400;
  label_dst.h = 30;
  m_planet_name_label->setDst(label_dst);
  m_planet_name_label->setBoxed(true);
  m_planet_name_label->setBoxColor(WHITE);
  m_planet_name_label->setBoxBorderColor(WHITE);
  m_planet_name_label->setColor(BLACK);
  m_planet_name_label->setAlive(selected);


  SDL_Rect description_dst;
  description_dst.x = label_dst.x;
  description_dst.y = label_dst.y + label_dst.h;
  description_dst.w = label_dst.w;
  description_dst.h = 400;
  m_description_box->setDst(description_dst);
  m_description_box->setBoxed(true);
  // m_description_box->setBoxColor(WHITE);
  m_description_box->setAlive(selected);
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

  SDL_Rect viewport = UI::getRenderSize();
  mouse.x -= viewport.x;
  mouse.y -= viewport.y;
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
  int size     = m_scale * std::min(m_star->getMass().getInEarthMasses() * EARTH_MASS_TO_PIXEL_SCALING, 300.0);
  m_position.x = 0;
  m_position.y = 0;

  SDL_Rect icon_dst;
  icon_dst.w = icon_dst.h = std::max(static_cast<double>(size), 5.0);
  icon_dst.x              = m_offset.x + m_position.x - size / 2;
  icon_dst.y              = m_offset.y + m_position.y - size / 2;
  m_selected_icon->setDst(icon_dst);
  m_selected_icon->setAlive(m_selected);
  m_not_selected_icon->setDst(icon_dst);
  m_not_selected_icon->setAlive(!m_selected);

  SDL_Rect label_dst;
  label_dst.x = icon_dst.x + icon_dst.w;
  label_dst.y = icon_dst.y - 20;
  label_dst.w = 200;
  label_dst.h = 30;
  m_star_name_label->setDst(label_dst);
  // m_star_name_label->setBoxColor(WHITE);
  m_star_name_label->setBoxed(true);
  m_star_name_label->setBoxBorder(true);
  m_star_name_label->setBoxColor(WHITE);
  m_star_name_label->setBoxBorderColor(WHITE);
  m_star_name_label->setColor(BLACK);
  m_star_name_label->setAlive(m_selected);
}

bool StarEntity::handleEvent(SDL_Event* e)
{
  SDL_Point mouse;
  SDL_GetMouseState(&mouse.x, &mouse.y);

  SDL_Rect viewport = UI::getRenderSize();
  mouse.x -= viewport.x;
  mouse.y -= viewport.y;

  bool selected = m_selected_icon->isUnderMouse(mouse.x, mouse.y);

  if (e->type == SDL_MOUSEMOTION)
  {
    m_selected = selected;

    // should not be counted as handled
    return false;
  }
  return false;
}

void ButtonEntity::update()
{
  m_button_not_selected->setDst(m_dst);
  m_button_selected->setDst(m_dst);

  m_button_not_selected->setDepth(0);
  m_button_selected->setDepth(0);

  m_button_selected->setAlive(m_selected);
  m_button_not_selected->setAlive(!m_selected);
}

bool ButtonEntity::handleEvent(SDL_Event* e)
{
  SDL_Point mouse;
  SDL_GetMouseState(&mouse.x, &mouse.y);

  SDL_Rect size = UI::getRenderSize();
  mouse.x -= size.x;
  mouse.y -= size.y;

  bool selected = m_button_not_selected->isUnderMouse(mouse.x, mouse.y);

  if (e->type == SDL_MOUSEMOTION)
  {
    m_selected = selected;

    // should not be counted as handled
    return false;
  }
  if (e->type == SDL_MOUSEBUTTONUP && selected)
  {
    onClick();
    return true;
  }
  return false;
}

void PlanetInfoEntity::generateText()
{
  std::stringstream s;
  s << "Planet Class: " << m_current_planet->getPlanetClass().getName() << "\n";
  s << "Class Description: " << m_current_planet->getPlanetClass().getDescription() << "\n";
  s << "Temperature: " << m_current_planet->getTemperature().getInC()  << " C\n";
  s << "Orbit Duration: " << m_current_planet->getOrbitDuration().getDays() << " Days\n";
  s << "Orbit Distance: " << m_current_planet->getOrbitDistance().getInAU() << " AU\n";
  m_text = s.str();
}

void PlanetInfoEntity::update()
{
  if (UI::state->focused_planet != m_current_planet)
  {
    m_current_planet = UI::state->focused_planet;
    if (m_current_planet != nullptr)
    {
      m_planet_name_label->setText("Name: " + m_current_planet->getName());
      generateText();
      m_info_box->setText(m_text);
    }
  }
  if (UI::state->focused_planet != nullptr)
  {
    SDL_Rect size = UI::getRenderSize();
    SDL_Rect name_dst;
    name_dst.x = name_dst.y = 0;
    name_dst.w              = size.w / 2;
    name_dst.h              = size.h * 0.1;
    m_planet_name_label->setDst(name_dst);
    m_planet_name_label->setAlive(true);
    m_planet_name_label->setFont(UI::assets->getFont(Font::DEFAULT_FONT_NOT_SO_SMALL));

    SDL_Rect info_dst;
    info_dst.x = 0;
    info_dst.y = name_dst.h;
    info_dst.w = size.w / 2;
    info_dst.h = size.h - name_dst.h;
    m_info_box->setFont(UI::assets->getFont(Font::DEFAULT_FONT_SMALL));
    m_info_box->setDst(info_dst);
    m_info_box->setAlive(true);

    SDL_Rect portrait_dst;
    portrait_dst.w = portrait_dst.h = std::min(size.w / 2, size.h);
    portrait_dst.x                  = size.w - portrait_dst.w;
    portrait_dst.y                  = 0;
    m_planet_portrait->setDst(portrait_dst);
    m_planet_portrait->setAlive(true);

    m_nothing_selected_label->setAlive(false);
  }
  else
  {
    m_planet_name_label->setAlive(false);
    m_info_box->setAlive(false);
    SDL_Rect nothing_label_dst = UI::getRenderSize();
    nothing_label_dst.x = nothing_label_dst.y = 0;
    m_nothing_selected_label->setDst(nothing_label_dst);
    m_nothing_selected_label->setAlive(true);
  }
}

void LoadingTextEntity::update()
{
  SDL_Rect size = UI::getRenderSize();
  SDL_Rect loading_label_dst;
  loading_label_dst.x = 0.05 * size.w;
  loading_label_dst.y = 0.95 * size.h;
  loading_label_dst.w = 0.3 * size.w;
  loading_label_dst.h = 0.05 * size.h;
  m_loading_label->setDst(loading_label_dst);
  m_loading_label->setText(util::Logger::getLastInfo());
  m_loading_label->setAlive(true);

  SDL_Rect title_label_dst;
  title_label_dst.x = 0.2 * size.w;
  title_label_dst.y = 0.2 * size.h;
  title_label_dst.w = 0.6 * size.w;
  title_label_dst.h = 0.6 * size.h;
  m_title_label->setDst(title_label_dst);
  m_title_label->setCentered(true);
  m_title_label->setAlive(true);
}


const std::string CheckBoxOptionEntity::CHECKBOX_SPRITE                = "checkbox";
const int CheckBoxOptionEntity::CHECKBOX_SPRITE_NOT_SELECTED_OFF_FRAME = 0;
const int CheckBoxOptionEntity::CHECKBOX_SPRITE_SELECTED_OFF_FRAME     = 1;
const int CheckBoxOptionEntity::CHECKBOX_SPRITE_NOT_SELECTED_ON_FRAME  = 2;
const int CheckBoxOptionEntity::CHECKBOX_SPRITE_SELECTED_ON_FRAME      = 3;

void CheckBoxOptionEntity::update()
{
  bool on = *m_val;

  SDL_Rect checkbox_dst;
  checkbox_dst.h = checkbox_dst.w = m_dst.h;
  checkbox_dst.x                  = m_dst.x + m_dst.w - m_dst.h;
  checkbox_dst.y                  = m_dst.y;
  m_checkbox_selected_on->setDst(checkbox_dst);
  m_checkbox_selected_off->setDst(checkbox_dst);
  m_checkbox_not_selected_on->setDst(checkbox_dst);
  m_checkbox_not_selected_off->setDst(checkbox_dst);
  m_checkbox_selected_on->setAlive(m_selected && on);
  m_checkbox_selected_off->setAlive(m_selected && !on);
  m_checkbox_not_selected_on->setAlive(!m_selected && on);
  m_checkbox_not_selected_off->setAlive(!m_selected && !on);

  SDL_Rect title_dst;
  title_dst.x = m_dst.x;
  title_dst.y = m_dst.y;
  title_dst.w = m_dst.w - checkbox_dst.w * 2;
  title_dst.h = m_dst.h;
  m_title_label->setDst(title_dst);
  m_title_label->setAlive(true);
}

bool CheckBoxOptionEntity::handleEvent(SDL_Event* e)
{
  SDL_Point mouse;
  SDL_GetMouseState(&mouse.x, &mouse.y);
  SDL_Rect size = UI::getRenderSize();

  mouse.x -= size.x;
  mouse.y -= size.y;

  bool selected = m_checkbox_not_selected_off->isUnderMouse(mouse.x, mouse.y);

  if (e->type == SDL_MOUSEMOTION)
  {
    m_selected = selected;

    return false;
  }
  if (e->type == SDL_MOUSEBUTTONUP)
  {
    if (selected)
    {
      *m_val = !(*m_val);
      return true;
    }
  }
  return false;
}

void TextEntity::update()
{
  m_label->setFont(m_font);
  m_label->setText(m_text);
  m_label->setDst(m_dst);
  m_label->setAlive(!m_wrapping);
  m_label->setCentered(m_centered);
  m_label->setBoxBorder(m_box_border);
  m_label->setBoxed(m_boxed);
  m_label->setBoxColor(m_box_color);
  m_label->setBoxBorderColor(m_box_border_color);
  m_label->setColor(m_color);

  m_box->setFont(m_font);
  m_box->setText(m_text);
  m_box->setDst(m_dst);
  m_box->setAlive(m_wrapping);
  m_box->setBoxBorder(m_box_border);
  m_box->setBoxed(m_boxed);
  m_box->setBoxColor(m_box_color);
  m_box->setBoxBorderColor(m_box_border_color);
  m_box->setColor(m_color);
}
