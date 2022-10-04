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
  void setDepth(int depth) { m_depth = depth; }
  int getDepth() const { return m_depth; }
  bool isVisible(const SDL_Rect& viewport)
  {
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
  Font::Size m_size = Font::Size::MEDIUM;

public:
  TextLabelUI() = default;
  TextLabelUI(std::string text)
    : m_text(std::move(text))
  {
    m_depth = 1;
  }

  void setText(const std::string& text) { m_text = text; }

  void setColor(const Color& color) { m_color = color; }
  std::string getText() const { return m_text; }
  SDL_Rect draw() override;
  void setFontSize(Font::Size size) { m_size = size; }
};

class TextBoxUI : public Component
{
public:
  using Ptr = std::shared_ptr<TextBoxUI>;

private:
  std::string m_text;
  Color m_color = WHITE;
  Font::Size m_size = Font::Size::SMALL;

public:
  TextBoxUI() = default;
  TextBoxUI(std::string text)
    : m_text(std::move(text))
  {
    m_depth = 1;
  }

  void setText(const std::string& text) { m_text = text; }

  void setColor(const Color& color) { m_color = color; }
  std::string getText() const { return m_text; }
  SDL_Rect draw() override;
  void setFontSize(Font::Size size) { m_size = size; }
};


class TextureComponent : public Component
{
public:
  using Ptr = std::shared_ptr<TextureComponent>;

private:
  Texture m_tex;
  int m_frame = -1;

public:
  TextureComponent(Texture tex, int frame = -1)
    : m_tex(std::move(tex))
    , m_frame(frame)
  {
  }
  TextureComponent(const std::string& name, int frame = -1)
    : m_tex(UI::assets->getTexture(name))
    , m_frame(frame)
  {
  }
  SDL_Rect draw() override;
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

} // namespace kardeshev

#endif // !UI_ELEMENTS_H
