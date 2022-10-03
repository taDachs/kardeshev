#ifndef ENTITIES_H
#define ENTITIES_H

#include "lib/solar_systems.h"
#include "lib/stars.h"
#include "ui_elements.h"
#include <memory>
#include <utility>

namespace kardeshev {


class Entity : public std::enable_shared_from_this<Entity>
{
public:
  using Ptr = std::shared_ptr<Entity>;

protected:
  std::vector<std::shared_ptr<kardeshev::Component> > m_components;
  SDL_Point m_position;
  SDL_Point m_offset;
  double m_scale;

public:
  void draw()
  {
    for (auto& c : m_components)
    {
      c->drawIfAlive();
    }
  }
  virtual void update()                  = 0;
  virtual bool handleEvent(SDL_Event* e) = 0;
  SDL_Point getPositon() const { return m_position; }
  void setScale(double scale) { m_scale = scale; }
  void setOffset(SDL_Point offset) { m_offset = offset; }
};

class SystemEntity : public Entity
{
private:
  SolarSystem::Ptr m_system;
  TextLabelUI::Ptr m_system_name_label;
  TextLabelUI::Ptr m_number_planets_label;
  SystemUI::Ptr m_system_icon;
  bool m_selected = false;

public:
  SystemEntity(SolarSystem::Ptr system)
    : m_system(std::move(system))
  {
    m_system_icon = std::make_shared<SystemUI>(m_system);
    m_system_name_label =
      std::make_shared<TextLabelUI>(m_system->getInfo()->getNameOrId().substr(0, 10));
    m_system_name_label->setAlive(false);
    m_number_planets_label = std::make_shared<TextLabelUI>(
      "Num Planets: " + std::to_string(m_system->getPlanets().size()));
    m_number_planets_label->setAlive(false);
    m_components.push_back(m_system_icon);
    m_components.push_back(m_system_name_label);
    m_components.push_back(m_number_planets_label);
  }
  void update() override;
  SolarSystem::Ptr getSystem() const { return m_system; }
  bool handleEvent(SDL_Event* e) override;
};

class PlanetEntity : public Entity
{
private:
  Planet::Ptr m_planet;
  TextLabelUI::Ptr m_planet_name_label;
  PlanetUI::Ptr m_planet_icon;
  OrbitRingUI::Ptr m_orbit_ring;
  TextBoxUI::Ptr m_description_box;
  bool m_selected = false;

public:
  PlanetEntity(Planet::Ptr planet)
    : m_planet(std::move(planet))
  {
    m_planet_icon = std::make_shared<PlanetUI>(m_planet);
    m_planet_name_label =
      std::make_shared<TextLabelUI>(m_planet->getInfo()->getNameOrId().substr(0, 10));
    m_orbit_ring = std::make_shared<OrbitRingUI>();
    m_planet_name_label->setAlive(false);
    m_description_box = std::make_shared<TextBoxUI>(m_planet->getInfo()->planet_class.getDescription());
    m_components.push_back(m_orbit_ring);
    m_components.push_back(m_planet_icon);
    m_components.push_back(m_planet_name_label);
    m_components.push_back(m_description_box);
  }
  void update() override;
  bool handleEvent(SDL_Event* e) override;
  Planet::Ptr getPlanet() const { return m_planet; }
};

class StarEntity : public Entity
{
private:
  Star::Ptr m_star;
  TextLabelUI::Ptr m_star_name_label;
  StarUI::Ptr m_star_icon;
  bool m_selected = false;

public:
  StarEntity(Star::Ptr star)
    : m_star(std::move(star))
  {
    m_star_icon = std::make_shared<StarUI>(m_star);
    m_star_name_label =
      std::make_shared<TextLabelUI>(m_star->getInfo()->getNameOrId().substr(0, 10));
    m_star_name_label->setAlive(false);
    m_components.push_back(m_star_icon);
    m_components.push_back(m_star_name_label);
  }
  void update() override;
  bool handleEvent(SDL_Event* e) override;
  Star::Ptr getStar() const { return m_star; }
};

class BackButtonEntity : public Entity
{
private:
  ButtonUI::Ptr m_button;
  bool m_selected = false;
public:
  BackButtonEntity() {
    m_button = std::make_shared<ButtonUI>("galaxy_button");
    m_components.push_back(m_button);
  }
  void update() override;
  bool handleEvent(SDL_Event* e) override;
};

} // namespace kardeshev
#endif // !ENTITIES_H
