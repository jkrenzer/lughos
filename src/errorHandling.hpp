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

#ifndef ERROR_HANDLING_HPP
#define ERROR_HANDLING_HPP
#include <string>
#include <sstream>
#include <boost/exception/all.hpp>

namespace lughos
{

enum severity { Neglectable, Informative, ShouldNot, MustNot, Fatal, Catastrophic };

typedef boost::error_info<struct errorNameStruct,std::string> errorName;
typedef boost::error_info<struct errorTitleStruct,std::string> errorTitle;
typedef boost::error_info<struct errorDrescriptionStruct,std::string> errorDescription;
typedef boost::error_info<struct errorSeverityStruct,severity> errorSeverity;

struct exception: virtual boost::exception, virtual std::exception { };


std::string severityToString(severity s);

std::string makeErrorReport(exception& e);



} //namespace lughos
#endif