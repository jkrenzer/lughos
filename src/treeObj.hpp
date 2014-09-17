#ifndef TREE_OBJ_HPP
#define TREE_OBJ_HPP

#include <map>

#include "exposedValues.hpp"
#include "errorHandling.hpp"

//TODO How should we proceed with our trees? --> wrong renderer-invocation of children. Ord should renderers even be object-specific? but how should this go into modules?
// Will wrapping the children help?

namespace lughos
{

template <class T> class treeChild : public exposedObject
{
protected:
    T* parent;
public:
  
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

};

template <class T> class treeParent
{
protected:
  std::vector<T*> children;
  std::vector<std::string> childrenNames;
public:
  int countChildren()
  {
    return this->children.size();
  }
  
  std::vector<T*> getChildren()
  {
    return std::vector<T*>(this->children);
  }
  
  void addChild(T* objectPtr)
  {
    if(!isChild(objectPtr) && objectPtr != NULL)
    {
      this->children.push_back(objectPtr);
      this->childrenNames.push_back(objectPtr->getName());
      objectPtr->setParent(this);
    }
  }
  
  template <class V> void addChild(V* objectPtr)
  {
    this->addChild<T>(new T(objectPtr));
  }
  
  
  
  void removeChild(T* objectPtr)
  {
    if(isChild(objectPtr))
    {
      this->children.erase(std::find(this->children.begin(), this->children.end(), objectPtr));
      this->childrenNames.erase(std::find(this->childrenNames.begin(), this->childrenNames.end(), objectPtr->getName()));
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



template <class T> class treeObject : public treeChild<treeObject<T> >, public treeParent<treeObject<T> >
{
protected:
  
  T* value;
      
public:
  
  treeObject(T& value)
  {
    this->value = value;
    this->parent = NULL;
  }
  
  treeObject()
  {
    this->parent = NULL;
  }
  
};


} // namespace lughos

#endif