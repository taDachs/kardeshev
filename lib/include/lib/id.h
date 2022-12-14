#ifndef ID_H
#define ID_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>
#include <utility>

namespace kardeshev {
namespace lib {
class GameObject
{
private:
  boost::uuids::uuid m_id;

public:
  GameObject();
  std::string getIdAsString() const;
  bool operator==(GameObject& other) { return (other.m_id == this->m_id); }
  virtual void update(){};
};

class GameClass
{
private:
  std::string m_key;

public:
  GameClass(std::string key)
    : m_key(std::move(key))
  {
  }
  bool operator==(const GameClass& other) { return (other.m_key == this->m_key); }
  std::string getKey() const { return m_key; }
};
} // namespace lib
} // namespace kardeshev
#endif // !ID_H
