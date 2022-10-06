#include "ui/ui_elements.h"
#include "ui/assets.h"
#include "ui/render.h"
#include "ui/ui_state.h"
#include "ui/window.h"

using namespace kardeshev;
using namespace ui;

SDL_Rect TextureComponent::draw()
{
  if (m_frame >= 0)
  {
    SDL_Rect src = m_tex.getFrame(m_frame);
    UI::render->copyTexture(m_tex, &src, &m_dst);
  }
  else
  {
    UI::render->copyTexture(m_tex, nullptr, &m_dst);
  }
  return m_dst;
}

SDL_Rect TextLabelUI::draw()
{
  Font font = UI::assets->getFont(Font::DEFAULT_FONT);
  UI::render->drawText(m_text, font, m_size, m_dst, m_centered, false, m_color);
  return m_dst;
}

SDL_Rect TextBoxUI::draw()
{
  Font font = UI::assets->getFont(Font::DEFAULT_FONT);
  UI::render->drawText(m_text, font, m_size, m_dst, false, true, m_color);
  return m_dst;
}


SDL_Rect OrbitRingUI::draw()
{
  // SDL_RenderCopy(UI::render, m_tex.getTexture(), nullptr, &m_dst);
  int center_x = m_dst.x + m_dst.w / 2;
  int center_y = m_dst.y + m_dst.h / 2;
  int radius   = m_dst.w / 2;
  UI::render->setColor(GRAY);
  UI::render->drawCircle(center_x, center_y, radius - 1);
  UI::render->drawCircle(center_x, center_y, radius);
  UI::render->drawCircle(center_x, center_y, radius + 1);
  return m_dst;
}
