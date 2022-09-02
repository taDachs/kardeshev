#ifndef PLANETS_H
#define PLANETS_H

#include "id.h"
#include "population.h"
#include <buildings.h>
#include <memory>
#include <utility>
#include <vector>

namespace kardeshev {

class PlanetClass : public GameClass
{
private:
  std::string m_name;
  std::string m_description;
  std::pair<double, double> m_temp_range;

public:
  PlanetClass()
    : GameClass("none"){};
  PlanetClass(std::string name,
              std::string key,
              std::string description,
              std::pair<double, double> temp_range)
    : GameClass(std::move(key))
    , m_name(std::move(name))
    , m_description(std::move(description))
    , m_temp_range(std::move(temp_range)){};
  std::string getName() const { return m_name; }
  std::string getDescription() const { return m_description; }
  std::pair<double, double> getTempRange() const { return m_temp_range; }
};

/**
 * @brief A theoretical terrestrial planet with very little water.
 */
const PlanetClass DESERT("desert",
                         "desert_planet",
                         "A theoretical terrestrial planet with very little water.",
                         std::pair<double, double>(300, 500));
/**
 * @brief A low-mass planet composed primarily of hydrogen and helium.
 */
const PlanetClass GAS_DWARF("gas_dwarf",
                            "gas_dwarf_planet",
                            "A low-mass planet composed primarily of hydrogen and helium.",
                            std::pair<double, double>(150, 200));
/**
 * @brief A massive planet composed primarily of hydrogen and helium.
 */
const PlanetClass GAS_GIANT("gas_giant",
                            "gas_giant_planet",
                            "A massive planet composed primarily of hydrogen and helium.",
                            std::pair<double, double>(150, 200));
/**
 * @brief A hypothetical type of habitable planet described as a hot, water-covered planet with a
 * hydrogen-rich atmosphere.
 */
const PlanetClass HYCEAN("hycean",
                         "hycean_planet",
                         "A hypothetical type of habitable planet described as a hot, "
                         "water-covered planet with a hydrogen-rich atmosphere.",
                         std::pair<double, double>(300, 350));
/**
 * @brief A giant planet composed mainly of 'ices'—volatile substances heavier than hydrogen and
 * helium, such as water, methane, and ammonia—as opposed to 'gas' (hydrogen and helium).
 */
const PlanetClass ICE_GIANT(
  "ice giant",
  "ice_giant_planet",
  "A giant planet composed mainly of 'ices'—volatile substances heavier than hydrogen and helium, "
  "such as water, methane, and ammonia—as opposed to 'gas' (hydrogen and helium)",
  std::pair<double, double>(50, 150));
/**
 * @brief A theoretical planet with an icy surface and consists of a global cryosphere.
 */
const PlanetClass
  ICE("ice",
      "ice_planet",
      "A theoretical planet with an icy surface and consists of a global cryosphere.",
      std::pair<double, double>(50, 150));
/**
 * @brief A theoretical planet that consists primarily of an iron-rich core with little or no
 * mantle.
 */
const PlanetClass IRON(
  "iron",
  "iron_planet",
  "A theoretical planet that consists primarily of an iron-rich core with little or no mantle.",
  std::pair<double, double>(50, 150));
/**
 * @brief A theoretical terrestrial planet with a surface mostly or entirely covered by molten lava.
 */
const PlanetClass
  LAVA("lava",
       "lava_planet",
       "A theoretical terrestrial planet with a surface mostly or entirely covered by molten lava.",
       std::pair<double, double>(400, 700));
/**
 * @brief A theoretical planet which has a substantial fraction of its mass made of water.
 */
const PlanetClass
  OCEAN("ocean",
        "ocean_planet",
        "A theoretical planet which has a substantial fraction of its mass made of water.",
        std::pair<double, double>(290, 350));
/**
 * @brief A large planetary embryo that originates within protoplanetary discs and has undergone
 * internal melting to produce differentiated interiors. Protoplanets are believed to form out of
 * kilometer-sized planetesimals that attract each other gravitationally and collide.
 */
const PlanetClass PROTO(
  "proto",
  "proto_planet",
  "A large planetary embryo that originates within protoplanetary discs and has undergone internal "
  "melting to produce differentiated interiors. Protoplanets are believed to form out of "
  "kilometer-sized planetesimals that attract each other gravitationally and collide.",
  std::pair<double, double>(400, 700));
/**
 * @brief Also known as a telluric planet or rocky planet. A planet that is composed primarily of
 * carbonaceous or silicate rocks or metals.
 */
const PlanetClass TERRESTIAL("terestial",
                             "terestial_planet",
                             "Also known as a telluric planet or rocky planet. A planet that is "
                             "composed primarily of carbonaceous or silicate rocks or metals.",
                             std::pair<double, double>(200, 400));
class Planet;

struct PlanetInfo
{
  std::string name;
  std::shared_ptr<Planet> planet;
  PlanetClass planet_class;
  double temperature;
  std::vector<ResourceDeposit> resources;
  std::string getNameOrId() const;
};

class Planet : public GameObject
{
private:
  std::vector<Population> m_pops;
  std::vector<Building> m_buildings;
  std::shared_ptr<PlanetInfo> m_info;

public:
  Planet(std::shared_ptr<PlanetInfo> info)
    : m_info(std::move(info)){};
  std::vector<Population> getPops() const { return m_pops; };
  std::vector<Building> getBuildings() const { return m_buildings; };
  std::shared_ptr<PlanetInfo> getInfo() const { return m_info; }
};

} // namespace kardeshev

#endif // !PLANETS_H
