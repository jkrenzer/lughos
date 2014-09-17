#ifndef GUI_FORMS_HPP
#define GUI_FORMS_HPP
#include "lughosObjects.hpp"
#include "io.hpp"
#include "exposedValues.hpp"
#include "treeObj.hpp"

namespace lughos
{

class wtContext
{
};

class formNode : public treeChild<formNode>, public treeParent<formNode>
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

class textNode : public formNode
{
  virtual void render()
  {
  }
};



}//namespace lughos
#endif