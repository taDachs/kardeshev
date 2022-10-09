#ifndef VIEW_H
#define VIEW_H

#include "entities.h"
#include "ui/assets.h"
#include "ui_state.h"
#include "util/logger.h"
#include <utility>

namespace kardeshev {
namespace ui {


class View
{
public:
  using Ptr = std::shared_ptr<View>;

protected:
  std::vector<Entity::Ptr> m_entities;
  std::shared_ptr<SDL_Rect> m_viewport = nullptr;

  virtual void updateView()
  {
    for (auto& entity : m_entities)
    {
      entity->update();
    }
  }

  virtual void drawView()
  {
    std::vector<Component::Ptr> draw_list;
    for (auto& e : m_entities)
    {
      std::vector<Component::Ptr> entity_list = e->getDrawList();
      draw_list.insert(draw_list.end(), entity_list.begin(), entity_list.end());
    }

    std::sort(
      draw_list.begin(), draw_list.end(), [&](const Component::Ptr& a, const Component::Ptr& b) {
        return a->getDepth() > b->getDepth();
      });

    for (auto& d : draw_list)
    {
      d->draw();
    }
  };
  virtual bool handleEventView(SDL_Event* e)
  {
    for (auto& entity : m_entities)
    {
      if (entity->handleEvent(e))
      {
        return true;
      }
    }
    return false;
  }

public:
  void draw()
  {
    if (m_viewport != nullptr)
    {
      UI::render->setViewport(m_viewport);
    }
    drawView();
    if (m_viewport != nullptr)
    {
      UI::render->setViewport(nullptr);
    }
  }
  void update()
  {
    if (m_viewport != nullptr)
    {
      UI::render->setViewport(m_viewport);
    }
    updateView();
    if (m_viewport != nullptr)
    {
      UI::render->setViewport(nullptr);
    }
  }
  virtual bool handleEvent(SDL_Event* e)
  {
    if (m_viewport != nullptr)
    {
      SDL_Point p;
      SDL_GetMouseState(&p.x, &p.y);
      if (!SDL_PointInRect(&p, m_viewport.get()))
      {
        return false;
      }
    }
    UI::render->setViewport(m_viewport);
    bool handled         = handleEventView(e);
    UI::render->setViewport(nullptr);
    return handled;
  }

  virtual void setViewport(std::shared_ptr<SDL_Rect> viewport) { m_viewport = std::move(viewport); }
};

class MoveableView : public View
{
protected:
  SDL_Point m_offset;
  SDL_Point m_mouse_pos_on_click;
  double m_scale            = 1;
  bool m_left_mouse_pressed = false;

  double m_max_scale  = 12.0;
  double m_min_scale  = 0.05;
  double m_scale_step = 1.05;

  SDL_Point getCenterOffset()
  {
    SDL_Point offset = m_offset;
    SDL_Rect size = UI::getRenderSize();
    offset.x += size.w / 2;
    offset.y += size.h / 2;
    return offset;
  }
  bool handleEventView(SDL_Event* e) override;

public:
  void setViewport(std::shared_ptr<SDL_Rect> viewport) override
  {
    m_viewport = std::move(viewport);
  }
};

class GalaxyView : public MoveableView
{
private:
  void drawView() override;

protected:
  void updateView() override;
};

class SystemView : public MoveableView
{
private:
  ButtonEntity::Ptr m_back_button;
  lib::SolarSystem::Ptr m_current_system = nullptr;

  void drawView() override;
  void updateView() override;
};

class GalaxyInfoView : public View
{
private:
  ButtonEntity::Ptr m_settings_button;
  void updateView() override;
};

class PlanetInfoView : public View
{
  void updateView() override;
};

class LoadingScreenView : public View
{
public:
  using Ptr = std::shared_ptr<LoadingScreenView>;

private:
  void updateView() override;
  LoadingTextEntity::Ptr m_entity;
  std::string m_text;
};

class GameSettingsView : public View
{
public:
  using Ptr = std::shared_ptr<GameSettingsView>;

private:
  ButtonEntity::Ptr m_settings_button;
  TextEntity::Ptr m_title_label;
  std::map<std::string, bool*> m_toggle_options;
  std::string m_title;
  void updateView() override;
  std::vector<CheckBoxOptionEntity::Ptr> m_options;

public:
  GameSettingsView(std::string title, std::map<std::string, bool*> toggle_options)
    : m_title(std::move(title))
    , m_toggle_options(std::move(toggle_options))
  {
  }
};

class MainMenuView : public View
{
public:
  using Ptr = std::shared_ptr<MainMenuView>;

private:
  TextEntity::Ptr m_title_label;
  TextButton::Ptr m_play_label;
  TextButton::Ptr m_quit_label;
  void updateView() override;
};

} // namespace ui
} // namespace kardeshev

#endif // !VIEW_H
