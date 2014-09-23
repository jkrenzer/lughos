#ifndef EXPOSED_VALUES_HPP
#define EXPOSED_VALUES_HPP

#include "rawValues.hpp"
#include "treeObj.hpp"
#include <boost/concept_check.hpp>
namespace lughos
{
  class ExposedObject : public TreeNode
  {
  public:
    ExposedObject()
    {
      this->init();
    }
    
  protected:
    virtual void init()
    { 
      this->addChild(new Pointer<std::string>(&this->name,"name"));
      this->addChild(new Value<std::string>("N/A","description"));
    }
    
  };

  
template <class T> class ExposedValue : public ExposedObject
  {
  public:
    ExposedValue(T value, std::string name, std::string description = std::string("N/A"))
    {
      this->addChild(new Value<T>(value,"value"));
      this->getAs<Value<std::string> >("name")->setValue(name);
      this->getAs<Value<std::string> >("description")->setValue(description);
    }
    
  }; 
  
  template <class T> class ExposedPointer : public ExposedObject
  {
  public:
    ExposedPointer(T* pointer, std::string name, std::string description = std::string("N/A"))
    {
      this->addChild(new Pointer<T>(pointer,"value"));
      this->getAs<Value<std::string> >("name")->setValue(name);
      this->getAs<Value<std::string> >("description")->setValue(description);
    }
  };
}//namespace lughos
#endif