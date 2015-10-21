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

#ifndef MEASURED_DB_VALUE_HPP
#define MEASURED_DB_VALUE_HPP

#include <iostream>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/QueryModel>
#include <boost/asio/io_service.hpp>
#include "measuredValue.hpp"
#include "jobQueue.hpp"

using namespace std;
using namespace lughos;
namespace dbo = Wt::Dbo;

template <class T>
class measuredDBValue : public measuredValue<T>
{
public:
  
  template<class Action>
  void persist(Action& a)
  {
    dbo::field(a, this->timeStamp,     "timestamp");
    dbo::field(a, this->timeStampTicks, "ticks");
    dbo::field(a, this->sensorName, "sensorName");
    dbo::field(a, this->unit,     "unit");
    dbo::field(a, *this->valuePointer,    "value");
  }
  
  measuredDBValue(measuredValue<T> other) : measuredValue<T>(other)
  {
    
  }
  
  measuredDBValue() : measuredValue<T>()
  {
    
  }
  
};

#endif
