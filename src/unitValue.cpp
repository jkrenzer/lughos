#include <ostream>
#include <limits>
// #pragma comment(lib, "Setupapi.lib")
#include "unitValue.hpp"
#include <boost/math/special_functions.hpp>

unitValue::unitValue(void)
{
 
}

unitValue::unitValue(double putvalue, std::string putunit)
{
}

unitValue::~unitValue(void)
{

}

void unitValue::setunit(std::string putunit)
{
  this->unit = putunit;
    return;
  
}

void unitValue::setvalue(double putvalue)
{
  this->value = putvalue;
    return;
}

void unitValue::setunitvalue(double putvalue, std::string putunit)
{
  setvalue(putvalue);
  setunit(putunit);
    return;
}

void unitValue::setunitvalue(int putvalue, std::string putunit)
{
  setvalue((double)putvalue);
  setunit(putunit);
    return;
}

double unitValue::getvalue()
{
    return value;
}

std::string unitValue::getunit()
{
    return unit;
}

bool unitValue::isPositiveInfinity()
{
  return (this->value > 0 && boost::math::isinf(this->value));
}

bool unitValue::isNegativeInfinity()
{
  return (this->value < 0 && boost::math::isinf(this->value));
}

bool unitValue::isNotANumber()
{
  return boost::math::isnan(this->value);
}

bool unitValue::isValidValue()
{
  return boost::math::isnormal(this->value);
}