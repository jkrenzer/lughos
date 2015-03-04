#ifndef VALUES_HPP
#define VALUES_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <typeinfo>       // operator typeid
#include <typeindex>
#include "errorHandling.hpp"
#include "transformations.hpp"
#include "threadSafety.hpp"
#include <boost/smart_ptr/shared_ptr.hpp>

#define REGISTER_CLASS_FAMILY(name) template <class T> name<T>& get ## name (name<T> &d) { return d; }

namespace lughos
{

class data
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
  
  virtual bool setValueFromString(std::string str) = 0;
   
};

class TypeDeclarationInterface
{
public:
  virtual bool verify() = 0;
  
  virtual std::string getTypeName() = 0;
  
  virtual std::string getTypeShortDescription() = 0;
        
  virtual std::string getTypeDescription() = 0;

};

// template <class T> class ValueDeclarationImplementation
// {
// public:
//   
// };

template <class T> class TypeDeclaration : public TypeDeclarationInterface
{
public:
  bool verify(T value)
  {
//     BOOST_THROW_EXCEPTION( exception() << errorName("no_value_verification_implemented") << errorTitle("The provided data could not verified. No suitable function has been implemented at compile-time.") << errorSeverity(severity::ShouldNot) );
    return true;
  }
  
};

template <class T> TypeDeclaration<T> getTypeDeclaration(T t)
{
  return TypeDeclaration<T>();
}

template <class T> class ValueImplementation : public ValueInterface, public TypeDeclaration<T>, public ThreadSaveObject
{
protected:
  
  boost::shared_ptr<T> valuePointer;

public:
    
 ValueImplementation()
 {
 }
 
  ~ValueImplementation()
  {
  }
     
  virtual bool setValue(T value)
  {
    if(this->verify((T) value))
    {
      ExclusiveLock lock(this->mutex);
      this->valuePointer.reset( new T(value));
      lock.unlock();
      return true;
    }
    else
      return false;
  }
  
  virtual T getValue() const
  {
    SharedLock lock(this->mutex);
    return *this->valuePointer;
  }
  
  virtual std::string getValueAsString()
  {
    return transformTo<std::string>::from(this->getValue());
  }
  
  virtual bool setValueFromString(std::string string)
  {
    return this->setValue(transformTo<T>::from(string));
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
  
  Value<T>(T& value)
  {
    this->setValue(value);
  }
  
  Value<T>& operator=(const T &other)
  {
    this->setValue(other);
    return *this;
  }	
    
  Value<T>& operator=(const Value<T> &other)
  {
    this->setValue(other.getValue());
    return *this;
  }
  
  operator T() const
  {
    return this->getValue();
  }
  
};

template <class T> class Pointer : public Value<T> 
{
public:
  
  Pointer(T* ptr)
  {
    this->setPtr(ptr);
  }
  
  void setPtr(T* ptr)
  {
    exclusiveLock lock(this->mutex);
    this->valuePointer.reset(ptr);
  }
  
  T* getPtr()
  {
    sharedLock lock(this->mutex);
    return this->valuePointer.get();
  }
  
};

typedef ValueInterface Values;

REGISTER_CLASS_FAMILY(TypeDeclaration)

REGISTER_CLASS_FAMILY(Value)

} //namespace lughos
#endif