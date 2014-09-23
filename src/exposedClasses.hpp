#ifndef EXPOSED_CLASSES_HPP
#define EXPOSED_CLASSES_HPP
#include "Values.hpp"
#include "treeObj.hpp"

namespace lughos
{

class ExposedClassInterface : public TreeNode
{
  
};

template <class C> class ExposedClassImplementation : public ExposedClassInterface
{
public:
  
  virtual void exposeClass(C c) = 0;
  
};

template <class C> class ExposedClass : ExposedClassImplementation<C>
{
  
};

} //namespace lughos
#endif