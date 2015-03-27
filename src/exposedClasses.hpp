#ifndef EXPOSED_CLASSES_HPP
#define EXPOSED_CLASSES_HPP
#include "exposedValues.hpp"
#include "exposedFunctions.hpp"

namespace lughos
{

  class ExposedClass : public ExposedObject
  {
  protected:
    ExposerRegistry members;
    
    bool declared;
    
    virtual void memberDeclaration() = 0;
    
    void addMember(ExposedObject& object)
    {
      this->addMember(&object);
    }
    
    void addMember(ExposedObject* object)
    {
      this->members.addObject(object);
    }
    
    template <class ReturnType, class... Arguments> void addFunction(boost::function<ReturnType(Arguments...)> f, std::string name,std::string description = "")
    {
      this->addMember(new ExposedFunction<ReturnType, Arguments...>(f,name));
    }
    
    template <class T> void addMember(T& member, std::string name, std::string description = "")
    {
      this->addMember(new ExposedValue<T>(member,name));
    }
    
  public:
    
    ExposedObject* operator[](std::string name)
    {
      if(!this->declared)
	this->memberDeclaration();
      return members[name];
    }
    
    ExposedObject* operator[](int i)
    {
      if(!this->declared)
	this->memberDeclaration();
      return members[i];
    }
    
    ExposedClass()
    {
      this->declared = false;
    }
       
  };

} //namespace lughos
#endif