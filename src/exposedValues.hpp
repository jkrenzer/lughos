#ifndef EXPOSED_VALUES_HPP
#define EXPOSED_VALUES_HPP

#include "log.hpp"
#include "BasicObject.hpp"
#include "BasicParsers.hpp"
#include "threadSafety.hpp"
#include "values.hpp"
#include <sstream>
#include <boost/signals2/signal.hpp>

namespace lughos
{
  struct allSlotsTrue
  {
    typedef bool result_type;
    
    template<typename InputIterator>
    bool operator()(InputIterator first, InputIterator last) const
    {
      // If there are no slots to call, just return the
      // default-constructed value
      if(first == last ) return true;
      while (first != last) {
	if (!*first)
	{
	  return false;
	}
	++first;
      }
      return true;
    }
  };
  
  class ExposedObject : public BasicObject
  {
  private:
    Mutex mutex;
  public:
    ExposedObject()
    {
    }
    
    void setDescription(std::string description)
    {
      ExclusiveLock lock(this->mutex);
      this->description = description;
    }
    
    std::string getDescription()
    {
      SharedLock lock(this->mutex);
      return this->description;
    }
    
    virtual std::string showStructure()
    {
      return std::string("");
    }
    
    virtual std::string parse(std::string command)
    {
      return std::string("");
    }
    

    
  protected:
    
    std::string description;
    
  };

  template <class T>
  class ExposerRegistryTemplate
{
private:
  Mutex mutex;
public:
  
  typedef T Object;
  
protected:
  
  typedef std::map<std::string,Object*> ExposedObjects;
  
  ExposedObjects exposedObjects;

public:
  
  void addObject(Object& object);
  
  void addObject(Object* object);
  
  template <typename ... Arguments>
  void addObjects(Object* object, Arguments ... Rest)
  {
    this->addObject(object);
    this->addObjects(Rest...);
  }

  template <typename ... Arguments>
  void addObjects(Object& object, Arguments ... Rest)
  {
    this->addObject(object);
    this->addObjects(Rest...);
  }
  
  void addObjects()
  {
    //Must be empty for the recursion to work.
  }

  Object* getObject(int i);
  
  Object* getObject(std::string name);
  
  void deleteObject(std::string name);
  
  void deleteObject(Object* object);
  
  void deleteObject(Object& object);
  
  Object* operator[](std::string name) ;
  
  Object* operator[](int i);
  
  std::string showStructure();
};

typedef ExposerRegistryTemplate<ExposedObject> ExposerRegistry;

class ExposedValueInterface :  public ExposedObject
{
public:
  
  boost::signals2::signal<void ()> beforeValueChange;
  boost::signals2::signal<void ()> onValueChange;
  boost::signals2::signal<void ()> beforeReadValue;
  
  virtual std::string showStructure() = 0;
  virtual bool setValueFromString(std::string& str) = 0;
  virtual std::string getValueAsString() = 0;
  virtual std::string parse(std::string command) = 0;
};

template <class T> class ExposedValueTemplate : public ExposedValueInterface
{
public:
  virtual bool setValue(T newValue) = 0;
  virtual operator T() = 0;
};

template <class T> class ExposedValue : public ExposedValueTemplate<T>, virtual public Value<T>
  {
  private:
    Mutex mutex;
   
  public:

    ExposedValue(T& value, std::string name, std::string description = std::string("N/A")) : Value<T>(value)
    {
      this->name = name;
      this->description = description;
    }
    
    ExposedValue(std::string name) : Value<T>()
    {
      this->name = name;
      this->description = std::string("N/A");
    }
    
    std::string showStructure()
    {
      std::stringstream ss;
      ExclusiveLock lock(this->mutex);
      ss << "ExposedValue: " << this->getName() << " of type " << this->type.getName();
      lock.unlock();
      return ss.str();
    }
    
    ExposedValue<T>& operator=(T other)
    {
      this->setValue(other);
      return *this;
    }
    
    T getValue()
    {
      SharedLock lock(this->mutex);
      this->beforeReadValue();
      if(this->valuePointer)
        return *this->valuePointer;
      else
        BOOST_THROW_EXCEPTION(exception() << errorName("value_accessed_but_no_value_set") << errorSeverity(severity::ShouldNot));
    }
    
    bool setValue(T newValue)
    {
      try
      {
	this->beforeValueChange();
      }
      catch(std::exception& e)
      {
// 	lughos::debuglog(std::string("beforeValueChange-function threw exception."));
      }
      try
      {
	Value<T>::setValue(newValue);
	this->onValueChange();
	return true;
      }
      catch(std::exception& e)
      {
// 	lughos::debuglog(std::string("Setting of value threw exception."));
      }
      
      return false;
    }
    
    bool setValueFromString(std::string& str)
    {
      T t = this->type.fromString(str);
      return this->setValue(t);
    }
    
    std::string getValueAsString()
    {
      return this->type.toString(this->getValue());
    }
    
    operator T()
    {
      return this->getValue();
    }
    
    operator T*()
    {
      return this->getPtr();
    }
    
    virtual std::string parse(std::string command)
    {
      bool success = false;
      if(command[0] == '=' && command.size() > 1)
      {
	try 
	{
	  success = this->setValueFromString(command);
	}
	catch(...)
	{
	  success = false;
	}
	if(success)
	  return std::string("OK");
	else
	  return std::string("ERROR");
      }
      else if(command[0] == '?')
      {
	return this->getValueAsString();
      }
    }
    
  }; 
  
