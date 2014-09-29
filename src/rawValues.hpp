#ifndef RAW_VALUES_HPP
#define RAW_VALUES_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <typeinfo>       // operator typeid
#include <typeindex>
#include "errorHandling.hpp"
#include "transformations.hpp"
#include "treeObj.hpp"

#define REGISTER_CLASS_FAMILY(name) template <class T> name<T>& get ## name (name<T> &d) { return d; }

namespace lughos
{

class data : public TreeNode
{
  
};
  
class ValueInterface : public data
{
protected:
public:
  
  ValueInterface()
  {
  }
  
  virtual ~ValueInterface()
  {
    
  }
   
  virtual std::string getValueAsString() = 0;
  
  virtual void setValueFromString(std::string string) = 0;
   
};

class ValueDeclarationInterface
{
public:
  virtual bool verify() = 0;
  
  virtual std::string getTypeName() = 0;
  
  virtual std::string getTypeShortDescription() = 0;
        
  virtual std::string getTypeDescription() = 0;

};

template <class T> class ValueDeclarationImplementation
{
public:
  
};

template <class T> class ValueDeclaration
{
public:
  bool verify(T value)
  {
//     BOOST_THROW_EXCEPTION( exception() << errorName("no_value_verification_implemented") << errorTitle("The provided data could not verified. No suitable function has been implemented at compile-time.") << errorSeverity(severity::ShouldNot) );
    return true;
  }
  
};

template <class T> class ValueImplementation : public ValueInterface, public ValueDeclaration<T>
{
protected:
  
  T* value;
  bool isOwner;

//TODO Add callback-functionality on change

  void clearOldValue()
  {
    if(this->value != NULL && isOwner)
    {
      delete this->value;
    }
  }
  
public:
    
 ValueImplementation()
 {
   this->value = NULL;
   this->isOwner = true;
 }
 
  ~ValueImplementation()
  {
    this->clearOldValue();
  }
     
  virtual void setValue(T value)
  {
    if(this->verify((T) value))
    {
      this->clearOldValue();
      this->value = new T(value);
      this->isOwner = true;
    }
    else
   BOOST_THROW_EXCEPTION( exception() << errorName(std::string("invalid_value_supplied_type_")+std::string(typeid(T).name())) << errorTitle("The provided data could not be transformed in a veritable value.") << errorSeverity(severity::Informative) );
  }
  
  virtual T getValue() const
  {
    return *this->value;
  }
  
  std::string getValueAsString()
  {
    return transformTo<std::string>::from(this->getValue());
  }
  
  void setValueFromString(std::string string)
  {
    this->setValue(transformTo<T>::from(string));
  }
  
};

template <class T> class Value : public ValueImplementation<T>
{
protected:
  
public:
  
  typedef T type;
    
  Value<T>()
  {
  }
  
  template <class E> Value<T>(Value<E> &e)
  {
    this->setValue(transformTo<T>::from(e.getValue()));
  }
  
  Value<T>(T value)
  {
    this->setValue(value);
  }
  
  Value<T>(T value, std::string name)
  {
    this->setValue(value);
    this->setName(name);
  }
  


};

template <class T> class Pointer : public Value<T> 
{
public:
  
  Pointer(T* ptr)
  {
    this->setPtr(ptr);
    this->isOwner = false;
  }
  
  Pointer(T* ptr, std::string name)
  {
    this->setPtr(ptr);
    this->setName(name);
    this->isOwner = false;
  }
  
  void setPtr(T* ptr, bool isOwner = false)
  {
    this->value = ptr;
    this->isOwner = isOwner;
  }
  
  T* getPtr()
  {
    return this->value;
  }
  
};

typedef ValueInterface Values;

REGISTER_CLASS_FAMILY(ValueDeclaration)

REGISTER_CLASS_FAMILY(Value)

} //namespace lughos
#endif