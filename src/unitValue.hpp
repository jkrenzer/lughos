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

#ifndef UNITVALUE_HPP
#define UNITVALUE_HPP


#include "values.hpp"
#include <string>
#include <cstring>
#include <iostream>

namespace lughos
{

  class unitValueInterface
  {
  protected:
    std::string unit = "";

  public:
    std::string getUnit () const;
    void setUnit (std::string putunit);
    virtual std::string getString () const = 0;

  };

  template < class T >
  class unitValueTemplate:public unitValueInterface, virtual public Value<T>
  {
  public:
    unitValueTemplate (void);
    unitValueTemplate (const unitValueTemplate<T>& other);
    unitValueTemplate (T putvalue, std::string putunit);
    virtual ~unitValueTemplate (void);

    void setValueAndUnit (T putvalue, std::string putunit);
    virtual std::string getString () const;
  };

template < class T > class unitValue: public unitValueTemplate<T>
  {

  };

//Definition of template classes as required by C++ standard

  template <class T> unitValueTemplate<T>::unitValueTemplate() : Value<T>()
  {
    
  }
  
  template <class T> unitValueTemplate<T>::unitValueTemplate (const unitValueTemplate < T > &other) : Value<T>(other)
  {
    this->unit = other.unit;
  }



  template <class T> unitValueTemplate<T>::unitValueTemplate(T putvalue, std::string putunit)
  {
    *(this->valuePointer)  = putvalue;
    this->unit = putunit;
  }

  template <class T> unitValueTemplate<T>::~unitValueTemplate(void)
  {

  }
  
  template <class T> std::string unitValueTemplate<T>::getString() const
  {
    return this->getValueAsString() + this->unit;
  }
  
  template <class T> void unitValueTemplate<T>::setValueAndUnit(T putvalue, std::string putunit)
  {
    this->setValue(putvalue);
    this->setUnit(putunit);
      return;
  }
  
}				//namespace lughos

#endif
