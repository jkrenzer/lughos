#ifndef GUI_FORMS_HPP
#define GUI_FORMS_HPP
#include "basicObject.hpp"
#include "io.hpp"
#include "exposedValues.hpp"
#include "treeObj.hpp"

namespace lughos
{

class wtContext
{
};

class formNode : public TreeNode
{
public:
  
  enum permissions { READ, READWRITE };
  
protected:
  
  permissions perm;
  
public:
  
  virtual void render() = 0;
  
  void setPermissions(permissions p)
  {
    this->perm = p;
  }
  
  permissions getPermissions()
  {
    return this->perm;
  }
  
};

template <class T> FormNode formCreator(ExposedObject object)
{
}

}//namespace lughos
#endif