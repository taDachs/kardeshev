#ifndef TEXTURE_H
#define TEXTURE_H

#include "ui/exceptions.h"
#include <SDL.h>
#include <memory>
#include <utility>
#include <vector>

namespace kardeshev {
namespace ui {

class Texture
{
private:
  SDL_Texture* m_texture;
  std::vector<SDL_Rect> m_frames;
  int m_w;
  int m_h;

public:
  Texture(SDL_Texture* texture)
    : m_texture(texture)
  {
    SDL_QueryTexture(texture, nullptr, nullptr, &m_w, &m_h);
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = m_w;
    r.h = m_h;
    m_frames.push_back(r);
  }

  Texture(SDL_Texture* texture, int w, int h, int frames)
    : m_texture(texture)
  {
    m_w = w;
    m_h = h;
    for (int i = 0; i < frames; i++)
    {
      SDL_Rect r;
      r.x = 0;
      r.y = i * h;
      r.h = h;
      r.w = w;
      m_frames.push_back(r);
    }
  }

  Texture(SDL_Texture* texture, std::vector<SDL_Rect> frames): m_texture(texture), m_frames(std::move(frames)) {}

  SDL_Texture* getTexture() const { return m_texture; }

  SDL_Rect getFrame(const int i) const { return m_frames.at(i); }

  void setTextureBlendMode(const SDL_BlendMode blend_mode)
  {
    if (SDL_SetTextureBlendMode(m_texture, blend_mode))
    {
      throw SDLException();
    }
  }

  void closeTexture() { SDL_DestroyTexture(m_texture); }
};

} // namespace ui
} // namespace kardeshev


#endif // !TEXTURE_H
