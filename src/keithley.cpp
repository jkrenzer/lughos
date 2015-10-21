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

#include <ostream>
#include "tcpConnections.hpp"
#include "keithley.hpp"

KeithleyConnection::KeithleyConnection()
{

}

Keithley::Keithley()
{
  set_default();
}

template <class T, class S> T save_lexical_cast(S& source, T saveDefault)
{
  try
  {
    return boost::lexical_cast<T>(source);
  }
  catch(boost::bad_lexical_cast e)
  {
    return saveDefault;
  }
  
}

Keithley::~Keithley(void)
{

}



std::string Keithley::composeRequest(std::string query)
{
    std::string requestString="";
//   std::string host_path=std::string("/scpi_response.html?cmd=");
  
    requestString+=std::string("GET ");
    requestString+=std::string("/scpi_response.html?cmd=");
    // requestString+=std::string(host_path);
    
    requestString+=std::string(query);  
    requestString+=std::string(" HTTP/1.0\r\n");  
    requestString+=std::string("Host: ");
    requestString+=std::string("localhost");  
    requestString+=std::string("\r\nAccept: */*\r\nConnection: close\r\n\r\n");
//     requestString+=query;
//     requestString+=std::string("\r");

    return requestString;
  
}


void Keithley::set_default()
{

}

std::string Keithley::interpretAnswer(std::string s)
{     

 boost::regex e("<body>(.*)</body>");
 // boost::regex e("(.*)");
 boost::cmatch res;
 boost::regex_search(s.c_str(), res, e);
  
  return res[1];  

}

void Keithley::initImplementation()
{
}
    

void Keithley::shutdownImplementation()
{
}

bool Keithley::isConnectedImplementation()
{
  std::string s = this->inputOutput("*IDN?");
  boost::regex e("(KEITHLEY INSTRUMENTS INC)");

  boost::smatch res;
  boost::regex_search(s, res, e);
  return res[1] != "";
}


measuredValue<double> Keithley::getMeasure(bool force)
{
  if(!force &&!storedMeasure.getTimeStamp().is_not_a_date_time()&& storedMeasure.getTimeStamp()>boost::posix_time::second_clock::local_time()+boost::posix_time::seconds(5))
  {
    return storedMeasure;
   }
    

  std::string s = this->inputOutput("READ?");
  static const boost::regex e("([\\d\\.+-]*[E][\\d\\.+-]*)([\\w]*)");

  boost::cmatch res;
  boost::regex_search(s.c_str(), res, e);
  double number = save_lexical_cast<double>(res[1],-1);

  s=res[2];
  measuredValue<double> value;
  if(!s.empty() && number == 0)
  {
    value.setValue(save_lexical_cast<double>(s,std::numeric_limits<double>::signaling_NaN()));
    value.setUnit(s);
    storedMeasure=value;
  }
    if(s.empty() && number == 0)
  {
    value.setValue(save_lexical_cast<double>(s,std::numeric_limits<double>::signaling_NaN()));
    value.setUnit("");
    storedMeasure=value;    
  }
  else 
  {
    value.setValue(number);
    value.setUnit(s);
    value.setTimeStamp(boost::posix_time::second_clock::local_time());
    storedMeasure=value;
  }

  return value;
  
}
