#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H
#include "id.h"
#include "planets.h"
#include "stars.h"

namespace kardeshev {
class SolarSystem;
struct SolarSystemInfo
{
  std::string name;
  std::shared_ptr<SolarSystem> solar_system;
  std::string getNameOrId() const;
};

class SolarSystem : public GameObject
{
private:
  std::shared_ptr<Star> m_star;
  std::vector<std::shared_ptr<Planet> > m_planets;
  std::shared_ptr<SolarSystemInfo> m_info;

public:
  SolarSystem(std::shared_ptr<SolarSystemInfo> info, std::shared_ptr<Star> star, std::vector<std::shared_ptr<Planet> > planets)
    : m_info(std::move(info))
    , m_star(std::move(star))
    , m_planets(std::move(planets))
  {
  }
  std::vector<std::shared_ptr<Planet> > getPlanets() const { return m_planets; }
  std::shared_ptr<SolarSystemInfo> getInfo() const { return m_info; }
  std::shared_ptr<Star> getStar() const { return m_star; }
  void update() override;
};
} // namespace kardeshev
#endif //! SOLAR_SYSTEM_H
