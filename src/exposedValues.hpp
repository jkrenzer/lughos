#ifndef EXPOSED_VALUES_HPP
#define EXPOSED_VALUES_HPP
#include "rawValues.hpp"
namespace lughos
{
  class ExposedObject : public TreeNode
  {
  public:
    ExposedObject()
    {
      this->addChild(new Pointer<std::string>(&this->name,"name"));
      this->addChild(new Value<std::string>("N/A","description"));
    }
    
  };
}//namespace lughos
#endif