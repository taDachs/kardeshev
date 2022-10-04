#ifndef MAIN_H
#define MAIN_H

#include "generation/galaxy_generation.h"
#include "generation/name_generation.h"
#include "generation/planet_generation.h"
#include "generation/solar_system_generation.h"
#include "generation/star_generation.h"
#include "lib/game.h"

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

kardeshev::PlanetGenerator::Ptr pg;
kardeshev::StarGenerator::Ptr sg;
kardeshev::SolarSystemGenerator::Ptr ssg;
kardeshev::GalaxyGenerator::Ptr gg;
kardeshev::NameGenerator::Ptr ng;

void setupGenerators();

#endif // !MAIN_H
