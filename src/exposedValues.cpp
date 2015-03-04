#include "exposedValues.hpp"

namespace lughos {

  void ExposerRegistry::addObject(simulationObject* object)
{
  this->addObject(Object(object));
}

void ExposerRegistry::addObject(Object object)
{
  std::string name = object->getName();
  this->exposedObjects.insert(std::pair<int,Object >(name,object));
}

void ExposerRegistry::deleteObject(Object object)
{
  this->exposedObjects.erase(object->getName());
}

void ExposerRegistry::deleteObject(simulationObject& object)
{
  this->exposedObjects.erase(object.getName());
}

void ExposerRegistry::deleteObject(std::string name)
{
  this->exposedObjects.erase(name);
}

std::string ExposerRegistry::show()
{
  std::stringstream ss;
  ss << "Map of " << this->exposedObjects.size() << " objects:" << std::endl
     << "-----------------------------------" << std::endl;
  for(ExposedObjects::iterator it = this->exposedObjects.begin(); it != this->exposedObjects.end(); it++)
  {
    ss << it->second->showStructure() << std::endl;
  }
  ss << "-----------------------------------";
  return ss.str();
}

ExposerRegistry::Object& ExposerRegistry::getObject(std::string name)
{
    return this->exposedObjects[name];
}

ExposerRegistry::Object& ExposerRegistry::getObject(int i)
{
    return this->exposedObjects[i];
}
  
ExposerRegistry::Object& ExposerRegistry::operator[](int i)
{
  return this->getObject(i);
}

ExposerRegistry::Object& ExposerRegistry::operator[](std::string name)
{
  return this->getObject(name);
}


}
