#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "unitValue.hpp"


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