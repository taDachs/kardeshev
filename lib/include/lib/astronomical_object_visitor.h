#ifndef ASTRONOMICAL_OBJECT_VISITOR_H
#define ASTRONOMICAL_OBJECT_VISITOR_H

#include "lib/astronomical_object.h"
#include "lib/planets.h"
#include "lib/stars.h"

namespace kardeshev {
namespace lib {

class AstronomicalObjectVisitor
{
public:
  void visit(AstronomicalObject& obj) { obj.visited(*this); }
  virtual void visitPlanet(Planet& obj) {};
  virtual void visitStar(Star& obj) {};
};
} // namespace lib

} // namespace kardeshev

#endif // !ASTRONOMICAL_OBJECT_VISITOR_H
