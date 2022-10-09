#include "lib/stars.h"
#include "lib/astronomical_object_visitor.h"
using namespace kardeshev;
using namespace lib;

void Star::visited(AstronomicalObjectVisitor& visitor) {
  visitor.visitStar(*this);
}
