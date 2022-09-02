#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "resources.h"

namespace kardeshev {
class Building
{
};

class Mine : public Building
{
private:
  ResourceDeposit m_deposit;
};
} // namespace kardeshev
#endif // !BUILDINGS_H
