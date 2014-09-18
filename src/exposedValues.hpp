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

namespace lughos
{

class exposedObject
{
protected:
  std::string name;
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
    
  std::string getDescription()
  {
    return this->description;
  }
  
  void setDescription(std::string description)
  {
    this->description = description;
  }
  
};
  
class exposedTypeInterface
{
  
public:
  
    exposedTypeInterface() 
    {
      
    }
        
    virtual std::string getGlobalTypeName() = 0;
        
    virtual std::string getTypeDescription() = 0;
        
    virtual std::type_index getLocalTypeInfo() = 0;
  
};

template <class T> class exposedTypeImplementation : public exposedTypeInterface
{
public:
  
  std::type_index getLocalTypeInfo()
  {
    return std::type_index(typeid(T));
  }
  
  virtual bool verify(T value)
  {
    BOOST_THROW_EXCEPTION( exception() << errorName("no_value_verification_implemented") << errorTitle("The provided data could not verified. No suitable function has been implemented at compile-time.") << errorSeverity(severity::ShouldNot) );
  }
  
  virtual bool verify()
  {
    return this->verify(this->value);
  }
  
};

template <class T> class exposedType : public exposedTypeImplementation<T>
{
};

class exposedValueInterface
{
protected:
  bool valueIsSet;
public:
  
  exposedValueInterface() : valueIsSet(false)
  {
  }
  
   bool isSet()
  {
    return this->valueIsSet;
  }
  
};

template <class T> class exposedValueImplementation : public exposedValueInterface
{
protected:
  T value;
  
public:
  
  exposedValueImplementation<T>(T value, std::string name, std::string description = "")
    {
      this->value = value;
      this->valueIsSet = true;
    }
    
    exposedValueImplementation<T>(std::string name, std::string description = "")
    {
      this->valueIsSet = false;
    }
     
  void setValue(T value)
  {
    if(this->verify(value))
    {
      this->value = value;
      T t;
      if (value == t)
	this->valueIsSet = false;
      else
	this->valueIsSet = true;
    }
    else
   BOOST_THROW_EXCEPTION( exception() << errorName("invalid_value_supplied") << errorTitle("The provided data could not be transformed in a veritable value.") << errorSeverity(severity::Informative) );
  }
  
  T getValue() const
  {
    return this->value;
  }
     
};

template <class T> class exposedValue : public exposedValueImplementation<T>, public exposedType<T>
{
};


template <class T> class exposedPtr : public exposedValue<T> 
{
protected:
  T* ptr;
  
public:
  
    exposedPtr(T* ptr, std::string name, std::string description = "")
    {
      this->ptr = ptr;
    }
  
  void setValue(T value)
  {
    if(this->verify(value))
    {
      *this->ptr = value;
      T t;
      if (*this->ptr == t)
	this->valueIsSet = false;
      else
	this->valueIsSet = true;
    }
    else
      true;
    //TODO Exception
  }
  
  T getValue()
  {
    return *this->ptr;
  }
  
  void setPtr(T* ptr)
  {
    this->ptr = ptr;
  }
  
  T* getPtr()
  {
    return this->ptr;
  }
  
};

} //namespace lughos
#endif