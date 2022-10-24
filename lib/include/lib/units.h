#ifndef UNITS_H
#define UNITS_H

#include <iostream>
namespace kardeshev {
namespace lib {


class Position
{
private:
  int m_x = 0;
  int m_y = 0;
public:
  Position() = default;
};

class Distance
{
private:
  double m_au = 0;

public:
  Distance(double au)
    : m_au(au)
  {
  }
  Distance() = default;
  unsigned long long int getInKm() const { return m_au * 1.496e+8; }
  double getInAU() const { return m_au; }
};

class Mass
{
private:
  double m_earth_masses = 0;

public:
  Mass() = default;
  Mass(double earth_masses)
    : m_earth_masses(earth_masses)
  {
  }
  double getInEarthMasses() const { return m_earth_masses; }
};

using verylargenumber = unsigned long long int;

class Duration
{
private:
  verylargenumber m_days;

public:
  Duration(verylargenumber days = 0)
    : m_days(days)
  {
  }
  verylargenumber getDays() const { return m_days; }
  void add(verylargenumber a) { m_days += a; }
};

class Temperature {
private:
  double m_kelvin;
public:
  Temperature() = default;
  Temperature(double kelvin): m_kelvin(kelvin) {}

  double getInKelvin() const {
    return m_kelvin;
  }

  double getInC() const {
    return m_kelvin - 273.0;
  }
};

} // namespace lib
} // namespace kardeshev

#endif // !UNITS_H
