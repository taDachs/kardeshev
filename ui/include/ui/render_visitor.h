#ifndef RENDER_VISITOR_H
#define RENDER_VISITOR_H
#include "lib/asteroid.h"
#include "lib/astronomical_object_visitor.h"
#include "ui/entities.h"

namespace kardeshev {
namespace ui {
class RenderVisitor : public lib::AstronomicalObjectVisitor {
private:
  std::vector<Entity::Ptr> m_entities;
public:
  void visitPlanet(lib::Planet& obj) override {
    m_entities.push_back(std::make_shared<PlanetEntity>(std::static_pointer_cast<lib::Planet>(obj.shared_from_this())));
  };
  void visitStar(lib::Star& obj) override {
    m_entities.push_back(std::make_shared<StarEntity>(std::static_pointer_cast<lib::Star>(obj.shared_from_this())));
  };
  void visitAsteroid(lib::Asteroid& obj) override {
    m_entities.push_back(std::make_shared<AsteroidEntity>(std::static_pointer_cast<lib::Asteroid>(obj.shared_from_this())));
  };
  std::vector<Entity::Ptr> getEntities() const { return m_entities; }
};

}
}

#endif // !RENDER_VISITOR_H
