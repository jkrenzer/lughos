#ifndef TREE_OBJ_HPP
#define TREE_OBJ_HPP

#include "basicObject.hpp"
#include "errorHandling.hpp"

//TODO How should we proceed with our trees? --> wrong renderer-invocation of children. Ord should renderers even be object-specific? but how should this go into modules?
// Will wrapping the children help?

namespace lughos
{

class treeNode : public basicObject
{
protected:
    treeNode* parent;
    
    std::vector<treeNode*> children;
  std::vector<std::string> childrenNames;
  
  void _setParent(treeNode* objectPtr = NULL);
  
  void _unsetParent(treeNode* objectPtr = NULL);
    
    void _setChild(treeNode* objectPtr = NULL);
    
    void _unsetChild(treeNode* objectPtr = NULL);
    
public:
  
  template <class T> T* getParent();
  
  void setParent(treeNode* objectPtr = NULL, bool callback = true);
  
    treeNode()
    {
      this->parent = NULL;
    }
  
  virtual ~treeNode()
  {
    if (this->parent != NULL)
      this->parent->removeChild(this,true);
    for(std::vector<treeNode*>::iterator i = this->children.begin(); i < this->children.end(); i++)
    {
      (*i)->setParent(NULL,false);
    }
  }
  
  int countChildren();
  
  std::vector<treeNode*> getChildren();
  
  void addChild(treeNode* objectPtr);
  
//   template <class V> void addChild(V* objectPtr)
//   {
//     this->addChild<T>(new T(objectPtr));
//   }
  
  
  
  void removeChild(treeNode* objectPtr,bool callback = true);
  
  bool isChild(std::string name);
  
  bool isChild(treeNode* objectPtr);
  
  treeNode* get(std::string name)
  {
    std::vector<std::string>::iterator it = std::find(this->childrenNames.begin(), this->childrenNames.end(), name);
    if (it != this->childrenNames.end())
      return this->children[it-this->childrenNames.begin()];
    else
      return NULL;
  }
  
  treeNode* get(unsigned long int number)
  {
    if (number < this->children.size())
      return this->children[number];
    else
      return this->children[children.size()-1];
  }
  
  template <class T> T* getAs(std::string name);
  
  template <class T> T* getAs(unsigned long int number)
  {
    return dynamic_cast<T*>(this->get(number));
  }
  
  std::string getNameOf(treeNode* objectPtr);
  
  std::vector<std::string> path();
  
};

} // namespace lughos

#include "treeObj.cpp"

#endif