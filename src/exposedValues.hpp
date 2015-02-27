#ifndef EXPOSED_VALUES_HPP
#define EXPOSED_VALUES_HPP

#include "BasicObject.hpp"
#include "values.hpp"

namespace lughos
{
  class ExposedObject : public BasicObject
  {
  public:
    ExposedObject()
    {
    }
    
    void setDescription(std::string description)
    {
      this->description = description;
    }
    
    std::string getDescription()
    {
      return this->description;
    }
    
  protected:
    
    std::string description;
    
  };

  
template <class T> class ExposedValue : public ExposedObject, public Value<T>
  {
  protected:
    T& value;
  public:
    ExposedValue(T& value, std::string name, std::string description = std::string("N/A")) : value(value)
    {
      this->name = name;
      this->description = description;
    }
    
    ExposedValue<T>& operator=(const T &other)
    {
      this->value = other;
      return *this;
    }	
    
    ExposedValue<T>& operator=(const ExposedValue<T> &other)
    {
      this->value = other.getValue();
      return *this;
    }
    
    T getValue()
    {
      return this->value;
    }
    
  }; 
  
  template <class T> class ExposedPointer : public ExposedObject, public Pointer<T>
  {
  protected:
    T* pointer;
  public:
    ExposedPointer(T* pointer, std::string name, std::string description = std::string("N/A"))
    {
      this->pointer = pointer;
      this->name = name;
      this->description = description;
    }
    
    
    ExposedPointer<T>& operator=(const T &other)
    {
      *this->pointer = other;
      return *this;
    }	
    
    ExposedPointer<T>& operator=(const T* &other)
    {
      this->pointer = other;
      return *this;
    }
    
    ExposedPointer<T>& operator=(const ExposedValue<T> &other)
    {
      this->pointer = other.getAddress();
      return *this;
    }
    
    T getAddress()
    {
      return this->pointer;
    }
  };
}//namespace lughos
#endif