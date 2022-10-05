#ifndef MAIN_H
#define MAIN_H

#include "lib/game.h"
#include "lib/generators.h"
#include "ui/window.h"

const std::vector<kardeshev::lib::PlanetClass> PLANETS{kardeshev::lib::DESERT,
                                                       kardeshev::lib::GAS_DWARF,
                                                       kardeshev::lib::GAS_GIANT,
                                                       kardeshev::lib::HYCEAN,
                                                       kardeshev::lib::ICE_GIANT,
                                                       kardeshev::lib::ICE,
                                                       kardeshev::lib::IRON,
                                                       kardeshev::lib::LAVA,
                                                       kardeshev::lib::OCEAN,
                                                       kardeshev::lib::PROTO,
                                                       kardeshev::lib::TERRESTIAL};
const std::vector<kardeshev::lib::StarClass> STARS{
  kardeshev::lib::O, kardeshev::lib::B, kardeshev::lib::G};
kardeshev::GameWindow::Ptr main_window;
kardeshev::generation::PlanetGenerator::Ptr pg;
kardeshev::generation::StarGenerator::Ptr sg;
kardeshev::generation::SolarSystemGenerator::Ptr ssg;
kardeshev::generation::GalaxyGenerator::Ptr gg;
kardeshev::generation::NameGenerator::Ptr ng;

void setupGenerators();

#endif // !MAIN_H
