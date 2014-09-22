#include "treeObj.hpp"

namespace lughos {
void treeNode::_setChild(treeNode* objectPtr)
{
    objectPtr->addChild(this);
}

void treeNode::_unsetChild(treeNode* objectPtr)
{
    objectPtr->removeChild(this);
}
template <class T> T* treeNode::getParent()
{
    return (T*)this->parent;
}
void treeNode::setParent(treeNode* objectPtr,bool callback)
{
    if (this->parent != objectPtr && objectPtr != NULL)
    {
        this->parent = objectPtr;
	if(callback)
	  _setChild(objectPtr);
    }
    else if (this->parent != objectPtr && objectPtr == NULL)
    {
	if(callback)
	  _unsetChild(this->parent);
        this->parent = NULL;

    }
}

 void treeNode::_setParent(treeNode* objectPtr)
{
    objectPtr->setParent(this);
}
 void treeNode::_unsetParent(treeNode* objectPtr)
{
    objectPtr->setParent(NULL);
}
int treeNode::countChildren()
{
    return this->children.size();
}
std::vector< treeNode* > treeNode::getChildren()
{
    return std::vector<treeNode*>(this->children);
}
void treeNode::addChild(treeNode* objectPtr)
{
    if(!isChild(objectPtr) && objectPtr != NULL)
    {
        this->children.push_back(objectPtr);
        this->childrenNames.push_back(objectPtr->getName());
        this->_setParent(objectPtr);
    }
}
void treeNode::removeChild(treeNode* objectPtr,bool callback)
{
    if(isChild(objectPtr))
    {
        this->children.erase(std::find(this->children.begin(), this->children.end(), objectPtr));
        this->childrenNames.erase(std::find(this->childrenNames.begin(), this->childrenNames.end(), objectPtr->getName()));
	if(callback)
	  objectPtr->setParent(NULL);

    }
}
bool treeNode::isChild(std::string name)
{
    return std::find(this->childrenNames.begin(), this->childrenNames.end(), name) != this->childrenNames.end();
}
bool treeNode::isChild(treeNode* objectPtr)
{
    return std::find(this->children.begin(), this->children.end(), objectPtr) != this->children.end();
}
template <class T> T* treeNode::getAs(std::string name)
{
  return dynamic_cast<T*>(get(name));
}
std::string treeNode::getNameOf(treeNode* objectPtr)
{
    std::vector<treeNode*>::iterator it = std::find(this->children.begin(), this->children.end(), objectPtr);
    if (it != this->children.end())
        return this->childrenNames[it-this->children.begin()];
    else
        return std::string("");
}
std::vector<std::string> treeNode::path()
{
    std::vector<std::string> path;
    if (this->parent != NULL)
        path = this->parent->path();
    path.push_back(this->name);

}
}
