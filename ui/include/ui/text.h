#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include "ui/exceptions.h"
#include "ui/texture.h"
#include <SDL_ttf.h>
#include <array>
#include <memory>
#include <string>

namespace kardeshev {
namespace ui {


class Font
{
public:
  const static std::string DEFAULT_FONT_SMALL;
  const static std::string DEFAULT_FONT_NOT_SO_SMALL;
  const static std::string DEFAULT_FONT_MEDIUM;
  const static std::string DEFAULT_FONT_LARGE;

  using Ptr = std::shared_ptr<Font>;

private:
  int m_atlas_size = 1024;
  TTF_Font* m_font;
  SDL_Texture* m_atlas;
  std::vector<SDL_Rect> m_glyphs;
  int m_w;
  int m_h;

public:
  Font(TTF_Font* font, SDL_Renderer* render)
    : m_font(font)
  {
    SDL_Color white{0xff, 0xff, 0xff, 0xff};
    SDL_Surface* surface = SDL_CreateRGBSurface(0, m_atlas_size, m_atlas_size, 32, 0, 0, 0, 0xff);
    if (surface == nullptr) {
      throw SDLException("Failed to create surface for glyph atlas");
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

    SDL_Rect dst;
    dst.x = dst.y = 0;

    std::array<char, 2> c;
    for (int i = ' '; i <= '}'; i++)
    {
      c[0] = i;
      c[1] = 0;

      SDL_Surface* text = TTF_RenderUTF8_Blended(font, c.data(), white);
      TTF_SizeText(font, c.data(), &dst.w, &dst.h);
      if (dst.x + dst.w >= m_atlas_size) {
        dst.x = 0;
        dst.y += dst.h + 1;
        if (dst.y + dst.h >= m_atlas_size) {
          throw AssetException("Ran out of space in glyph atlas");
        }
      }

      SDL_BlitSurface(text, nullptr, surface, &dst);
      m_glyphs.push_back(dst);
      SDL_FreeSurface(text);
      dst.x += dst.w;
    }
    m_atlas = SDL_CreateTextureFromSurface(render, surface);
    if (m_atlas == nullptr) {
      throw SDLException("Failed to create glyph atlas from surface");
    }
    SDL_FreeSurface(surface);
  }

  TTF_Font* getFont() const
  {
    return m_font;
  }

  SDL_Texture* getAtlas() const { return m_atlas; }

  SDL_Rect getGlyphSrc(char c) const {
    return m_glyphs.at(c - ' ');
  }

  void closeFont()
  {
    TTF_CloseFont(m_font);
    SDL_DestroyTexture(m_atlas);
    m_font = nullptr;
    m_atlas = nullptr;
  }
};

} // namespace ui
} // namespace kardeshev


#endif // !TEXT_H
