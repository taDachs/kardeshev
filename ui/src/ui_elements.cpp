#include "ui/ui_elements.h"
#include "ui/assets.h"
#include "ui/render.h"
#include "ui/ui_state.h"
#include "ui/window.h"
#include "util/logger.h"

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
  SDL_Rect text_dst = m_dst;
  SDL_Rect text_size = UI::render->getExpectedTextSize(m_font, m_text);
  if (m_boxed)
  {
    int border_width = std::max(m_border_percentage * m_dst.w, 1.0);
    int text_padding = std::max(m_padding_percentage * m_dst.w, 1.0);
    UI::render->setColor(m_box_border_color);
    UI::render->drawRect(text_dst, true);
    text_dst.x += border_width;
    text_dst.y += border_width;
    text_dst.h -= 2 * border_width;
    text_dst.w -= 2 * border_width;
    UI::render->setColor(m_box_color);
    UI::render->drawRect(text_dst, true);
    text_dst.x += text_padding;
    text_dst.y += text_padding;
    text_dst.h -= 2 * text_padding;
    text_dst.w -= 2 * text_padding;
  }
  if (m_centered) {
    text_dst.x = m_dst.x + (m_dst.w - text_size.w) / 2;
  }
  text_dst.y = m_dst.y + (m_dst.h - text_size.h) / 2;
  UI::render->drawText(m_text, m_font, text_dst, false, m_color);

  return m_dst;
}

SDL_Rect TextLabelUI::getExpectedSize() const {
  SDL_Rect size = UI::render->getExpectedTextSize(m_font, m_text);
  if (m_dst.w > size.w) {
    size.w = m_dst.w;
  }
  if (m_dst.h > size.h) {
    size.h = m_dst.h;
  }

  return size;
}

SDL_Rect TextBoxUI::draw()
{
  SDL_Rect text_dst = m_dst;
  const double border_thickness = 0.005;
  const double padding = 0.01;
  if (m_boxed)
  {
    int border_width = std::max(border_thickness * m_dst.w, 1.0);
    int text_padding = std::max(padding * m_dst.w, 1.0);

    text_dst = UI::render->getExpectedWrappedTextSize(m_font, m_text, m_dst.w - 2 * (border_width * text_padding));
    text_dst.w = m_dst.w;
    text_dst.h += 2 * (border_width + text_padding); // add border and padding to box
    text_dst.x = m_dst.x;
    text_dst.y = m_dst.y;
    m_dst.h = text_dst.h;

    UI::render->setColor(m_box_border_color);
    UI::render->drawRect(text_dst, true);
    text_dst.x += border_width;
    text_dst.y += border_width;
    text_dst.h -= 2 * border_width;
    text_dst.w -= 2 * border_width;
    UI::render->setColor(m_box_color);
    UI::render->drawRect(text_dst, true);
    text_dst.x += text_padding;
    text_dst.y += text_padding;
    text_dst.h -= 2 * text_padding;
    text_dst.w -= 2 * text_padding;
  }
  UI::render->drawText(m_text, m_font, text_dst, true, m_color);

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
