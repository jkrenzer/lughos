#ifndef GUI_FORMS_HPP
#define GUI_FORMS_HPP
#include "basicObject.hpp"
#include "io.hpp"
#include "basicIo.hpp"
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
  
  ioContext context;
  
  ioWrapperInterface wrapper;
  
  template <class C, class T> void renderTemplate()
  {
    
  }
  
public:
  
  formNode(basicObject* obj)
  {
    this->setName(obj->getName());
  }
  
  template <class C, class T> void getWrapper()
  {
    
  }
  
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