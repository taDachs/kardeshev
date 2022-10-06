#include "ui/assets.h"
#include "generation/name_generation.h"
#include "ui/ui_state.h"
#include "ui/window.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <stdexcept>

using namespace kardeshev;
using namespace ui;
const std::string Font::DEFAULT_FONT = "default";

AssetHandler::~AssetHandler()
{
  for (auto& font : m_fonts)
  {
    font.second.closeFont();
  }
  for (auto& font : m_textures)
  {
    font.second.closeTexture();
  }
}

void AssetHandler::addFont(const std::string& name,
                           const std::string& path,
                           const int small_size,
                           const int medium_size,
                           const int large_size)
{
  util::Logger::logInfo("Loading Font " + name);
  Font f = UI::render->loadFont(path, small_size, medium_size, large_size);
  m_fonts.insert({name, f});
}

void AssetHandler::addTexture(const std::string& name, const std::string& path)
{
  util::Logger::logInfo("Loading Texture " + name);

  Texture tex = UI::render->loadTexture(path);
  m_textures.insert({name, tex});
}

void AssetHandler::addTexture(
  const std::string& name, const std::string& path, int w, int h, int frames)
{
  util::Logger::logInfo("Loading Texture " + name);
  Texture temp = UI::render->loadTexture(path);
  Texture tex(temp.getTexture(), w, h, frames);
  m_textures.insert({name, tex});
}

void AssetHandler::addTokenList(const std::string& name, const std::string& path)
{
  util::Logger::logInfo("Loading Tokenlist " + name);
  std::ifstream word_file(path);
  std::string line;
  auto token_list = std::make_shared<generation::TokenList>();
  while (getline(word_file, line))
  {
    line = regex_replace(line, std::regex("^[ |\t|\n|\r|\v|\f]*|[ |\t|\n|\r|\v|\f]*$"), "");
    for (const auto& t : util::splitString(line, ";")) {
      token_list->addToken(t);
    }
  }
  m_token_lists.insert({name, token_list});
}
