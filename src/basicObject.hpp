#ifndef BASIC_OBJECT_HPP
#define BASIC_OBJECT_HPP

#include <Wt/Dbo/Dbo>
#include "device.hpp"

namespace lughos
{

class basicObject
{
protected:
  std::string name;
  

  
public:
  
  basicObject() : name("N/A")
  {
  }
  
  basicObject(std::string name)
  {
    this->name = name;
  }
  
  ~basicObject()
  {
  }
  
  std::string getName()
  {
    return this->name;
  }
  
  void setName(std::string name)
  {
    this->name = name;
  }
  


     Wt::Dbo::collection< Wt::Dbo::ptr<basicObject> > channel;
    template<class Action>
    void persist(Action& a)
    {
      Wt::Dbo::field(a, name, "name");
      Wt::Dbo::hasMany(a, channel, Wt::Dbo::ManyToOne, "device");
    } 
   
};

}//namespace lughos
#endif