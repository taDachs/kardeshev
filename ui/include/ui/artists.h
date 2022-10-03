#ifndef ARTIST_H
#define ARTIST_H
#include "render.h"
#include "ui/entities.h"
#include "ui/window.h"
#include "ui_elements.h"

namespace kardeshev {

class SystemViewArtist : public Artist
{
private:
  std::shared_ptr<Game> m_game;
  std::shared_ptr<SolarSystem> m_current_system;
  std::vector<PlanetEntity> m_planets;
  glm::vec2 m_offset             = {0, 0};
  glm::vec2 m_mouse_pos_on_click = {0, 0};
  bool m_left_mouse_pressed      = false;
  void resetSystem()
  {
    m_planets.clear();
    for (auto& p : UI::state->focused_system->getPlanets())
    {
      m_planets.emplace_back(p);
    }
  }

public:
  SystemViewArtist(std::shared_ptr<Game> game)
    : m_game(std::move(game))
  {
  }
  void draw(Render& renderer) override;
  bool handleEvent(SDL_Event* e) override;
};

class GalaxyViewArtist : public Artist
{
private:
  std::shared_ptr<Galaxy> m_galaxy;
  std::shared_ptr<Game> m_game;
  std::vector<SystemEntity> m_systems;
  double m_zoom_level            = 1;
  glm::vec2 m_offset             = {0, 0};
  glm::vec2 m_mouse_pos_on_click = {0, 0};
  bool m_left_mouse_pressed      = false;

public:
  GalaxyViewArtist(std::shared_ptr<Game> game)
    : m_game(std::move(game))
  {
  }
  void draw(Render& renderer) override;
  void setGalaxy(std::shared_ptr<Galaxy> galaxy)
  {
    m_galaxy = std::move(galaxy);
    m_systems.clear();
    for (auto& s : m_galaxy->getSystems())
    {
      m_systems.emplace_back(s);
    }
  };
  bool handleEvent(SDL_Event* e) override;
};


class SystemInfoViewArtist : public Artist
{
private:
  std::shared_ptr<Game> m_game;

public:
  SystemInfoViewArtist(std::shared_ptr<Game> game)
    : m_game(std::move(game))
  {
  }
  void draw(Render& renderer) override;
  bool handleEvent(SDL_Event* e) override;
};

class PlanetInfoViewArtist : public Artist
{
private:
  std::shared_ptr<Game> m_game;

public:
  PlanetInfoViewArtist(std::shared_ptr<Game> game)
    : m_game(std::move(game))
  {
  }
  void draw(Render& renderer) override;
  bool handleEvent(SDL_Event* e) override;
};


} // namespace kardeshev


#endif // !ARTIST_H
