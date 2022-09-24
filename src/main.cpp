/* #include "art.h" */
#include "ascii_planets.h"
#include "buildings.h"
#include "galaxy_generation.h"
#include "planet_generation.h"
#include "planets.h"
#include "population.h"
#include "solar_system_generation.h"
#include "solar_systems.h"
#include "star_generation.h"
#include "stars.h"
#include <SDL.h>
#include <iostream>
#include <map>
#include <memory>
#include "render.h"
const std::vector<kardeshev::PlanetClass> PLANETS{kardeshev::DESERT,
                                            kardeshev::GAS_DWARF,
                                            kardeshev::GAS_GIANT,
                                            kardeshev::HYCEAN,
                                            kardeshev::ICE_GIANT,
                                            kardeshev::ICE,
                                            kardeshev::IRON,
                                            kardeshev::LAVA,
                                            kardeshev::OCEAN,
                                            kardeshev::PROTO,
                                            kardeshev::TERRESTIAL};
const std::vector<kardeshev::StarClass> STARS{kardeshev::O, kardeshev::B, kardeshev::G};

/**/
/* int main_old() */
/* { */
/*   auto pg = std::make_shared<kardeshev::NaivePlanetGenerator>(PLANETS); */
/*   auto sg = std::make_shared<kardeshev::NaiveStarGenerator>(STARS); */
/*   kardeshev::NaiveSolarSystemGenerator ssg(pg, sg, 3, 10); */
/**/
/*   std::map<std::string, std::string> visualizer; */
/*   visualizer[kardeshev::DESERT.getKey()]     = kardeshev::ASCII_MARS; */
/*   visualizer[kardeshev::GAS_DWARF.getKey()]  = kardeshev::ASCII_VENUS; */
/*   visualizer[kardeshev::GAS_GIANT.getKey()]  = kardeshev::ASCII_JUPITER; */
/*   visualizer[kardeshev::HYCEAN.getKey()]     = kardeshev::ASCII_EARTH; */
/*   visualizer[kardeshev::ICE_GIANT.getKey()]  = kardeshev::ASCII_PLUTO; */
/*   visualizer[kardeshev::ICE.getKey()]        = kardeshev::ASCII_PLUTO; */
/*   visualizer[kardeshev::IRON.getKey()]       = kardeshev::ASCII_MERKUR; */
/*   visualizer[kardeshev::LAVA.getKey()]       = kardeshev::ASCII_VENUS; */
/*   visualizer[kardeshev::OCEAN.getKey()]      = kardeshev::ASCII_EARTH; */
/*   visualizer[kardeshev::PROTO.getKey()]      = kardeshev::ASCII_VENUS; */
/*   visualizer[kardeshev::TERRESTIAL.getKey()] = kardeshev::ASCII_MERKUR; */
/*   visualizer[kardeshev::O.getKey()]          = kardeshev::ASCII_O_STAR; */
/*   visualizer[kardeshev::B.getKey()]          = kardeshev::ASCII_B_STAR; */
/*   visualizer[kardeshev::G.getKey()]          = kardeshev::ASCII_SUN; */
/**/
/*   kardeshev::ResourceType potatoes("potato_resource", "Potatoes"); */
/*   kardeshev::Need food_req(potatoes, 1); */
/*   std::shared_ptr<kardeshev::Building> potato_farm = std::make_shared<kardeshev::Farm>(potatoes); */
/*   kardeshev::Species humans("human_species", "Humans", 1.4, {food_req}); */
/*   kardeshev::Population pop(humans, {}); */
/**/
/*   kardeshev::TerminalVisualizer tv(visualizer); */
/**/
/**/
/*   std::shared_ptr<kardeshev::Planet> planet = pg->generatePlanet(); */
/*   planet->addBuilding(potato_farm); */
/*   planet->addPop(pop); */
/*   while (true) */
/*   { */
/*     std::cout << tv.drawPlanet(*planet) << std::endl; */
/*     sleep(1); */
/*     planet->update(); */
/*   } */
/**/
/*   return 0; */
/* } */
/**/
int main()
{
  auto game = std::make_shared<kardeshev::Game>();
  std::cout << "Setting up generators" << std::endl;
  auto pg = std::make_shared<kardeshev::NaivePlanetGenerator>(PLANETS);
  auto sg = std::make_shared<kardeshev::NaiveStarGenerator>(STARS);
  auto ssg = std::make_shared<kardeshev::NaiveSolarSystemGenerator>(pg, sg, 3, 10);

  auto gg = std::make_shared<kardeshev::NaiveGalaxyGenerator>(ssg, 500, 1000);

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
  auto ui_state = std::make_shared<kardeshev::UIState>();

  auto system_artist = std::make_shared<kardeshev::SystemView>(ui_state, game);
  auto galaxy_artist = std::make_shared<kardeshev::GalaxyViewArtist>(ui_state, game);
  auto system_info_artist = std::make_shared<kardeshev::SystemInfoViewArtist>(ui_state, game);
  auto planet_info_artist = std::make_shared<kardeshev::PlanetInfoViewArtist>(ui_state, game);

  galaxy_artist->setGalaxy(g);

  std::cout << "Creating window" << std::endl;
  auto main_window = std::make_shared<kardeshev::GameWindow>();
  main_window->setGame(game);
  main_window->setUIState(ui_state);
  main_window->init();

  main_window->setGalaxyViewArtist(galaxy_artist);
  main_window->setSystemViewArtist(system_artist);
  main_window->getSidebarRender()->setArtist(system_info_artist);
  main_window->getBottomBarRender()->setArtist(planet_info_artist);
  main_window->display();

  std::cout << "killing" << std::endl;
  main_window->kill();

  return 0;
}
