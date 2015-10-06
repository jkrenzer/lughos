#ifndef BASIC_OBJECT_HPP
#define BASIC_OBJECT_HPP

#include <Wt/Dbo/Dbo>
#include "threadSafety.hpp"

namespace lughos
{

class BasicObject
{
private:
  
  std::string getType_()
  {
    return std::string("BasicObject");
  }
  
protected:
  std::string name;
  Mutex mutex;

  
public:
  
  BasicObject() : name("N/A")
  {
  }
  
  BasicObject(std::string name)
  {
    this->name = name;
  }
  
  ~BasicObject()
  {
  }
  
  std::string getName()
  {
    SharedLock lock(BasicObject::mutex);
    return this->name;
  }
  
  void setName(std::string name)
  {
    ExclusiveLock lock(BasicObject::mutex);
    this->name = name;
  }
  
  std::string getType()
  {
    return this->getType_();
  }

//      Wt::Dbo::collection< Wt::Dbo::ptr<BasicObject> > channel;
//     template<class Action>
//     void persist(Action& a)
//     {
//       Wt::Dbo::field(a, name, "name");
//       Wt::Dbo::hasMany(a, channel, Wt::Dbo::ManyToOne, "device");
//     } 
   
};

}//namespace lughos
#endif