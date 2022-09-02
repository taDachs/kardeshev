#include "id.h"
#include "util.h"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cmath>
#include <iostream>
#include <string>


using namespace kardeshev;

GameObject::GameObject()
{
  boost::uuids::random_generator gen;
  m_id = gen();
}

std::string GameObject::getIdAsString() const
{
  const unsigned long OFFSET = 133742069;
  /* long encoded = m_number_id + OFFSET; */
  /**/
  /* int number_zeros = 0; */
  /* unsigned long r = encoded; */
  /* while ((r / (powl(10, number_zeros))) > 1) { */
  /*   number_zeros++; */
  /* } */
  /**/
  /* int upper = r / static_cast<int>(powl(10, number_zeros / 2)); */
  /* int lower = r % static_cast<int>(powl(10, number_zeros / 2)); */
  /* std::string base32_encoding = encodeBase32(upper); */
  /* std::string lower_string = std::to_string(lower); */
  /**/
  /* return base32_encoding + "-" + lower_string; */
  return boost::lexical_cast<std::string>(m_id);
}
