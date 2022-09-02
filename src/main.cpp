#include "art.h"
#include "ascii_planets.h"
#include "planet_generation.h"
#include "planets.h"
#include "solar_system_generation.h"
#include "solar_systems.h"
#include "star_generation.h"
#include "stars.h"
#include <iostream>
#include <map>


int main()
{
  std::vector<kardeshev::PlanetClass> planets{kardeshev::DESERT,
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
  std::vector<kardeshev::StarClass> stars{kardeshev::O, kardeshev::B, kardeshev::G};

  auto pg = std::make_shared<kardeshev::NaivePlanetGenerator>(planets);
  auto sg = std::make_shared<kardeshev::NaiveStarGenerator>(stars);
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

  kardeshev::TerminalVisualizer tv(visualizer);

  std::shared_ptr<kardeshev::SolarSystem> ss = ssg.generateSolarSystem();
  for (const auto& p : ss->getPlanets())
  {
    std::cout << p->getInfo()->getNameOrId() << std::endl;
  }

  return 0;
}
