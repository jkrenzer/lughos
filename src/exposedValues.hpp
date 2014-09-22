#ifndef EXPOSED_VALUES_HPP
#define EXPOSED_VALUES_HPP
#include "rawValues.hpp"
namespace lughos
{
  class exposedObject : public treeNode
  {
  public:
    exposedObject()
    {
      this->addChild(new Pointer<std::string>(&this->name,"name"));
      this->addChild(new Value<std::string>("N/A","description"));
    }
    
  };
}//namespace lughos
#endif