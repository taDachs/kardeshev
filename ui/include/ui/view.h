#ifndef VIEW_H
#define VIEW_H

#include <SDL.h>

#include "entities.h"
#include "ui_state.h"
#include <utility>

namespace kardeshev {

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

public:
  virtual void drawView() = 0;
  void draw()
  {
    if (m_viewport != nullptr)
    {
      SDL_RenderSetViewport(UI::render, m_viewport.get());
      UI::current_viewport = m_viewport;
    }
    drawView();
    if (m_viewport != nullptr)
    {
      SDL_RenderSetViewport(UI::render, nullptr);
      UI::current_viewport = nullptr;
    }
  }
  void update()
  {
    if (m_viewport != nullptr)
    {
      SDL_RenderSetViewport(UI::render, m_viewport.get());
      UI::current_viewport = m_viewport;
    }
    updateView();
    if (m_viewport != nullptr)
    {
      SDL_RenderSetViewport(UI::render, nullptr);
      UI::current_viewport = nullptr;
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
    SDL_RenderSetViewport(UI::render, m_viewport.get());
    UI::current_viewport = m_viewport;
    for (auto& entity : m_entities)
    {
      if (entity->handleEvent(e))
      {
        SDL_RenderSetViewport(UI::render, nullptr);
        UI::current_viewport = nullptr;
        return true;
      }
    }
    SDL_RenderSetViewport(UI::render, nullptr);
    UI::current_viewport = nullptr;
    return false;
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
  SDL_Point getCenterOffset()
  {
    SDL_Point offset = m_offset;
    if (UI::current_viewport == nullptr)
    {
      offset.x += UI::window_size.w / 2;
      offset.y += UI::window_size.h / 2;
    }
    else
    {
      offset.x += UI::current_viewport->w / 2;
      offset.y += UI::current_viewport->h / 2;
    }
    return offset;
  }

public:
  bool handleEvent(SDL_Event* e) override;
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
  SolarSystem::Ptr m_current_system = nullptr;

  void drawView() override;
  void updateView() override;
};


} // namespace kardeshev

#endif // !VIEW_H
