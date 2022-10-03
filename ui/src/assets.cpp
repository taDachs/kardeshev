#include "ui/assets.h"
#include "ui/window.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdexcept>

using namespace kardeshev;
const std::string Font::DEFAULT_FONT = "default";

void AssetHandler::addFont(const std::string& name,
                           const std::string& path,
                           const int small_size,
                           const int medium_size,
                           const int large_size)
{
  TTF_Font* small  = TTF_OpenFont(path.c_str(), small_size);
  TTF_Font* medium = TTF_OpenFont(path.c_str(), medium_size);
  TTF_Font* large  = TTF_OpenFont(path.c_str(), large_size);
  Font f(small, medium, large);
  m_fonts.insert({name, f});
}

void AssetHandler::addTexture(const std::string& name, const std::string& path)
{
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  if (loaded_surface == nullptr)
  {
    throw std::invalid_argument(path);
  }
  SDL_Texture* loaded_tex = SDL_CreateTextureFromSurface(UI::render, loaded_surface);
  SDL_FreeSurface(loaded_surface);
  Texture tex(loaded_tex);
  m_textures.insert({name, tex});
}

void AssetHandler::addTexture(
  const std::string& name, const std::string& path, int w, int h, int frames)
{
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  if (loaded_surface == nullptr)
  {
    throw std::invalid_argument(path);
  }
  SDL_Texture* loaded_tex = SDL_CreateTextureFromSurface(UI::render, loaded_surface);
  SDL_FreeSurface(loaded_surface);
  Texture tex(loaded_tex, w, h, frames);
  m_textures.insert({name, tex});
}
