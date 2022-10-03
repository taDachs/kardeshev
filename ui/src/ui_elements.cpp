#include "ui/ui_elements.h"
#include "SDL_render.h"
#include "ui/assets.h"
#include "ui/render.h"
#include "ui/window.h"

using namespace kardeshev;

SDL_Rect PlanetUI::draw()
{
  if (m_selected)
  {
    SDL_Rect src = m_tex.getFrame(2);
    SDL_RenderCopy(UI::render, m_tex.getTexture(), &src, &m_dst);
  }
  else if (m_planet == UI::state->focused_planet)
  {
    SDL_Rect src = m_tex.getFrame(1);
    SDL_RenderCopy(UI::render, m_tex.getTexture(), &src, &m_dst);
  }
  else
  {
    SDL_Rect src = m_tex.getFrame(0);
    SDL_RenderCopy(UI::render, m_tex.getTexture(), &src, &m_dst);
  }
  return m_dst;
}


SDL_Rect SystemUI::draw()
{
  if (m_selected)
  {
    SDL_Rect src = m_tex.getFrame(2);
    SDL_RenderCopy(UI::render, m_tex.getTexture(), &src, &m_dst);
  }
  else
  {
    SDL_Rect src = m_tex.getFrame(0);
    SDL_RenderCopy(UI::render, m_tex.getTexture(), &src, &m_dst);
  }
  return m_dst;
}

SDL_Rect StarUI::draw()
{
  if (m_selected)
  {
    SDL_Rect src = m_tex.getFrame(2);
    SDL_RenderCopy(UI::render, m_tex.getTexture(), &src, &m_dst);
  }
  else
  {
    SDL_Rect src = m_tex.getFrame(0);
    SDL_RenderCopy(UI::render, m_tex.getTexture(), &src, &m_dst);
  }
  return m_dst;
}

SDL_Rect TextLabelUI::draw()
{
  Font font = UI::assets->getFont(Font::DEFAULT_FONT);
  SDL_Surface* surface_message =
    TTF_RenderText_Blended(font.medium, m_text.c_str(), {m_color.r, m_color.g, m_color.b});
  m_dst.w = std::min(surface_message->w, m_dst.w);
  m_dst.h = std::min(surface_message->h, m_dst.h);
  SDL_Texture* message = SDL_CreateTextureFromSurface(UI::render, surface_message);
  SDL_RenderCopy(UI::render, message, nullptr, &m_dst);
  SDL_FreeSurface(surface_message);

  SDL_DestroyTexture(message);
  return m_dst;
}

SDL_Rect TextBoxUI::draw()
{
  Font font = UI::assets->getFont(Font::DEFAULT_FONT);
  SDL_Surface* surface_message =
    TTF_RenderText_Blended_Wrapped(font.small, m_text.c_str(), {m_color.r, m_color.g, m_color.b}, m_dst.w);
  m_dst.h = std::min(surface_message->h, m_dst.h);
  SDL_Texture* message = SDL_CreateTextureFromSurface(UI::render, surface_message);
  SDL_RenderCopy(UI::render, message, nullptr, &m_dst);
  SDL_FreeSurface(surface_message);

  SDL_DestroyTexture(message);
  return m_dst;
}


SDL_Rect OrbitRingUI::draw()
{
  // SDL_RenderCopy(UI::render, m_tex.getTexture(), nullptr, &m_dst);
  int center_x = m_dst.x + m_dst.w / 2;
  int center_y = m_dst.y + m_dst.h / 2;
  int radius   = m_dst.w / 2;
  drawCircle(center_x, center_y, radius, GRAY);
  return m_dst;
}

SDL_Rect ButtonUI::draw() {
  if (m_selected) {
    SDL_Rect frame = m_tex.getFrame(1);
    SDL_RenderCopy(UI::render, m_tex.getTexture(), &frame, &m_dst);
  } else {
    SDL_Rect frame = m_tex.getFrame(0);
    SDL_RenderCopy(UI::render, m_tex.getTexture(), &frame, &m_dst);
  }
  return m_dst;
}
