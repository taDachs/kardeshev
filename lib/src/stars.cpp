#include "lib/stars.h"
using namespace kardeshev;
using namespace lib;

std::string StarInfo::getNameOrId() const
{
  if (name.empty())
  {
    return star->getIdAsString();
  }
  return name;
}
