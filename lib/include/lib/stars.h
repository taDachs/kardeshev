#ifndef STARS_H
#define STARS_H

#include "id.h"
#include <memory>

namespace kardeshev {
namespace lib {

class StarClass : public GameClass
{
private:
  std::string m_name;
  std::string m_description;
  double m_temp;

public:
  StarClass()
    : GameClass("none"){};
  StarClass(std::string name, std::string key, std::string description, double temp)
    : GameClass(std::move(key))
    , m_name(std::move(name))
    , m_description(std::move(description))
    , m_temp(temp){};
  std::string getName() const { return m_name; }
  std::string getDescription() const { return m_description; }
  double getTemp() const { return m_temp; }
};

const StarClass O("o", "o_star", "", 40000);
const StarClass B("b", "b_star", "", 20000);
const StarClass A("a", "a_star", "", 8500);
const StarClass F("f", "f_star", "", 6500);
const StarClass G("g", "g_star", "", 5700);
const StarClass K("k", "k_star", "", 4500);
const StarClass M("m", "m_star", "", 3200);

class Star;

struct StarInfo
{
  std::string name;
  std::shared_ptr<Star> star;
  StarClass star_class;
  std::string getNameOrId() const;
};

class Star : public GameObject
{
public:
  using Ptr = std::shared_ptr<Star>;

private:
  std::shared_ptr<StarInfo> m_info;

public:
  Star(std::shared_ptr<StarInfo> info)
    : m_info(std::move(info)){};
  std::shared_ptr<StarInfo> getInfo() const { return m_info; }
};

} // namespace lib
} // namespace kardeshev

#endif //! STARS_H
