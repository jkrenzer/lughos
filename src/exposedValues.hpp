#ifndef EXPOSED_VALUES_HPP
#define EXPOSED_VALUES_HPP

#include "BasicObject.hpp"
#include "values.hpp"
#include <sstream>

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
    
    virtual std::string showStructure() = 0;
    
    virtual std::string interface(std::string) = 0;
    
  protected:
    
    std::string description;
    
  };

  class ExposerRegistry
{
protected:
  
  typedef std::map<std::string,boost::shared_ptr<ExposedObject>> ExposedObjects;
  
  ExposedObjects exposedObjects;

public:
  void addObject(boost::shared_ptr<ExposedObject> object);
  
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

  boost::shared_ptr<ExposedObject> getObject(int i);
  
  void deleteObject(std::string name);
  
  void deleteObject(ExposedObject& object);
  
  void deleteObject(boost::shared_ptr<ExposedObject> object);
  
  std::string show();
};
  
template <class T> class ExposedValue : public ExposedObject, public Value<T>
  {
  public:
    ExposedValue(T& value, std::string name, std::string description = std::string("N/A")) : Value<T>(value)
    {
      this->name = name;
      this->description = description;
    }
    
    std::string showStructure()
    {
      std::stringstream ss;
      ss << "ExposedValue: " << this->getName() << " of type " << this->getTypeName();
      return ss.str();
    }
  }; 
  
  template <class T> class ExposedPointer : public ExposedObject, public Pointer<T>
  {

  public:
    ExposedPointer(T* pointer, std::string name, std::string description = std::string("N/A"))
    {
      this->pointer = pointer;
      this->name = name;
      this->description = description;
    }
    
    
    std::string showStructure()
    {
      std::stringstream ss;
      ss << "ExposedPointer: " << this->getName() << " of type " << this->getTypeName();
      return ss;
    }
    
  };
}//namespace lughos
#endif