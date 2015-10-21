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

#include "treeObj.hpp"


namespace lughos {
void TreeNode::_setChild(boost::shared_ptr<TreeNode> objectPtr)
{
    objectPtr->addChild(shared_from_this());
}

void TreeNode::_unsetChild(boost::shared_ptr<TreeNode> objectPtr)
{
    objectPtr->removeChild(shared_from_this());
}
template <class T> boost::shared_ptr<T> TreeNode::getParent()
{
    return boost::dynamic_pointer_cast<T>(this->parent);
}
void TreeNode::setParent(boost::shared_ptr<TreeNode> objectPtr,bool callback)
{
    if (this->parent != objectPtr && objectPtr != NULL)
    {
        this->parent = objectPtr;
	if(callback)
	  _setChild(objectPtr);
    }
}

void TreeNode::setParent(bool callback)
{
  if(callback)
    _unsetChild(this->parent);
  this->parent.reset();
}

 void TreeNode::_setParent(boost::shared_ptr<TreeNode> objectPtr)
{
    objectPtr->setParent(shared_from_this());
}
 void TreeNode::_unsetParent(boost::shared_ptr<TreeNode> objectPtr)
{
    objectPtr->setParent();
}
int TreeNode::countChildren()
{
    return this->children.size();
}
std::vector<boost::shared_ptr<TreeNode>> TreeNode::getChildren()
{
    return this->children;
}

void TreeNode::addChild(boost::shared_ptr<TreeNode> objectPtr)
{
    if(!this->isChild(objectPtr) && objectPtr != NULL)
    {
        this->children.push_back(objectPtr);
        this->childrenNames.push_back(objectPtr->getName());
        this->_setParent(objectPtr);
    }
}


void TreeNode::removeChild(boost::shared_ptr<TreeNode> objectPtr,bool callback)
{
    if(isChild(objectPtr))
    {
        this->children.erase(std::find(this->children.begin(), this->children.end(), objectPtr));
        this->childrenNames.erase(std::find(this->childrenNames.begin(), this->childrenNames.end(), objectPtr->getName()));
	if(callback)
	  objectPtr->setParent();

    }
}
bool TreeNode::isChild(std::string name)
{
    return std::find(this->childrenNames.begin(), this->childrenNames.end(), name) != this->childrenNames.end();
}
bool TreeNode::isChild(boost::shared_ptr<TreeNode> objectPtr)
{
    return std::find(this->children.begin(), this->children.end(), objectPtr) != this->children.end();
}
template <class T> boost::shared_ptr<T> TreeNode::getAs(std::string name)
{
  return boost::dynamic_pointer_cast<T>(this->get(name));
}
std::string TreeNode::getNameOf(boost::shared_ptr<TreeNode> objectPtr)
{
    std::vector<boost::shared_ptr<TreeNode>>::iterator it = std::find(this->children.begin(), this->children.end(), objectPtr);
    if (it != this->children.end())
        return this->childrenNames[it-this->children.begin()];
    else
        return std::string("");
}
std::vector<std::string> TreeNode::path()
{
    std::vector<std::string> path;
    if (this->parent != NULL)
        path = this->parent->path();
    path.push_back(this->name);

}
}
