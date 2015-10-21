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

#ifndef IO_HPP
#define IO_HPP
#include<iostream>
#include<string>
#include<typeinfo>
#include<boost/smart_ptr/shared_ptr.hpp>

#include "values.hpp"
#include "exposedValues.hpp"
#include "errorHandling.hpp"

namespace lughos
{

  class ioContext
  {};
  
  template <class C> class ioWrapper
  {
  public:
    
    template <class O> O output()
    {
      
    }
    
    template <class O> void input(O o)
    {
      
    }
    
  };
  
  template <class C> class textLine : public ioWrapper<C>
  {
    
  };
  
  
  template <class C, class T> class defaultWrapper : textLine<C>
  {};
  
  template <class C> class ioRenderer
  {
  public:
    template <class T> void output(Value<T>& t)
    {
      std::cout << t.getValue() << std::endl;
    }    
    
    template <class T> void output(ExposedValue<T>& eV)
    {
      std::cout << "-= Object " << eV.getName() << "=-" << std::endl;
      for (int i = 0; i < eV.countChildren(); i++)
      {
// 	std::cout << eV->getName() << " - " << eV.getAs<ExposedValue>(i)->getValueAsString() << std::endl;
      }
    }
  };
  
  

} //namespace lughos
#endif