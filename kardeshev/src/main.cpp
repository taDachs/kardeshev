#include "main.h"
#include "generation/galaxy_generation.h"
#include "generation/planet_generation.h"
#include "generation/solar_system_generation.h"
#include "generation/star_generation.h"
#include "lib/buildings.h"
#include "lib/planets.h"
#include "lib/population.h"
#include "lib/solar_systems.h"
#include "lib/stars.h"
#include "ui/assets.h"
#include "ui/render.h"
#include "ui/window.h"
#include <SDL.h>
#include <iostream>
#include <map>
#include <memory>

void setupGenerators()
{
  pg  = std::make_shared<kardeshev::NaivePlanetGenerator>(PLANETS);
  sg  = std::make_shared<kardeshev::NaiveStarGenerator>(STARS);
  ssg = std::make_shared<kardeshev::NaiveSolarSystemGenerator>(pg, sg, 3, 10);
  gg  = std::make_shared<kardeshev::NaiveGalaxyGenerator>(ssg, 1000, 1001);
}

int main()
{
  game = std::make_shared<kardeshev::Game>();

  std::cout << "Setting up generators" << std::endl;
  setupGenerators();

  std::cout << "generating galaxy" << std::endl;
  std::shared_ptr<kardeshev::Galaxy> g = gg->generateGalaxy();
  game->setGalaxy(g);

  kardeshev::ResourceType potatoes("potato_resource", "Potatoes");
  kardeshev::Need food_req(potatoes, 1);
  std::shared_ptr<kardeshev::Building> potato_farm = std::make_shared<kardeshev::Farm>(potatoes);
  kardeshev::Species humans("human_species", "Humans", 1.01, {food_req});
  kardeshev::Population pop(humans, {});
  std::shared_ptr<kardeshev::SolarSystem> ss = g->getSystems()[0];
  ss->getPlanets()[0]->addPop(pop);
  ss->getPlanets()[0]->addBuilding(potato_farm);

  std::cout << "Creating system artist" << std::endl;

  std::cout << "Init SDL" << std::endl;
  kardeshev::initSDL();

  std::cout << "Creating window" << std::endl;
  auto main_window = std::make_shared<kardeshev::GameWindow>();

  main_window->init();
  kardeshev::UI::game = game;

  // kardeshev::UI::assets->addFont(
  //   kardeshev::Font::DEFAULT_FONT, "assets/aerial.ttf", 12, 26, 52);
  kardeshev::UI::assets->addFont(
    kardeshev::Font::DEFAULT_FONT, "assets/kongtext.ttf", 12, 26, 52);
  kardeshev::UI::assets->addTexture("planet", "assets/test_planet.png");
  kardeshev::UI::assets->addTexture("planet_simple", "assets/planet_simple.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture("system_simple", "assets/system_simple.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture("orbit_ring", "assets/orbit_ring.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture("main_view_border", "assets/main_view_border.png");
  kardeshev::UI::assets->addTexture("star_simple", "assets/star_simple.png", 128, 128, 3);
  kardeshev::UI::assets->addTexture("galaxy_button", "assets/galaxy_button.png", 128, 64, 2);

  main_window->setupViews();
  main_window->display();

  std::cout << "killing" << std::endl;
  main_window->kill();

  return 0;
}
