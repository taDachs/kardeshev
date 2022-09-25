#include "lib/id.h"
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
  return boost::lexical_cast<std::string>(m_id);
}
