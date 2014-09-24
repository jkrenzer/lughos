#ifndef PERSIST_HPP
#define PERSIST_HPP

#include "exposedValues.hpp"

#include <Wt/Dbo/Dbo>

namespace lughos {

class persistContext
{
};

class wtContext
{
};

namespace dbo = Wt::Dbo;

class PersistantRawValue
{
protected:
  TreeNode* linkedObj;
  dbo::collection<dbo::ptr<PersistantRawValue> > children;
  dbo::ptr<PersistantRawValue> parent;
  
  dbo::Session* session;
public:
  
  std::string name;
  
  PersistantRawValue() : children()
  {
    this->session = NULL;
    this->linkedObj = NULL;
  }
  
  PersistantRawValue(dbo::Session* session) : children()
  {
    this->linkedObj = NULL;
    this->session = session;
  }
  
  PersistantRawValue(TreeNode* eO,dbo::Session* session) : children()
  {
    this->session = session; session->
    this->sync(eO);
  }
  
  void sync(TreeNode* eO)
  {
    this->linkedObj = eO;
    if(this->session != NULL && this->linkedObj != NULL)
    {
      this->children.clear();
      this->name = eO->getName();
      std::vector<boost::shared_ptr<TreeNode>> eOChildren = eO->getChildren();
      std::cout << "We have children: " << eOChildren.size() << std::endl;
      if(eOChildren.size() > 0)
      {
      for(std::vector<boost::shared_ptr<TreeNode>>::iterator it = eOChildren.begin(); it < eOChildren.end();it++)
	{
	  dbo::ptr<PersistantRawValue> ptr = this->session->add(new PersistantRawValue(it->get(),this->session));
	  dbo::Transaction transaction(*this->session);
	  this->children.insert(ptr);
	  transaction.commit();
	}
      }
//       this->parent = dbo::ptr<PersistantRawValue>(this->session->add(new PersistantRawValue(eO->getParent<TreeNode>().get(),this->session)));
  }
  }
  
  void sync()
  {
    this->sync(linkedObj);
  }
  
  template<class Action>
  void persist(Action& a)
  {
    dbo::field(a,name,"name");
    dbo::hasMany(a,children,dbo::ManyToOne,"children");
    dbo::belongsTo(a,parent,"children");
  }
  
};



template <class C> class Persistor
{
protected:
  
public:
  
};

template <> class Persistor<wtContext>
{
protected:
  
public:
  
  
  
};

}

#endif