#include "art.h"
#include "ascii_planets.h"
#include "buildings.h"
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


int main_old()
{
  auto pg = std::make_shared<kardeshev::NaivePlanetGenerator>(PLANETS);
  auto sg = std::make_shared<kardeshev::NaiveStarGenerator>(STARS);
  kardeshev::NaiveSolarSystemGenerator ssg(pg, sg, 3, 10);

  std::map<std::string, std::string> visualizer;
  visualizer[kardeshev::DESERT.getKey()]     = kardeshev::ASCII_MARS;
  visualizer[kardeshev::GAS_DWARF.getKey()]  = kardeshev::ASCII_VENUS;
  visualizer[kardeshev::GAS_GIANT.getKey()]  = kardeshev::ASCII_JUPITER;
  visualizer[kardeshev::HYCEAN.getKey()]     = kardeshev::ASCII_EARTH;
  visualizer[kardeshev::ICE_GIANT.getKey()]  = kardeshev::ASCII_PLUTO;
  visualizer[kardeshev::ICE.getKey()]        = kardeshev::ASCII_PLUTO;
  visualizer[kardeshev::IRON.getKey()]       = kardeshev::ASCII_MERKUR;
  visualizer[kardeshev::LAVA.getKey()]       = kardeshev::ASCII_VENUS;
  visualizer[kardeshev::OCEAN.getKey()]      = kardeshev::ASCII_EARTH;
  visualizer[kardeshev::PROTO.getKey()]      = kardeshev::ASCII_VENUS;
  visualizer[kardeshev::TERRESTIAL.getKey()] = kardeshev::ASCII_MERKUR;
  visualizer[kardeshev::O.getKey()]          = kardeshev::ASCII_O_STAR;
  visualizer[kardeshev::B.getKey()]          = kardeshev::ASCII_B_STAR;
  visualizer[kardeshev::G.getKey()]          = kardeshev::ASCII_SUN;

  kardeshev::ResourceType potatoes("potato_resource", "Potatoes");
  kardeshev::Need food_req(potatoes, 1);
  std::shared_ptr<kardeshev::Building> potato_farm = std::make_shared<kardeshev::Farm>(potatoes);
  kardeshev::Species humans("human_species", "Humans", 1.4, {food_req});
  kardeshev::Population pop(humans, {});

  kardeshev::TerminalVisualizer tv(visualizer);


  /* std::shared_ptr<kardeshev::SolarSystem> ss = ssg.generateSolarSystem(); */
  std::shared_ptr<kardeshev::Planet> planet = pg->generatePlanet();
  planet->addBuilding(potato_farm);
  planet->addPop(pop);
  while (true)
  {
    std::cout << tv.drawPlanet(*planet) << std::endl;
    /* std::cout << "Next action: q(uit), s(tep)"; */
    sleep(1);
    planet->update();
    /* std::string next; */
    /* std::cin >> next; */
    /* if (next == "s" || next.empty()) { */
    /*   planet->update(); */
    /* } else if (next == "q") { */
    /*   break; */
    /* } else { */
    /*   std::cout << "Invalid input" << std::endl; */
    /* } */
  }

  return 0;
}

int main()
{
  std::cout << "Setting up generators" << std::endl;
  auto pg = std::make_shared<kardeshev::NaivePlanetGenerator>(PLANETS);
  auto sg = std::make_shared<kardeshev::NaiveStarGenerator>(STARS);
  kardeshev::NaiveSolarSystemGenerator ssg(pg, sg, 3, 10);

  std::cout << "Generating solar system" << std::endl;
  std::shared_ptr<kardeshev::SolarSystem> ss = ssg.generateSolarSystem();

  std::cout << "Creating render" << std::endl;
  auto render = std::make_shared<kardeshev::Render>();
  std::cout << "Creating system artist" << std::endl;
  auto system_artist = std::make_shared<kardeshev::SystemView>();
  system_artist->setSystem(ss);
  render->setArtist(system_artist);

  std::cout << "init render" << std::endl;
  render->init();
  std::cout << "displaying" << std::endl;
  render->display();
  std::cout << "killing" << std::endl;
  render->kill();

  return 0;
}
