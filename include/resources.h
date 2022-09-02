#ifndef RESOURCES_H
#define RESOURCES_H

// Every amount is meant in metric tons

#include "id.h"
#include <string>
#include <utility>
#include <vector>
namespace kardeshev {
class Requirement;

class ResourceType : public GameClass
{
private:
  std::string m_name;
  double m_rarity;
  std::vector<Requirement> m_components;

public:
  /**
   * @brief Constructor for ResourceType
   *
   * @param id unique identifier for resource, must not be shared between any resource
   * @param name name of resources
   * @param rarity percentage value of this resource being encountered in a location spot
   */
  ResourceType(const std::string& key,
               std::string name,
               const double rarity,
               std::vector<Requirement> components)
    : GameClass(key)
    , m_name(std::move(name))
    , m_rarity(rarity)
    , m_components(std::move(components))
  {
  }
};

class Requirement
{
private:
  ResourceType m_type;
  double m_amount;

public:
  Requirement(ResourceType type, double amount)
    : m_type(std::move(type))
    , m_amount(amount)
  {
  }
  ResourceType getType() const { return m_type; }
  double getAmout() const { return m_amount; }
};

class ResourceDeposit
{
private:
  ResourceType m_type;
  double m_amount;
  double m_mined = 0;

public:
  ResourceDeposit(ResourceType type, double amount)
    : m_type(std::move(type))
    , m_amount(amount)
  {
  }
  ResourceType getType() const { return m_type; }
  double getAmount() const { return m_amount; }
  double getMined() const { return m_mined; }
};

class ResourceShipment
{
private:
  ResourceType m_type;
  double m_amount;

public:
  ResourceShipment(ResourceType type, double amount)
    : m_type(std::move(type))
    , m_amount(amount)
  {
  }
  ResourceType getType() const { return m_type; }
  double getAmount() const { return m_amount; }
};

} // namespace kardeshev

#endif // !RESOURCES_H
