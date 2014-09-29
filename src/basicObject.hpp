#ifndef BASIC_OBJECT_HPP
#define BASIC_OBJECT_HPP

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
   
};

}//namespace lughos
#endif