  template <class T> class ExposedPointer : public ExposedValue<T>, public Pointer<T>
  {
  protected:

    boost::signals2::signal<void (ExposedPointer<T>&)> beforePointerChange;
    boost::signals2::signal<void (ExposedPointer<T>&)> onPointerChange;

  public:
    ExposedPointer(T* pointer, std::string name, std::string description = std::string("N/A"))
    {
      this->setPointer(pointer);
      this->name = name;
      this->description = description;
    }
    
    
    std::string showStructure()
    {
      std::stringstream ss;
      SharedLock lock(this->mutex);
      ss << "ExposedPointer: " << this->getName() << " of type " << this->getTypeName();
      lock.unlock();
      return ss;
    }
    
    ExposedPointer<T>& operator=(T* other)
    {
      beforePointerChange(other);
      this->setPointer(other);
      onPointerChange(other);
      return *this;
    }
    
  };

/*
 * Definition in same file as needed by C++-standard
 */
 template <class T>
  void ExposerRegistryTemplate<T>::addObject(Object& object)
  {
    this->addObject(&object);
    std::cout << "Got reference. Redirecting..." << std::endl;
  }

  template <class T>
  void ExposerRegistryTemplate<T>::addObject(Object* object)
  {
    std::string name = object->getName();
    ExclusiveLock lock(this->mutex);
    this->exposedObjects.insert(std::pair<std::string,Object*>(name,object));
    std::cout << "Added object " << name << " with address " << object << std::endl;
  }

  template <class T>
  void ExposerRegistryTemplate<T>::deleteObject(Object& object)
  {
    ExclusiveLock lock(this->mutex);
    this->exposedObjects.erase(object.getName());
  }

  template <class T>
  void ExposerRegistryTemplate<T>::deleteObject(Object* object)
  {
    ExclusiveLock lock(this->mutex);
    this->exposedObjects.erase(object->getName());
  }

  template <class T>
  void ExposerRegistryTemplate<T>::deleteObject(std::string name)
  {
    ExclusiveLock lock(this->mutex);
    this->exposedObjects.erase(name);
  }

  template <class T>
  std::string ExposerRegistryTemplate<T>::showStructure()
  {
    std::stringstream ss;
    SharedLock(this->mutex);
    ss << "Map of " << this->exposedObjects.size() << " objects:" << std::endl
      << "-----------------------------------" << std::endl;
    for(typename ExposedObjects::iterator it = this->exposedObjects.begin(); it != this->exposedObjects.end(); it++)
    {
      ss << it->second->showStructure() << std::endl;
    }
    ss << "-----------------------------------";
    return ss.str();
  }

  template <class T>
  typename ExposerRegistryTemplate<T>::Object* ExposerRegistryTemplate<T>::getObject(std::string name)
  {
    SharedLock lock(this->mutex);
    typename ExposedObjects::iterator it = exposedObjects.find(name);
    if(it != exposedObjects.end())
      return it->second;
    else
      throw std::runtime_error("Tried to access non-existant object by name");
  }

  template <class T>
  typename ExposerRegistryTemplate<T>::Object* ExposerRegistryTemplate<T>::getObject(int i)
  {
    SharedLock lock(this->mutex);
    if(i < exposedObjects.size() && i >= 0)
    {
      typename ExposedObjects::iterator it = exposedObjects.begin();
      std::advance(it,i);
      return it->second;
    }
    else if (i >= exposedObjects.size())
      return exposedObjects.end()->second;
    else
      return exposedObjects.begin()->second;
    
  }

  template <class T>
  typename ExposerRegistryTemplate<T>::Object* ExposerRegistryTemplate<T>::operator[](int i)
  {
    
    return this->getObject(i);
  }

  template <class T>
  typename ExposerRegistryTemplate<T>::Object* ExposerRegistryTemplate<T>::operator[](std::string name)
  {
    return this->getObject(name);
  }
}//namespace lughos
#endif