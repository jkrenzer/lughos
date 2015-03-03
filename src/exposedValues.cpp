#include "exposedValues.hpp"

namespace lughos {

  void ExposerRegistry::addObject(simulationObject* object)
{
  this->addObject(boost::shared_ptr< simulationObject >(object));
}

void ExposerRegistry::addObject(boost::shared_ptr< simulationObject > object)
{
  std::string name = object->getName();
  this->exposedObjects.insert(std::pair<int,boost::shared_ptr<simulationObject> >(name,object));
}

void ExposerRegistry::deleteObject(boost::shared_ptr< simulationObject > object)
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

boost::shared_ptr< simulationObject > ExposerRegistry::getObject(std::string name)
{
    return this->exposedObjects[name];
}
  
}
