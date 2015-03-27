#include "exposedValues.hpp"

namespace lughos {

  void ExposerRegistry::addObject(Object& object)
{
  this->addObject(&object);
  std::cout << "Got reference. Redirecting..." << std::endl;
}

void ExposerRegistry::addObject(Object* object)
{
  std::string name = object->getName();
  ExclusiveLock lock(this->mutex);
  this->exposedObjects.insert(std::pair<std::string,Object*>(name,object));
  std::cout << "Added object " << name << " with address " << object << std::endl;
}

void ExposerRegistry::deleteObject(Object& object)
{
  ExclusiveLock lock(this->mutex);
  this->exposedObjects.erase(object.getName());
}

void ExposerRegistry::deleteObject(Object* object)
{
  ExclusiveLock lock(this->mutex);
  this->exposedObjects.erase(object->getName());
}

void ExposerRegistry::deleteObject(std::string name)
{
  ExclusiveLock lock(this->mutex);
  this->exposedObjects.erase(name);
}

std::string ExposerRegistry::showStructure()
{
  std::stringstream ss;
  SharedLock(this->mutex);
  ss << "Map of " << this->exposedObjects.size() << " objects:" << std::endl
     << "-----------------------------------" << std::endl;
  for(ExposedObjects::iterator it = this->exposedObjects.begin(); it != this->exposedObjects.end(); it++)
  {
    ss << it->second->showStructure() << std::endl;
  }
  ss << "-----------------------------------";
  return ss.str();
}

ExposerRegistry::Object* ExposerRegistry::getObject(std::string name)
{
  SharedLock lock(this->mutex);
  ExposedObjects::iterator it = exposedObjects.find(name);
  if(it != exposedObjects.end())
    return it->second;
  else
    throw std::runtime_error("Tried to access non-existant object by name");
}

ExposerRegistry::Object* ExposerRegistry::getObject(int i)
{
  SharedLock lock(this->mutex);
  if(i < exposedObjects.size() && i >= 0)
  {
    ExposedObjects::iterator it = exposedObjects.begin();
    std::advance(it,i);
    return it->second;
  }
  else if (i >= exposedObjects.size())
    return exposedObjects.end()->second;
  else
    return exposedObjects.begin()->second;
  
}
  
ExposerRegistry::Object* ExposerRegistry::operator[](int i)
{
  
  return this->getObject(i);
}

ExposerRegistry::Object* ExposerRegistry::operator[](std::string name)
{
  return this->getObject(name);
}


}
