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

#ifndef MAKROS_HPP
#define MAKROS_HPP

//Automatically declare getter and setter-functions for variable X of type T

#define DECLARE_GETTER_SETTER(V,T) \
    void set ## V(T V);		\
				\
    const T get ## V() const;


//Automatically define getter and setter-functions for variable X of type T

#define DEFINE_GETTER_SETTER(C,V,T) \
    void C::set ## V(T V)		\
    {				\
      this->V = V;		\
    }				\
				\
    const T C::get ## V() const		\
    {				\
      return this->V;	\
    }

    
#endif