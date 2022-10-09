#ifndef ASTRONOMICAL_OBJECT_H
#define ASTRONOMICAL_OBJECT_H

#include "lib/buildings.h"
#include "lib/id.h"
#include "lib/population.h"
#include "lib/units.h"
#include <map>
#include <memory>

namespace kardeshev {
namespace lib {

class AstronomicalObjectVisitor;

class AstronomicalObject : public GameObject
{
public:
  using Ptr = std::shared_ptr<AstronomicalObject>;
protected:
  int m_num_build_slots;
  std::vector<std::unique_ptr<Building>> m_buildings;
  std::vector<Population> m_populations;
  std::vector<ResourceDeposit> m_resource_deposits;
  AstronomicalObject::Ptr m_parent;
  std::string m_name;
  Mass m_mass;
  Distance m_orbit_distance;
  Duration m_orbit_duration;
  Temperature m_temperature;

public:
  Distance getOrbitDistance() const { return m_orbit_distance; }
  Duration getOrbitDuration() const { return m_orbit_duration; }
  Temperature getTemperature() const { return m_temperature; }
  Mass getMass() const { return m_mass; }
  std::string getName() const { return m_name; }

  void setName(const std::string& name) { m_name = name; }
  void setOrbitDistance(const Distance& distance) { m_orbit_distance = distance; }
  void setOrbitDuration(const Duration& duration) { m_orbit_duration = duration; }
  void setTemperature(const Temperature& temperature) { m_temperature = temperature; }
  void setMass(const Mass& mass) { m_mass = mass; }
  void setParent(const AstronomicalObject::Ptr& parent) { m_parent = parent; }

  virtual int getNumBuildSlots() const { return m_num_build_slots; }
  virtual std::vector<std::unique_ptr<Building>>& getBuildings() { return m_buildings; }
  virtual std::vector<Population> getPopulations() const { return m_populations; }
  virtual void addBuilding(std::unique_ptr<Building> building) { m_buildings.push_back(std::move(building)); }
  virtual void addPopulation(const Population& population) { m_populations.push_back(population); }
  virtual std::vector<ResourceDeposit> getDeposits() const { return m_resource_deposits; }
  virtual AstronomicalObject::Ptr getParent() const { return m_parent; }
  virtual void addResourceDeposit(const ResourceDeposit& deposit) { m_resource_deposits.push_back(deposit); }

  virtual void visited(AstronomicalObjectVisitor& visitor) = 0;

  int getCurrentAngle(const Duration& time) const
  {
    // this is okay because the orbit duraiton shouldn't be that large
    auto day_of_year     = static_cast<double>(time.getDays() % m_orbit_duration.getDays());
    return static_cast<int>((day_of_year / m_orbit_duration.getDays()) * 360.0);
  }
};

} // namespace lib
} // namespace kardeshev

#endif // !ASTRONOMICAL_OBJECT_H
