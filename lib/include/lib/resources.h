#ifndef RESOURCES_H
#define RESOURCES_H

// Every amount is meant in metric tons

#include "id.h"
#include <map>
#include <string>
#include <vector>
namespace kardeshev {
namespace lib {
class Requirement;
class ResourceType : public GameClass
{
private:
  std::string m_name;
  std::vector<Requirement> m_components;

public:
  ResourceType()
    : GameClass("none")
  {
  }
  /**
   * @brief Constructor for ResourceType
   *
   * @param id unique identifier for resource, must not be shared between any resource
   * @param name name of resources
   * @param rarity percentage value of this resource being encountered in a location spot
   */
  ResourceType(const std::string& key, std::string name, std::vector<Requirement> components = {})
    : GameClass(key)
    , m_name(std::move(name))
    , m_components(std::move(components))
  {
  }
  std::string getName() const { return m_name; }
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

public:
  ResourceDeposit(ResourceType type, double amount)
    : m_type(std::move(type))
    , m_amount(amount)
  {
  }
  ResourceType getType() const { return m_type; }
  double getAmount() const { return m_amount; }
  double mine(double amount) {
    m_amount -= amount;
    return m_amount;
  }
};

class ResourceSupply
{
private:
  ResourceType m_type;
  size_t m_amount;

public:
  ResourceSupply(ResourceType type, size_t amount)
    : m_type(std::move(type))
    , m_amount(amount)
  {
  }
  ResourceType getType() const { return m_type; }
  size_t getAmount() const { return m_amount; }
  void addAmount(size_t amount) { m_amount += amount; }
  void removeAmount(size_t amount) { m_amount -= amount; }
};

class Need
{
private:
  ResourceType m_type;
  size_t m_amount;

public:
  Need(ResourceType type, size_t amount)
    : m_type(std::move(type))
    , m_amount(amount)
  {
  }
  ResourceType getType() const { return m_type; }
  size_t getAmount() const { return m_amount; }
};


class ResourceList
{
private:
  std::map<std::string, size_t> m_map;
  // TODO: repalce this
  std::map<std::string, ResourceType> m_type_helper_map;

public:
  void addSupply(const ResourceSupply& supply)
  {
    std::string key        = supply.getType().getKey();
    m_type_helper_map[key] = supply.getType();
    if (!m_map.count(key))
    {
      m_map[key] = supply.getAmount();
    }
    else
    {
      m_map[key] += supply.getAmount();
    }
  }
  ResourceSupply getSupply(const ResourceType& type)
  {
    std::string key = type.getKey();
    if (!m_map.count(key))
    {
      return {type, 0};
    }
    return {type, m_map[key]};
  }
  std::vector<ResourceSupply> getSupplies()
  {
    std::vector<ResourceSupply> out;
    for (const auto& supply : m_map)
    {
      out.emplace_back(m_type_helper_map[supply.first], supply.second);
    }
    return out;
  }
  Need provideNeed(const Need& need)
  {
    std::string key = need.getType().getKey();
    if (!m_map.count(key))
    {
      return need;
    }
    if (m_map[key] >= need.getAmount())
    {
      m_map[key] -= need.getAmount();
      return {need.getType(), 0};
    }
    size_t amount_left = need.getAmount() - m_map[key];
    m_map[key]         = 0;
    return {need.getType(), amount_left};
  }
};

} // namespace lib
} // namespace kardeshev

#endif // !RESOURCES_H
