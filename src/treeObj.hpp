#ifndef TREE_OBJ_HPP
#define TREE_OBJ_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "basicObject.hpp"
#include "errorHandling.hpp"

//TODO How should we proceed with our trees? --> wrong renderer-invocation of children. Ord should renderers even be object-specific? but how should this go into modules?
// Will wrapping the children help?

namespace lughos
{

class TreeNode : public basicObject
{
protected:
    TreeNode* parent;
    
    std::vector<TreeNode*> children;
    std::vector<std::string> childrenNames;
  
  void _setParent(TreeNode* objectPtr = NULL);
  
  void _unsetParent(TreeNode* objectPtr = NULL);
    
    void _setChild(TreeNode* objectPtr = NULL);
    
    void _unsetChild(TreeNode* objectPtr = NULL);
    
public:
  
  template <class T> T* getParent();
  
  void setParent(TreeNode* objectPtr = NULL, bool callback = true);
  
    TreeNode() : parent(NULL)
    {
    }
  
  virtual ~TreeNode()
  {
    if (this->parent)
    {
      TreeNode* parent(this->parent);
      this->parent = NULL;
      parent->removeChild(this,false);
    }
    for(std::vector<TreeNode*>::iterator i = this->children.begin(); i < this->children.end(); i++)
    {
      delete *i;
    }
  }
  
  int countChildren();
  
  std::vector<TreeNode*> getChildren();
  
  void addChild(TreeNode* objectPtr);
  
  void removeChild(TreeNode* objectPtr,bool callback = true);
  
  bool isChild(std::string name);
  
  bool isChild(TreeNode* objectPtr);
  
 
  TreeNode* get(std::string name)
  {
    std::vector<std::string>::iterator it = std::find(this->childrenNames.begin(), this->childrenNames.end(), name);
    if (it != this->childrenNames.end())
      return this->children[it-this->childrenNames.begin()];
    else
      return NULL;
  }
  
  TreeNode* get(unsigned long int number)
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
  
  std::string getNameOf(TreeNode* objectPtr);
  
  std::vector<std::string> path();
  
};

} // namespace lughos

#include "treeObj.cpp"

#endif