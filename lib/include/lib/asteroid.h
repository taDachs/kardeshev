#ifndef ASTEROID_H
#define ASTEROID_H

#include "lib/astronomical_object.h"
#include <memory>
namespace kardeshev {
namespace lib {
class AstronomicalObjectVisitor;
class Asteroid: public AstronomicalObject {
public:
  using Ptr = std::shared_ptr<Asteroid>;
private:
  double m_angle;
public:
  void visited(AstronomicalObjectVisitor& visitor) override;
  double getCurrentAngle(const Duration& time) const override {
    return m_angle;
  }
  void setAngle(const double angle) { m_angle = angle; }
};
}
}

#endif // !ASTEROID_H
