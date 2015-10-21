/********************************************************************************
 *   This file is part of LUGHOS.						*
 *										*
 *										*
 *  LUGHOS is free software: you can redistribute it and/or modify		*
 *  it under the terms of the GNU General Public License as published by	*
 *  the Free Software Foundation, either version 3 of the License, or		*
 *  (at your option) any later version.						*
 *										*
 *  LUGHOS is distributed in the hope that it will be useful,			*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of		*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*
 *  GNU General Public License for more details.				*
 *										*
 *  We sincerily invite everybody to participate in LUGHO'S development. 	*
 *  Together we can achieve more.						*
 *										*
 *  (c) 2014, 2015 Jörn Mankiewicz (joern.mankiewicz@gmx.de), 			*
 *                 Irina Brodski (irina@brodski.de)				*
 *										*
 ********************************************************************************/

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
  
  formNode(BasicObject* obj)
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