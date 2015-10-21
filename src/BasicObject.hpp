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