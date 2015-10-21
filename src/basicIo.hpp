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

#ifndef BASIC_IO_HPP
#define BASIC_IO_HPP
#include<iostream>
#include<string>
#include <boost/lexical_cast.hpp>
#include "io.hpp"

namespace lughos
{

class consoleContext : public ioContext
{
public:
  std::string getName()
  {
    return std::string("consoleContext");
  }
  
  std::string getDescription()
  {
    return std::string("Context for input/output of values via console interface.");
  }
  
};

class IoWrapperInterface
{
public:
  
};

template <> class ioWrapper<consoleContext> : public IoWrapperInterface
{
public:
  virtual std::string output() = 0;
  
};

template <> class textLine<consoleContext> : public ioWrapper<consoleContext>
  {
  protected:
    std::string text;
    ValueInterface* object;
    
    template <class T> void callbackTemplate(textLine<consoleContext>* objPtr)
    {
      std::cout << "Interpreting string \"" << this->text << "\" to type " << typeid(T).name() << std::endl;
      Value<T>* e = dynamic_cast<Value<T>*>(objPtr->object);
      if (e)
      {
	T value = boost::lexical_cast<T>(objPtr->text);
	std::cout << "Got value " << value << " of type " << typeid(value).name() << std::endl;
	e->setValue(value);
      }
      else
	std::cout << "Could not cast. :/" << std::endl;
      
    }
    
    void (textLine<consoleContext>::*callback)(textLine<consoleContext>*);
    
  public:
    template <class T> textLine(Value<T> &e)
    {
      
      this->text = boost::lexical_cast<std::string>(e.getValue());
      this->object = dynamic_cast<ValueInterface*>(&e);
      this->callback = &textLine< consoleContext >::callbackTemplate<T>;
    }
    
    std::string output()
    {
      return this->text;
    }
    
    void input(std::string s)
    {
      this->text = s;
      (this->*callback)(this);
    }
    
//     template <class T> void input(T t)
//     {
//       Value<T>* e = dynamic_cast<Value<T>*>(this->object);
//       if(e)
//       {
// 	e->setValue(boost::lexical_cast<T>(t));
//       }
//       else
// 	std::cout << "Could not set value. :/" << std::endl;
//         
//     }
  };
  

}//namespace lughos
#endif