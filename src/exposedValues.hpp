#ifndef EXPOSE_HPP
#define EXPOSE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <typeinfo>       // operator typeid
#include <typeindex>
#include <boost/concept_check.hpp>
#include "errorHandling.hpp"
#include "transformations.hpp"

#define REGISTER_CLASS_FAMILY(name) template <class T> name<T>& _ ## name (name<T> &d) { return d; }

namespace lughos
{

  
  
class exposedObject
{
protected:
  std::string name;
  std::string shortDescription;
  std::string description;
public:
  
  exposedObject() : name("N/A"), description("N/A")
  {
  }
  
  exposedObject(std::string name, std::string description = "")
  {
    this->name = name;
    this->description = description;
  }
  
  ~exposedObject()
  {
  }
  
  std::string getName()
  {
    return this->name;
  }
  
  void setName(std::string name)
  {
    this->name = name;
  }
  
  std::string getShortDescription()
  {
    return this->description;
  }
  
  void setShortDescription(std::string description)
  {
    this->description = description;
  }
    
  std::string getDescription()
  {
    return this->description;
  }
  
  void setDescription(std::string description)
  {
    this->description = description;
  }
  
};
  
class exposedValueInterface
{
public:
  
  exposedValueInterface()
  {
  }
  
  virtual ~exposedValueInterface()
  {
    
  }
  
   
};

class valueDeclarationInterface
{
public:
  virtual bool verify() = 0;
  
  virtual std::string getTypeName() = 0;
  
  virtual std::string getTypeShortDescription() = 0;
        
  virtual std::string getTypeDescription() = 0;
  
  virtual auto getTypeSample() = 0;
  
};

template <class T> class valueDeclarationImplementation
{
public:
  
  auto getTypeSample()
  {
    T t;
    return t;
  }
  
};

template <class T> class valueDeclaration
{
  bool verify(T value)
  {
    BOOST_THROW_EXCEPTION( exception() << errorName("no_value_verification_implemented") << errorTitle("The provided data could not verified. No suitable function has been implemented at compile-time.") << errorSeverity(severity::ShouldNot) );
    return false;
  }
  
};

template <class T> class exposedValueImplimentation : public exposedValueInterface, public exposedObject, public valueDeclaration<T>
{
protected:
  
  T* value;
  
public:

    
 exposedValueImplimentation()
 {
   this->value = new T;
 }
 
  ~exposedValueImplimentation()
  {
    if(value)
    {
      delete value;
    }
  }
     
  virtual void setValue(T value)
  {
    if(this->verify(value))
    {
      this->value = new T(value);
    }
    else
   BOOST_THROW_EXCEPTION( exception() << errorName(std::string("invalid_value_supplied_type_")+std::string(typeid(T).name())) << errorTitle("The provided data could not be transformed in a veritable value.") << errorSeverity(severity::Informative) );
  }
  
  virtual T getValue() const
  {
    return *this->value;
  }
  
//   template <class E> E getValue()
//   {
//     return (E) this->getValue();
//   }
//   
//   template <class E> void setValue(E e)
//   {
//     this->setValue((T) e);
//   }
  
};

template <class T> class exposedValue : public exposedValueImplimentation<T>
{
public:
    
  exposedValue<T>()
  {
  }
  
  template <class E> exposedValue<T>(exposedValue<E> &e)
  {
    this->value = transformation<T>(e.getValue());
  }
  
  exposedValue<T>(T value)
  {
    this->setValue(value);
  }
  
  exposedValue<T>(T value, std::string name, std::string description = "")
    {
      this->value = value;
      this->valueIsSet = true;
    }
    
    exposedValue<T>(std::string name, std::string description = "")
    {
      this->valueIsSet = false;
    }
};

template <class T> class exposedPtr : public exposedValue<T> 
{
public:
  
    exposedPtr(T* ptr, std::string name, std::string description = "")
    {
      this->value = ptr;
    }
  
  void setPtr(T* ptr)
  {
    this->value = ptr;
  }
  
  T* getPtr()
  {
    return this->value;
  }
  
};

REGISTER_CLASS_FAMILY(valueDeclaration)

REGISTER_CLASS_FAMILY(exposedValue)

} //namespace lughos
#endif