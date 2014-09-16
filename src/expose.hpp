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

namespace exposer
{

class exposedObject
{
protected:
  std::string name;
  std::string description;
public:
  
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
  
template <class T> class exposedTreeObject
{
protected:
  
  T* parent;
  std::vector<T*> children;
  std::vector<std::string> childrenNames;
  
    
public:
  
  exposedTreeObject() : parent(NULL)
  {
  }
  
  T* getParent()
  {
    return this->parent;
  }
  
  void setParent(T* objectPtr = NULL)
  {
    if (this->parent != objectPtr && objectPtr != NULL)
    {
      this->parent = objectPtr;
      objectPtr->addChild(this);
    }
    else if (this->parent != objectPtr && objectPtr == NULL)
    {
      this->parent = NULL;
      objectPtr->removeChild(this);
    }
  }
  
  void addChild(T* objectPtr)
  {
    if(!isChild(objectPtr))
    {
      this->children.push_back(objectPtr);
      this->childrenNames.push_back(objectPtr->name);
      objectPtr->setParent(this);
    }
  }
  
  void removeChild(T* objectPtr)
  {
    if(isChild(objectPtr))
    {
      this->children.erase(std::find(this->children.begin(), this->children.end(), objectPtr));
      this->childrenNames.erase(std::find(this->childrenNames.begin(), this->childrenNames.end(), objectPtr->name));
      objectPtr->setParent();
      
    }
  }
  
  bool isChild(std::string name)
  {
    return getChild(name) != NULL;
  }
  
  bool isChild(T* objectPtr)
  {
    return getChildName(objectPtr) != std::string("");
  }
  
  T* getChild(std::string name)
  {
    typename std::vector<std::string>::iterator it = std::find(this->childrenNames.begin(), this->childrenNames.end(), name);
    if (it != this->childrenNames.end())
      return this->children[it-this->childrenNames.begin()];
    else
      return NULL;
  }
  
  std::string getChildName(T* objectPtr)
  {
    typename std::vector<T*>::iterator it = std::find(this->children.begin(), this->children.end(), objectPtr);
    if (it != this->children.end())
      return this->childrenNames[it-this->children.begin()];
    else
      return std::string("");
  }
  
  std::vector<std::string> path()
  {
    std::vector<std::string> path;
    if (this->parent != NULL)
      path = this->parent->path();
    path.push_back(this->name);
      
  }
  
};

class exposedTypeImpl
{
  
public:
  
    exposedTypeImpl() 
    {
      
    }
        
    virtual std::string getGlobalTypeName() = 0;
        
    virtual std::string getTypeDescription() = 0;
        
    virtual std::type_index getLocalTypeInfo() = 0;
  
};

template <class T> class exposedTypeTemplate : public exposedTypeImpl
{
public:
  
  typedef T type;
  
  std::type_index getLocalTypeInfo()
  {
    return std::type_index(typeid(T));
  }
  
};

template <class T> class exposedType : public exposedTypeTemplate<T>, public exposedTypeImpl
{
};

class exposedValueImpl
{
protected:
  bool valueIsSet;
public:
  
  exposedValueImpl() : valueIsSet(false)
  {
  }
  
   bool isSet()
  {
    return this->valueIsSet;
  }
  
};

template <class T> class exposedValueTemplate : public exposedValueImpl
{
protected:
  T value;
  
public:
     
  virtual void setValue(T value)
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
  
  virtual T getValue()
  {
    return this->value;
  }
  
  virtual bool verify(T value) = 0;
  
  bool verify()
  {
    return this->verify(this->value);
  }
  
  virtual T stringToType(std::string) = 0;
   
};

template <class T> class exposedValue : public exposedValueTemplate<T>, public exposedValueImpl
{
};

template <class T> class exposedVar : public exposedValue<T>, public exposedType<T>
{
public:
    exposedVar(T value, std::string name, std::string description = "")
    {
      this->value = value;
      this->valueIsSet = true;
    }
    
    exposedVar(std::string name, std::string description = "")
    {
      this->valueIsSet = false;
    }

};

template <class T> class exposedPtr : public exposedValue<T>, public exposedType<T>
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

template <class T> class exposedArgument : public exposedType<T>
{
protected:
  T defaultValue;
  bool hasDefaultValue;
public:
 
  exposedArgument(T value, T defaultValue, std::string name, std::string description = "") : exposedVar<T>(value,name,description)
    {
      this->defaultValue = defaultValue;
      this->hasDefaultValue = true;
    }
  
  void setDefaultValue(T value)
  {
    this->defaultValue = value;
    T t;
    if(value == t)
      this->defaultValue = false;
  }
  
  T getDefaultValue()
  {
    return this->defaultValue;
  }  
  
};

template <class R> class exposedFunction : public exposedObject, public exposedTreeObject<exposedValueImpl>
{

public:
  
  template <class T> T getValue(exposedValueImpl* childPtr)
  {
    exposedValue<T>* child = (exposedValue<T>*) childPtr;
    return child->getValue();
  }
  
  template <class T> T getValue(std::string childName)
  {
    exposedValue<T>* child = (exposedValue<T>*) this->getChild(childName);
    return child->getValue();
  }
  
  virtual R exec() = 0;
  
  virtual bool runable() = 0;
  
  R run()
  {
    if(this->runable())
      return this->exec();
    else
      BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_not_runnable") << errorTitle("An exposed function was called but it is not executable") << errorDescription("This error occurs, when a exposed function is called but runable-state-verification-method reports an inexecutable state of the function object. This can be the case when arguments are missing or of wrong type." << errorSeverity(severity::ShouldNot)) );
  }
  
};

} //namespace exposer
#endif