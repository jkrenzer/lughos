#ifndef EXPOSED_VALUES_HPP
#define EXPOSED_VALUES_HPP

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

  class ExposerRegistry
{
private:
  Mutex mutex;
public:
  
  typedef ExposedObject Object;
  
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

class ExposedValueInterface :  public ExposedObject
{
public:
   
  virtual std::string showStructure() = 0;
  virtual bool setValueFromString(std::string& str) = 0;
  virtual std::string parse(std::string command) = 0;
};

template <class T> class ExposedValueTemplate : public ExposedValueInterface
{
public:
  boost::signals2::signal<bool (T&), allSlotsTrue> beforeValueChange;
  boost::signals2::signal<void (T&)> onValueChange;
  boost::signals2::signal<void (T&)> onRequestValue;
  
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
    
    bool setValue(T newValue)
    {
      if(this->beforeValueChange(newValue))
      {
	Value<T>::setValue(newValue);
	this->onValueChange(newValue);
	return true;
      }
      return false;
    }
    
    bool setValueFromString(std::string& str)
    {
      T t = this->type.fromString(str);
      return this->setValue(t);
    }
    
    operator T()
    {
      return this->getValue();
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

    boost::signals2::signal<bool (T*), allSlotsTrue> beforePointerChange;
    boost::signals2::signal<void (T*)> onPointerChange;

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
      if(beforePointerChange(other))
      {
	this->setPointer(other);
	onPointerChange(other);
      }
      return *this;
    }
    
  };
}//namespace lughos
#endif