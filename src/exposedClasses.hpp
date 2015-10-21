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

#ifndef EXPOSED_CLASSES_HPP
#define EXPOSED_CLASSES_HPP
#include "exposedValues.hpp"
#include "exposedFunctions.hpp"

namespace lughos
{
  
  class ExposedClass : public ExposedObject
  {
  private:
    mutable Mutex mutex;
  protected:
    ExposerRegistry members;
    
    bool declared;
    
    virtual void memberDeclaration() = 0;
    
    virtual void addMember(ExposedObject& object)
    {
      this->addMember(&object);
    }
    
    virtual void addMember(ExposedObject* object)
    {
      ExclusiveLock lock(mutex);
      this->members.addObject(object);
    }
    
    template <class ReturnType, class... Arguments> void addFunction(boost::function<ReturnType(Arguments...)> f, std::string name,std::string description = "")
    {
      this->addMember(new ExposedFunction<ReturnType, Arguments...>(f,name));
    }
    
    template <class T> void addMember(T& member, std::string name, std::string description = "")
    {
      this->addMember(new ExposedValue<T>(member,name));
    }
    
  public:
    
    ExposedObject* operator[](std::string name)
    { 
      SharedLock lock(mutex);
      if(!this->declared)
	this->memberDeclaration();
      return members[name];
    }
    
    template <class T>
    T* get(std::string name)
    {
      T* ptr = dynamic_cast<T*>(this->operator[](name));
      if(ptr != nullptr)
        return ptr;
      else
        BOOST_THROW_EXCEPTION( exception() << errorName("wrong_cast_access_exposed_value"));
    }

    ExposedObject* operator[](int i)
    {
      SharedLock lock(mutex);
      if(!this->declared)
	this->memberDeclaration();
      return members[i];
    }
    
    template <class T>
    T* get(int i)
    {
      T* ptr = dynamic_cast<T*>(this->operator[](i));
      if(ptr != nullptr)
        return ptr;
      else
        BOOST_THROW_EXCEPTION( exception() << errorName("wrong_cast_access_exposed_value"));
    }
    
    template <class T>
    T getValue(std::string name)
    {
      ExposedValue<T>* ptr = dynamic_cast<ExposedValue<T>*>(this->operator[](name));
      if(ptr != nullptr)
        return *ptr;
      else
        BOOST_THROW_EXCEPTION( exception() << errorName("wrong_cast_access_exposed_value"));
    }
    
    template <class T>
    T getValue(int i)
    {
      ExposedValue<T>* ptr = dynamic_cast<ExposedValue<T>*>(this->operator[](i));
      if(ptr != nullptr)
        return *ptr;
      else
        BOOST_THROW_EXCEPTION( exception() << errorName("wrong_cast_access_exposed_value"));
    }
    
    
    ExposedClass()
    { 
      ExclusiveLock lock(mutex);
      this->declared = false;
    }
       
  };

} //namespace lughos
#endif