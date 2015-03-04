#ifndef EXPOSED_CLASSES_HPP
#define EXPOSED_CLASSES_HPP
#include "exposedValues.hpp"

namespace lughos
{

class ExposedClass : public ExposedObject
{
protected:
  ExposerRegistry memberRegistry;
public:
  
};

} //namespace lughos
#endif