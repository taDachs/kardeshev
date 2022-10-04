#ifndef ENTITIES_H
#define ENTITIES_H

#include "lib/solar_systems.h"
#include "lib/stars.h"
#include "ui/assets.h"
#include "ui_elements.h"
#include <memory>
#include <utility>

namespace kardeshev {


class Entity : public std::enable_shared_from_this<Entity>
{
public:
  using Ptr = std::shared_ptr<Entity>;

protected:
  std::vector<std::shared_ptr<Component> > m_components;
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
  std::vector<Component::Ptr> getDrawList()
  {
    std::vector<Component::Ptr> alive;
    for (const auto& c : m_components)
    {
      SDL_Rect viewport = UI::getRenderSize();
      viewport.x = viewport.y = 0;
      if (c->isAlive() && c->isVisible(viewport))
      {
        alive.push_back(c);
      }
    }
    return alive;
  }
  virtual void update()                  = 0;
  virtual bool handleEvent(SDL_Event* e) = 0;
  SDL_Point getPositon() const { return m_position; }
  void setScale(double scale) { m_scale = scale; }
  void setOffset(SDL_Point offset) { m_offset = offset; }
};

class SystemEntity : public Entity
{
public:
  const static std::string SYSTEM_SPRITE;
  const static int SYSTEM_SPRITE_SELECTED_FRAME;
  const static int SYSTEM_SPRITE_NOT_SELECTED_FRAME;

private:
  SolarSystem::Ptr m_system;
  TextLabelUI::Ptr m_system_name_label;
  TextLabelUI::Ptr m_number_planets_label;
  TextureComponent::Ptr m_system_not_selected_icon;
  TextureComponent::Ptr m_system_selected_icon;
  bool m_selected = false;

public:
  SystemEntity(SolarSystem::Ptr system)
    : m_system(std::move(system))
  {
    m_system_selected_icon =
      std::make_shared<TextureComponent>(SYSTEM_SPRITE, SYSTEM_SPRITE_SELECTED_FRAME);
    m_system_not_selected_icon =
      std::make_shared<TextureComponent>(SYSTEM_SPRITE, SYSTEM_SPRITE_NOT_SELECTED_FRAME);
    m_system_selected_icon->setAlive(false);

    m_system_name_label =
      std::make_shared<TextLabelUI>(m_system->getInfo()->getNameOrId().substr(0, 10));
    m_system_name_label->setAlive(false);
    m_number_planets_label = std::make_shared<TextLabelUI>(
      "Num Planets: " + std::to_string(m_system->getPlanets().size()));
    m_number_planets_label->setAlive(false);

    m_components.push_back(m_system_selected_icon);
    m_components.push_back(m_system_not_selected_icon);

    m_components.push_back(m_system_name_label);
    m_components.push_back(m_number_planets_label);
  }
  void update() override;
  SolarSystem::Ptr getSystem() const { return m_system; }
  bool handleEvent(SDL_Event* e) override;
};

class PlanetEntity : public Entity
{
public:
  const static std::string PLANET_SPRITE;
  const static int PLANET_SPRITE_SELECTED_FRAME;
  const static int PLANET_SPRITE_FOCUSED_FRAME;
  const static int PLANET_SPRITE_NOT_SELECTED_FRAME;

private:
  Planet::Ptr m_planet;
  TextLabelUI::Ptr m_planet_name_label;
  TextureComponent::Ptr m_selected_icon;
  TextureComponent::Ptr m_focused_icon;
  TextureComponent::Ptr m_not_selected_icon;
  OrbitRingUI::Ptr m_orbit_ring;
  TextBoxUI::Ptr m_description_box;
  bool m_selected = false;

public:
  PlanetEntity(Planet::Ptr planet)
    : m_planet(std::move(planet))
  {
    m_selected_icon =
      std::make_shared<TextureComponent>(PLANET_SPRITE, PLANET_SPRITE_SELECTED_FRAME);
    m_not_selected_icon =
      std::make_shared<TextureComponent>(PLANET_SPRITE, PLANET_SPRITE_NOT_SELECTED_FRAME);
    m_focused_icon = std::make_shared<TextureComponent>(PLANET_SPRITE, PLANET_SPRITE_FOCUSED_FRAME);

    m_planet_name_label =
      std::make_shared<TextLabelUI>(m_planet->getInfo()->getNameOrId().substr(0, 10));
    m_orbit_ring = std::make_shared<OrbitRingUI>();
    m_planet_name_label->setAlive(false);
    m_description_box =
      std::make_shared<TextBoxUI>(m_planet->getInfo()->planet_class.getDescription());
    m_components.push_back(m_orbit_ring);

    m_components.push_back(m_selected_icon);
    m_components.push_back(m_not_selected_icon);
    m_components.push_back(m_focused_icon);

    m_components.push_back(m_planet_name_label);
    m_components.push_back(m_description_box);
  }
  void update() override;
  bool handleEvent(SDL_Event* e) override;
  Planet::Ptr getPlanet() const { return m_planet; }
};

class StarEntity : public Entity
{
public:
  const static std::string STAR_SPRITE;
  const static int STAR_SPRITE_SELECTED_FRAME;
  const static int STAR_SPRITE_NOT_SELECTED_FRAME;

private:
  Star::Ptr m_star;
  TextLabelUI::Ptr m_star_name_label;
  TextureComponent::Ptr m_selected_icon;
  TextureComponent::Ptr m_not_selected_icon;
  bool m_selected = false;

public:
  StarEntity(Star::Ptr star)
    : m_star(std::move(star))
  {
    m_selected_icon = std::make_shared<TextureComponent>(STAR_SPRITE, STAR_SPRITE_SELECTED_FRAME);
    m_selected_icon->setAlive(false);
    m_not_selected_icon =
      std::make_shared<TextureComponent>(STAR_SPRITE, STAR_SPRITE_NOT_SELECTED_FRAME);
    m_star_name_label =
      std::make_shared<TextLabelUI>(m_star->getInfo()->getNameOrId().substr(0, 10));
    m_star_name_label->setAlive(false);
    m_components.push_back(m_selected_icon);
    m_components.push_back(m_not_selected_icon);
    m_components.push_back(m_star_name_label);
  }
  void update() override;
  bool handleEvent(SDL_Event* e) override;
  Star::Ptr getStar() const { return m_star; }
};

class BackButtonEntity : public Entity
{
private:
  TextureComponent::Ptr m_button_not_selected;
  TextureComponent::Ptr m_button_selected;
  bool m_selected = false;

public:
  BackButtonEntity()
  {
    m_button_selected = std::make_shared<TextureComponent>("galaxy_button", 1);
    m_button_selected->setAlive(false);
    m_button_not_selected = std::make_shared<TextureComponent>("galaxy_button", 0);
    m_components.push_back(m_button_not_selected);
    m_components.push_back(m_button_selected);
  }
  void update() override;
  bool handleEvent(SDL_Event* e) override;
};

class GalaxyInfoEntity : public Entity
{
private:
  TextBoxUI::Ptr m_text_box;

public:
  GalaxyInfoEntity()
  {
    m_text_box = std::make_shared<TextBoxUI>();
    m_components.push_back(m_text_box);
  }
  void update() override
  {
    SDL_Rect dst = UI::getRenderSize();
    dst.x        = 0;
    dst.y        = dst.h * 0.01;
    dst.w = std::min(dst.w, 200);
    m_text_box->setText("Time: " + std::to_string(UI::game->getTime().getTicks()));
    m_text_box->setDst(dst);
  }
  bool handleEvent(SDL_Event* e) override { return false; }
};

class PlanetInfoEntity : public Entity
{
private:
  TextLabelUI::Ptr m_planet_name_label;
  TextLabelUI::Ptr m_nothing_selected_label;
  TextBoxUI::Ptr m_info_box;
  TextureComponent::Ptr m_planet_portrait;
  std::string m_text;
  Planet::Ptr m_current_planet;
  void generateText();

public:
  PlanetInfoEntity()
  {
    m_nothing_selected_label = std::make_shared<TextLabelUI>("No Planet Selected");

    m_planet_name_label = std::make_shared<TextLabelUI>();
    m_info_box          = std::make_shared<TextBoxUI>();
    m_planet_portrait   = std::make_shared<TextureComponent>("planet_portrait");
    m_planet_name_label->setAlive(false);
    m_info_box->setAlive(false);
    m_planet_portrait->setAlive(false);


    m_components.push_back(m_planet_name_label);
    m_components.push_back(m_nothing_selected_label);
    m_components.push_back(m_info_box);
    m_components.push_back(m_planet_portrait);
  }
  void update() override;
  bool handleEvent(SDL_Event* e) override { return false; }
};

class LoadingTextEntity : public Entity
{
public:
  using Ptr = std::shared_ptr<LoadingTextEntity>;
private:
  TextLabelUI::Ptr m_loading_label;
  TextLabelUI::Ptr m_title_label;
public:
  LoadingTextEntity()
  {
    m_loading_label = std::make_shared<TextLabelUI>("Loading");
    m_title_label = std::make_shared<TextLabelUI>("Kardeshev");
    m_title_label->setFontSize(Font::Size::LARGE);
    m_components.push_back(m_loading_label);
    m_components.push_back(m_title_label);
  }
  void update() override;
  void setText(const std::string& s) {
    m_loading_label->setText(s);
  }
  bool handleEvent(SDL_Event* e) override { return false; }
};

} // namespace kardeshev
#endif // !ENTITIES_H
