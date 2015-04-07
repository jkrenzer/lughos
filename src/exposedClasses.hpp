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
    
    template <class T>
    ExposedValue<T>* get(std::string name)
    {
      ExposedValue<T>* ptr = dynamic_cast<ExposedValue<T>* >(this->operator[](name));
      if(ptr != nullptr)
        return ptr;
      else
        BOOST_THROW_EXCEPTION( exception() << errorName("wrong_cast_access_exposed_value"));
    }
    
    ExposedObject* operator[](int i)
    {
      if(!this->declared)
	this->memberDeclaration();
      return members[i];
    }
    
    template <class T>
    ExposedValue<T>* get(int i)
    {
      ExposedValue<T>* ptr = dynamic_cast<ExposedValue<T>* >(this->operator[](i));
      if(ptr != nullptr)
        return ptr;
      else
        BOOST_THROW_EXCEPTION( exception() << errorName("wrong_cast_access_exposed_value"));
    }
    
    ExposedClass()
    {
      this->declared = false;
    }
       
  };

} //namespace lughos
#endif