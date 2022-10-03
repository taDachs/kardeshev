#include "ui/ui_elements.h"
#include "SDL_render.h"
#include "ui/assets.h"
#include "ui/render.h"
#include "ui/ui_state.h"
#include "ui/window.h"

using namespace kardeshev;

SDL_Rect TextureComponent::draw() {
  if (m_frame >= 0) {
    SDL_Rect src = m_tex.getFrame(m_frame);
    SDL_RenderCopy(UI::render, m_tex.getTexture(), &src, &m_dst);
  } else {
    SDL_RenderCopy(UI::render, m_tex.getTexture(), nullptr, &m_dst);
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
  if (surface_message == nullptr) {
    throw TTFException("failed drawing text");
  }
  m_dst.h = std::min(surface_message->h, m_dst.h);
  SDL_Texture* message = SDL_CreateTextureFromSurface(UI::render, surface_message);
  if (message == nullptr) {
    throw SDLException("failed creating texture from text message");
  }
  if (SDL_RenderCopy(UI::render, message, nullptr, &m_dst)) {
    throw SDLException("Failed copying text message to screen");
  }

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
  drawCircle(center_x, center_y, radius - 1, GRAY);
  drawCircle(center_x, center_y, radius, GRAY);
  drawCircle(center_x, center_y, radius + 1, GRAY);
  return m_dst;
}
