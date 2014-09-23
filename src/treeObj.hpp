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
  
  struct null_deleter
{
    void operator()(void const *) const
    {
    }
};

class TreeNode : public basicObject, public boost::enable_shared_from_this<TreeNode>
{
protected:
    boost::shared_ptr<TreeNode> _this;
    boost::shared_ptr<TreeNode> parent;
    
    std::vector<boost::shared_ptr<TreeNode> > children;
    std::vector<std::string> childrenNames;
  
  void _setParent(boost::shared_ptr<TreeNode> objectPtr = NULL);
  
  void _unsetParent(boost::shared_ptr<TreeNode> objectPtr = NULL);
    
    void _setChild(boost::shared_ptr<TreeNode> objectPtr = NULL);
    
    void _unsetChild(boost::shared_ptr<TreeNode> objectPtr = NULL);
    
public:
  
  template <class T> boost::shared_ptr<T> getParent();
  
  void setParent(boost::shared_ptr<TreeNode> objectPtr = NULL, bool callback = true);
  
    TreeNode() : parent(NULL), _this(this, null_deleter())
    {
    }
  
  virtual ~TreeNode()
  {
    std::cout << "Destructor: " << this->getName() << std::endl;
  }
  
  int countChildren();
  
  std::vector<boost::shared_ptr<TreeNode> > getChildren();
  
  void addChild(boost::shared_ptr<TreeNode> objectPtr);
  
  void addChild(TreeNode* objectPtr)
  {
    this->addChild(boost::shared_ptr<TreeNode>(objectPtr));
  }
  
  void removeChild(boost::shared_ptr<TreeNode> objectPtr,bool callback = true);
  
  void removeChild(TreeNode* objectPtr,bool callback = true)
  {
    this->removeChild(boost::shared_ptr<TreeNode>(objectPtr),callback);
  }
  
  bool isChild(std::string name);
  
  bool isChild(boost::shared_ptr<TreeNode> objectPtr);
  
  bool isChild(TreeNode* objectPtr)
  {
    return this->isChild(boost::shared_ptr<TreeNode>(objectPtr));
  }
  
 
  boost::shared_ptr<TreeNode> get(std::string name)
  {
    std::vector<std::string>::iterator it = std::find(this->childrenNames.begin(), this->childrenNames.end(), name);
    if (it != this->childrenNames.end())
      return this->children[it-this->childrenNames.begin()];
    else
      return NULL;
  }
  
  boost::shared_ptr<TreeNode> get(unsigned long int number)
  {
    if (number < this->children.size())
      return this->children[number];
    else
      return this->children[children.size()-1];
  }
  
  template <class T> boost::shared_ptr<T> getAs(std::string name);
  
  template <class T> boost::shared_ptr<T> getAs(unsigned long int number)
  {
    return boost::dynamic_pointer_cast<T>(this->get(number));
  }
  
  std::string getNameOf(boost::shared_ptr<TreeNode> objectPtr);
  
  std::string getNameOf(TreeNode* objectPtr)
  {
    return this->getNameOf(boost::shared_ptr<TreeNode>(objectPtr));
  }
  
  std::vector<std::string> path();
  
};

} // namespace lughos

#include "treeObj.cpp"

#endif