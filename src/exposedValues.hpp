#ifndef EXPOSED_VALUES_HPP
#define EXPOSED_VALUES_HPP

#include "BasicObject.hpp"
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
      sharedLock lock(this->mutex);
      return this->description;
    }
    
    virtual std::string showStructure() = 0;
    
    virtual std::string parse(std::string command) = 0;
    

    
  protected:
    
    std::string description;
    
  };

  class ExposerRegistry : public ThreadSaveObject
{
public:
  
  typedef boost::shared_ptr<ExposedObject> Object;
  
protected:
  
  typedef std::map<std::string,Object> ExposedObjects;
  
  ExposedObjects exposedObjects;

public:
  
  void addObject(Object object);
  
  void addObject(ExposedObject* object);
  
  template <typename ... Arguments>
  void addObjects(ExposedObject* object, Arguments ... Rest)
  {
    this->addObject(object);
    this->addObjects(Rest...);
  }

  template <typename ... Arguments>
  void addObjects(boost::shared_ptr< ExposedObject > object, Arguments ... Rest)
  {
    this->addObject(object);
    this->addObjects(Rest...);
  }
  
  void addObjects()
  {
    //Must be empty for the recursion to work.
  }

  Object getObject(int i);
  
  Object getObject(std::string name);
  
  void deleteObject(std::string name);
  
  void deleteObject(ExposedObject& object);
  
  void deleteObject(Object object);
  
  Object operator[](std::string name) ;
  
  Object operator[](int i);
  
  std::string show();
};
  
template <class T> class ExposedValue : public ExposedObject, public Value<T>
  {
  protected:
    
  public:
    
    boost::signals2::signal<bool (T&), allSlotsTrue> beforeValueChange;
    boost::signals2::signal<void (T&)> onValueChange;
    
    ExposedValue(T& value, std::string name, std::string description = std::string("N/A")) : Value<T>(value)
    {
      this->name = name;
      this->description = description;
    }
    
    std::string showStructure()
    {
      std::stringstream ss;
      ExclusiveLock lock(this->mutex);
      ss << "ExposedValue: " << this->getName() << " of type " << this->getTypeName();
      lock.unlock();
      return ss.str();
    }
    
    ExposedValue<T>& operator=(T other)
    {
      this->setValue(other);
      return *this;
    }
    
    bool setValue(T& other)
    {
      if(beforeValueChange(other))
      {
	Value<T>::setValue(other);
	onValueChange(other);
	return true;
      }
      return false;
    }
    
    bool setValueFromString(std::string& str)
    {
      T t = transformTo<T>::from(str);
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
      sharedLock lock(this->mutex);
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