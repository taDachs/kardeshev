#ifndef ASSETS_H
#define ASSETS_H

#include "generation/name_generation.h"
#include <SDL_ttf.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>
namespace kardeshev {

struct Font
{
  const static std::string DEFAULT_FONT;

  enum Size
  {
    SMALL,
    MEDIUM,
    LARGE
  };


  TTF_Font* small;
  TTF_Font* medium;
  TTF_Font* large;

  Font(TTF_Font* small_, TTF_Font* medium_, TTF_Font* large_)
    : small(small_)
    , medium(medium_)
    , large(large_)
  {
  }
  void closeFont()
  {
    TTF_CloseFont(small);
    small = nullptr;
    TTF_CloseFont(medium);
    medium = nullptr;
    TTF_CloseFont(large);
    large = nullptr;
  }
};

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

  SDL_Texture* getTexture() const { return m_texture; }

  SDL_Rect getFrame(const int i) const { return m_frames.at(i); }

  void closeTexture() { SDL_DestroyTexture(m_texture); }
};


class AssetHandler
{
public:
  using Ptr = std::shared_ptr<AssetHandler>;

private:
  std::map<std::string, Font> m_fonts;
  std::map<std::string, Texture> m_textures;
  std::map<std::string, generation::TokenList::Ptr> m_token_lists;

public:
  ~AssetHandler();
  void addFont(const std::string& name,
               const std::string& path,
               const int size_small,
               const int size_medium,
               const int size_large);
  void addTexture(const std::string& name, const std::string& path);
  void addTexture(const std::string& name, const std::string& path, int w, int h, int frames);
  void addTokenList(const std::string& name, const std::string& path);
  Font getFont(const std::string& name) const { return m_fonts.at(name); }
  bool isFontLoaded(const std::string& name) const { return m_fonts.find(name) != m_fonts.end(); }
  Texture getTexture(const std::string& name) const { return m_textures.at(name); }
  generation::TokenList::Ptr getTokenList(const std::string& name) const
  {
    return m_token_lists.at(name);
  }
};

} // namespace kardeshev

#endif // !ASSETS_H
