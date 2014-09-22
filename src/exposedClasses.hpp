#ifndef EXPOSED_CLASSES_HPP
#define EXPOSED_CLASSES_HPP
#include "exposedValues.hpp"
#include "treeObj.hpp"

namespace lughos
{

class exposedClassInterface : public treeNode
{
  
};

template <class C> class exposedClassImplementation : public exposedClassInterface
{
public:
  
  virtual void exposeClass(C c) = 0;
  
};

template <class C> class exposedClass : exposedClassImplementation<C>
{
  
};

} //namespace lughos
#endif