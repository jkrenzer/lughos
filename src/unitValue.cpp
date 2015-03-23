#include <ostream>
#include <limits>
// #pragma comment(lib, "Setupapi.lib")
#include "unitValue.hpp"
#include <boost/math/special_functions.hpp>

namespace lughos
{



void unitValueInterface::setUnit(std::string putunit)
{
  this->unit = putunit;
    return;
  
}

std::string unitValueInterface::getUnit() const
{
    return unit;
}

} //namespace lughos