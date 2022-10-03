#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H
#include <memory>
#include <utility>

#include "lib/stars.h"
#include "render.h"
#include "ui/assets.h"
#include "ui/ui_state.h"

namespace kardeshev {

class Component : public std::enable_shared_from_this<Component>
{
public:
  using Ptr = std::shared_ptr<Component>;
protected:
  bool m_alive = true;
  SDL_Rect m_dst;
  // 0 is foreground
  // 1000 is background
  int m_depth = 500;

public:
  virtual SDL_Rect draw() = 0;
  void drawIfAlive()
  {
    if (m_alive)
    {
      draw();
    }
  }
  void setAlive(bool alive) { m_alive = alive; }
  bool isAlive() const { return m_alive; }
  void setDst(SDL_Rect dst) { m_dst = dst; }
  bool isUnderMouse(int x, int y) const
  {
    SDL_Point p;
    p.x = x;
    p.y = y;
    return SDL_PointInRect(&p, &m_dst);
  }
  int getDepth() const {
    return m_depth;
  }
  bool isVisible(const SDL_Rect& viewport) {
    SDL_Rect res;
    return SDL_IntersectRect(&viewport, &m_dst, &res);
  }
};

class TextLabelUI : public Component
{
public:
  using Ptr = std::shared_ptr<TextLabelUI>;

private:
  std::string m_text;
  Color m_color = WHITE;

public:
  TextLabelUI() = default;
  TextLabelUI(std::string text)
    : m_text(std::move(text))
  {
    m_depth = 0;
  }

  void setText(const std::string& text) { m_text = text; }

  void setColor(const Color& color) { m_color = color; }
  std::string getText() const { return m_text; }
  SDL_Rect draw() override;
};

class TextBoxUI : public Component
{
public:
  using Ptr = std::shared_ptr<TextBoxUI>;

private:
  std::string m_text;
  Color m_color = WHITE;

public:
  TextBoxUI() = default;
  TextBoxUI(std::string text)
    : m_text(std::move(text))
  {
    m_depth = 0;
  }

  void setText(const std::string& text) { m_text = text; }

  void setColor(const Color& color) { m_color = color; }
  std::string getText() const { return m_text; }
  SDL_Rect draw() override;
};


class PlanetUI : public Component
{
public:
  using Ptr = std::shared_ptr<PlanetUI>;

private:
  std::shared_ptr<Planet> m_planet;
  bool m_selected = false;
  Texture m_tex;


public:
  PlanetUI(std::shared_ptr<Planet> planet)
    : m_planet(std::move(planet))
    , m_tex(UI::assets->getTexture("planet_simple"))
  {
  }
  std::shared_ptr<Planet> getPlanet() const { return m_planet; }
  SDL_Rect draw() override;
  void setSelected(bool selected) { m_selected = selected; }
};

class OrbitRingUI : public Component
{
public:
  using Ptr = std::shared_ptr<OrbitRingUI>;

private:
  bool m_selected = false;
  Texture m_tex;

public:
  OrbitRingUI()
    : m_tex(UI::assets->getTexture("orbit_ring"))
  {
    m_depth = 1000;
  }
  SDL_Rect draw() override;
  void setSelected(bool selected) { m_selected = selected; }
};

class SystemUI : public Component
{
public:
  using Ptr = std::shared_ptr<SystemUI>;

private:
  std::shared_ptr<SolarSystem> m_system;
  bool m_selected = false;
  Texture m_tex;

public:
  SystemUI(std::shared_ptr<SolarSystem> system)
    : m_system(std::move(system))
    , m_tex(UI::assets->getTexture("system_simple"))
  {
  }
  std::shared_ptr<SolarSystem> getSystem() const { return m_system; }
  SDL_Rect draw() override;
  void setSelected(const bool selected) { m_selected = selected; }
};

class StarUI : public Component
{
public:
  using Ptr = std::shared_ptr<StarUI>;

private:
  Star::Ptr m_star;
  bool m_selected = false;
  Texture m_tex;

public:
  StarUI(Star::Ptr star)
    : m_star(std::move(star))
    , m_tex(UI::assets->getTexture("star_simple"))
  {
  }
  Star::Ptr getStar() const { return m_star; }
  SDL_Rect draw() override;
  void setSelected(const bool selected) { m_selected = selected; }
};

class ButtonUI : public Component
{
public:
  using Ptr = std::shared_ptr<ButtonUI>;
private:
  Texture m_tex;
  bool m_selected = false;
public:
  ButtonUI(const std::string& texture): m_tex(UI::assets->getTexture(texture)) {
    m_depth = 250;
  }
  SDL_Rect draw() override;
  void setSelected(const bool selected) { m_selected = selected; }
};

} // namespace kardeshev

#endif // !UI_ELEMENTS_H
