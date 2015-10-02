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
public:
  
  ValueInterface()
  {
  }
  
  virtual ~ValueInterface()
  {
    
  }
   
  virtual std::string getValueAsString() = 0;
  
  virtual bool setValueFromString(std::string str) = 0;
  
  virtual bool isSet() const = 0;
  
  virtual void unset() = 0;
   
};

class TypeInterface
{
public:
  
  virtual std::string getName() = 0;
  
  virtual std::string getShortDescription() = 0;
        
  virtual std::string getDescription() = 0;

};

// template <class T> class ValueDeclarationImplementation
// {
// public:
//   
// };

template <class T, class C = void> class TypeImplementation : public TypeInterface
{
public:
  
  virtual bool verify(T value) = 0;

  
  virtual std::string toString(T t)
  {
    std::stringstream ss;
    ss << t;
    return ss.str();
  }
  
  virtual T fromString(std::string str)
  {
    std::stringstream ss(str);
    T t;
    ss >> t;
    return t;
  }
  
};

template <class T, class C = void> class Type : public TypeImplementation<T>
{
};

template <class T, class C = void> Type<T,C> getType(T t)
{
  return Type<T,C>();
}

template <class T> class ValueImplementation : public ValueInterface
{
private:
  Mutex mutex;
protected:
  
  boost::shared_ptr<T> valuePointer;
  
  virtual T* getPtr()
  {
    SharedLock lock(this->mutex);
    return this->valuePointer.get();
  }

public:
  
  Type<T> type;
    
 ValueImplementation() : mutex(), valuePointer(new T)
 {
 
 }
 
  ~ValueImplementation()
  {
  }
  
  ValueImplementation<T>(const ValueImplementation<T>& other) : ValueInterface(other), mutex(), valuePointer()
  {
    this->valuePointer = other.valuePointer;
  }
     
  virtual bool setValue(T value)
  {
    if(this->type.verify((T) value))
    {
      ExclusiveLock lock(this->mutex);
      this->valuePointer.reset( new T(value));
      lock.unlock();
      return true;
    }
    else
      return false;
  }
  
  virtual ValueImplementation<T>& operator=(T value)
  {
    this->setValue(value);
    return *this;
  }
  
  virtual T getValue()
  {
    SharedLock lock(this->mutex);
    if(this->valuePointer)
      return *this->valuePointer;
    else
      BOOST_THROW_EXCEPTION(exception() << errorName("value_accessed_but_no_value_set") << errorSeverity(severity::ShouldNot));
  }
  
  virtual std::string getValueAsString()
  {
    return type.toString(this->getValue());
  }
  
  virtual bool setValueFromString(std::string string)
  {
    return this->setValue(type.fromString(string));
  }
    
  void unset()
  {
    this->valuePointer.reset();
  }
  
  bool isSet() const
  {
    return (bool) this->valuePointer;
  }
  
};

template <class T> class Value : public ValueImplementation<T>
{
public:
    
  Value<T>() 
  {
  }
  
  template <class E> Value<T>(Value<E> &e)
  {
    this->setValue = (T) e.getValue();
  }
  
  Value<T>(T& value)
  {
    this->setValue(value);
  }
  
  Value<T>(const Value<T>& other) : ValueImplementation<T>(other)
  {
    
  }
  
  Value<T>& operator=(const T &other)
  {
    this->setValue(other);
    return *this;
  }	
    
  Value<T>& operator=(Value<T> &other)
  {
    if(other.isSet())
      this->setValue(other.getValue());
    else
      this->unset();
    return *this;
  }
  
  operator T()
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
  
  Pointer()
  {
    this->unsetPtr();
  }
  
  void setPtr(T* ptr)
  {
    ExclusiveLock lock(this->mutex);
    this->valuePointer.reset(ptr);
  }
  
  void unsetPtr()
  {
    this->valuePointer.reset();
  }
  
  T* getPtr()
  {
    SharedLock lock(this->mutex);
    return this->valuePointer.get();
  }
  
};

typedef ValueInterface Values;

REGISTER_CLASS_FAMILY(Type)

REGISTER_CLASS_FAMILY(Value)

} //namespace lughos
#endif