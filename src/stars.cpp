#include "stars.h"
using namespace kardeshev;

std::string StarInfo::getNameOrId() const
{
  if (name.empty())
  {
    return star->getIdAsString();
  }
  return name;
}
