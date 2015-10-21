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

namespace lughos 
{  
  template <class R> R cachedFunction<R>::operator()()
  {
      if(!this->isValid() && readFunction)
      {
	  this->tainted = false;
	  return readFunction();
      }
      else
	  return cachedValue;
  }
  template <class R> void cachedFunction<R>::setReadFunction(boost::function< R() > readFunction)
  {
      this->readFunction = readFunction;
  }

  template <class R> void cachedFunction<R>::taint()
  {
      this->tainted = true;
  }

  template <class R> cachedFunction<R>::cachedFunction()
  {
      this->tainted = true;
      this->interval = boost::posix_time::seconds(1);
  }

  template <class R> bool cachedFunction<R>::isValid()
  {
      return (boost::posix_time::microsec_clock::local_time() - timestamp < interval ) && this->tainted;
  }
}//namespace lughos