#include "ui/view.h"
#include "ui/assets.h"
#include "ui/entities.h"
#include "ui/render.h"
#include "ui/window.h"

using namespace kardeshev;
using namespace ui;

bool MoveableView::handleEventView(SDL_Event* e)
{
  if (View::handleEventView(e))
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
      m_scale *= m_scale_step;
      if (m_scale < m_max_scale)
      {
        m_offset.x *= m_scale_step;
        m_offset.y *= m_scale_step;
      }
      else
      {
        m_scale = m_max_scale;
      }
    }
    else if (e->wheel.y < 0)
    {
      m_scale /= m_scale_step;
      if (m_scale > m_min_scale)
      {
        m_offset.x /= m_scale_step;
        m_offset.y /= m_scale_step;
      }
      else
      {
        m_scale = m_min_scale;
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
  UI::render->copyTexture(border, nullptr, nullptr);
}

void GalaxyView::updateView()
{
  if (m_entities.empty())
  {
    std::vector<lib::SolarSystem::Ptr> systems = UI::game->getGalaxy()->getSystems();
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
  UI::render->copyTexture(border, nullptr, nullptr);
}

void SystemView::updateView()
{
  if (m_current_system != UI::state->focused_system)
  {
    m_entities.clear();
    m_current_system = UI::state->focused_system;
    m_scale          = 1;
    m_offset.x       = 0;
    m_offset.y       = 0;
  }
  if (m_entities.empty())
  {
    lib::Star::Ptr s = UI::state->focused_system->getStar();
    Entity::Ptr e    = std::make_shared<StarEntity>(s);
    m_entities.push_back(e);
    e->setOffset(getCenterOffset());
    e->setScale(m_scale);
    e->update();
    std::vector<lib::Planet::Ptr> planets = UI::state->focused_system->getPlanets();
    for (const auto& p : planets)
    {
      auto e = std::make_shared<PlanetEntity>(p);
      m_entities.push_back(e);
      e->setOffset(getCenterOffset());
      e->setScale(m_scale);
      e->update();
    }

    m_back_button = std::make_shared<BackButtonEntity>();
    m_entities.push_back(std::static_pointer_cast<Entity>(m_back_button));
    e->update();
  }
  else
  {
    SDL_Rect screen = UI::getRenderSize();

    int padding = 10;
    SDL_Rect button_dst;
    button_dst.y = screen.h - 64 - padding;
    button_dst.x = padding;
    button_dst.w = 128;
    button_dst.h = 64;
    m_back_button->setDst(button_dst);
    for (auto& e : m_entities)
    {
      e->setOffset(getCenterOffset());
      e->setScale(m_scale);
      e->update();
    }
  }
}

void GalaxyInfoView::updateView()
{
  if (m_entities.empty())
  {
    Entity::Ptr text = std::make_shared<GalaxyInfoEntity>();
    m_entities.push_back(text);
    m_settings_button = std::make_shared<SettingsButtonEntity>();
    m_entities.push_back(std::static_pointer_cast<Entity>(m_settings_button));
  }
  SDL_Rect size = UI::getRenderSize();
  SDL_Rect settings_button_dst;
  settings_button_dst.w = settings_button_dst.h = 0.1 * size.w;
  settings_button_dst.x                         = settings_button_dst.w;
  settings_button_dst.y                         = size.h - settings_button_dst.h * 1.5;
  m_settings_button->setDst(settings_button_dst);
  for (auto& e : m_entities)
  {
    e->update();
  }
}

void PlanetInfoView::updateView()
{
  if (m_entities.empty())
  {
    Entity::Ptr text = std::make_shared<PlanetInfoEntity>();
    m_entities.push_back(text);
  }
  for (auto& e : m_entities)
  {
    e->update();
  }
}

void LoadingScreenView::updateView()
{
  if (m_entities.empty())
  {
    Entity::Ptr text = std::make_shared<LoadingTextEntity>();
    m_entities.push_back(text);
    m_entity = std::static_pointer_cast<LoadingTextEntity>(text);
  }
  for (auto& e : m_entities)
  {
    e->update();
  }
}

void GameSettingsView::updateView()
{
  if (m_entities.empty())
  {
    m_title_label = std::make_shared<TextEntity>(m_title);
    m_entities.push_back(std::static_pointer_cast<Entity>(m_title_label));
    m_settings_button = std::make_shared<SettingsButtonEntity>();
    m_entities.push_back(std::static_pointer_cast<Entity>(m_settings_button));
    for (auto& o : UI::settings.ui_settings.toggle_options)
    {
      CheckBoxOptionEntity::Ptr option = std::make_shared<CheckBoxOptionEntity>(o.first, o.second);
      m_options.push_back(option);
      m_entities.push_back(std::static_pointer_cast<Entity>(option));
    }
  }
  SDL_Rect size = UI::getRenderSize();
  SDL_Rect settings_button_dst;
  settings_button_dst.w = settings_button_dst.h = 0.05 * size.w;
  settings_button_dst.x                         = settings_button_dst.w;
  settings_button_dst.y                         = size.h - settings_button_dst.h * 1.5;
  m_settings_button->setDst(settings_button_dst);
  m_settings_button->update();

  SDL_Rect title_dst;
  title_dst.w = size.w / 3.0; // 8:1 aspect ration for options
  title_dst.h = title_dst.w / 8.0;
  title_dst.x = size.w / 2 - title_dst.w / 2;
  title_dst.y = size.h * 0.2;
  m_title_label->setFont(UI::assets->getFont(Font::DEFAULT_FONT_LARGE));
  m_title_label->setDst(title_dst);
  m_title_label->update();

  SDL_Rect option_dst = title_dst;
  for (auto& o : m_options)
  {
    option_dst.y += option_dst.h * 1.5;
    o->setDst(option_dst);
    o->update();
  }
}

void MainMenuView::updateView()
{
  if (m_entities.empty())
  {
    m_title_label = std::make_shared<TextEntity>("Kardeshev");
    m_play_label = std::make_shared<PlayButton>();
    m_quit_label = std::make_shared<QuitButton>();

    m_entities.push_back(m_title_label);
    m_entities.push_back(m_play_label);
    m_entities.push_back(m_quit_label);
  }

  SDL_Rect size = UI::getRenderSize();

  SDL_Rect title_dst;
  title_dst.w = 500;
  title_dst.h = 100;
  title_dst.x = (size.w - title_dst.w) / 2;
  title_dst.y = size.h * 0.3;
  m_title_label->setFont(UI::assets->getFont(Font::DEFAULT_FONT_LARGE));
  m_title_label->setCentered(true);
  m_title_label->setDst(title_dst);
  m_title_label->setBoxed(true);
  m_title_label->setBoxBorder(true);
  m_title_label->setBoxColor(WHITE);
  m_title_label->setColor(BLACK);
  m_title_label->setBoxBorderColor(WHITE);
  m_title_label->update();


  SDL_Rect play_dst = title_dst;
  play_dst.y += title_dst.h * 2;
  m_play_label->setFont(UI::assets->getFont(Font::DEFAULT_FONT_LARGE));
  m_play_label->setDst(play_dst);
  m_play_label->update();


  SDL_Rect quit_dst = play_dst;
  quit_dst.y += play_dst.h * 2;
  m_quit_label->setFont(UI::assets->getFont(Font::DEFAULT_FONT_LARGE));
  m_quit_label->setDst(quit_dst);
  m_quit_label->update();
}
