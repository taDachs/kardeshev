#include "lib/asteroid.h"
#include "lib/astronomical_object_visitor.h"

using namespace kardeshev;
using namespace lib;
void Asteroid::visited(AstronomicalObjectVisitor& visitor) {
  visitor.visitAsteroid(*this);
}
