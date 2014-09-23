#include "treeObj.hpp"

namespace lughos {
void TreeNode::_setChild(TreeNode* objectPtr)
{
    objectPtr->addChild(this);
}

void TreeNode::_unsetChild(TreeNode* objectPtr)
{
    objectPtr->removeChild(this);
}
template <class T> T* TreeNode::getParent()
{
    return (T*)this->parent;
}
void TreeNode::setParent(TreeNode* objectPtr,bool callback)
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

 void TreeNode::_setParent(TreeNode* objectPtr)
{
    objectPtr->setParent(this);
}
 void TreeNode::_unsetParent(TreeNode* objectPtr)
{
    objectPtr->setParent(NULL);
}
int TreeNode::countChildren()
{
    return this->children.size();
}
std::vector< TreeNode* > TreeNode::getChildren()
{
    return std::vector<TreeNode*>(this->children);
}
void TreeNode::addChild(TreeNode* objectPtr)
{
    if(!isChild(objectPtr) && objectPtr != NULL)
    {
        this->children.push_back(objectPtr);
        this->childrenNames.push_back(objectPtr->getName());
        this->_setParent(objectPtr);
    }
}
void TreeNode::removeChild(TreeNode* objectPtr,bool callback)
{
    if(isChild(objectPtr))
    {
        this->children.erase(std::find(this->children.begin(), this->children.end(), objectPtr));
        this->childrenNames.erase(std::find(this->childrenNames.begin(), this->childrenNames.end(), objectPtr->getName()));
	if(callback)
	  objectPtr->setParent(NULL);

    }
}
bool TreeNode::isChild(std::string name)
{
    return std::find(this->childrenNames.begin(), this->childrenNames.end(), name) != this->childrenNames.end();
}
bool TreeNode::isChild(TreeNode* objectPtr)
{
    return std::find(this->children.begin(), this->children.end(), objectPtr) != this->children.end();
}
template <class T> T* TreeNode::getAs(std::string name)
{
  return dynamic_cast<T*>(get(name));
}
std::string TreeNode::getNameOf(TreeNode* objectPtr)
{
    std::vector<TreeNode*>::iterator it = std::find(this->children.begin(), this->children.end(), objectPtr);
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
