#ifndef ASSETS_H
#define ASSETS_H

#include "generation/name_generation.h"
#include "ui/render.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>
namespace kardeshev {
namespace ui {

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

} // namespace ui
} // namespace kardeshev

#endif // !ASSETS_H
