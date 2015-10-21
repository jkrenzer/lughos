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
// #pragma comment(lib, "Setupapi.lib")
#include "serialConnections.hpp"
#include "MaxiGauge.hpp"
#include <limits>

MaxiGauge::MaxiGauge()
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

template <class T> void MaxiGauge::setDefaultImpl(T& connection)
{
}

template <> void MaxiGauge::setDefaultImpl< Connection<serialContext> > (Connection<serialContext>& connection)
{
  
    connection.baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    connection.flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    connection.character_size=boost::asio::serial_port_base::character_size(8);
    connection.endOfLineRegExpr_='\x0A';
    connection.parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    connection.stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);
//     this->inputOutput("\x03");
//         this->inputOutput("\x05");
}

MaxiGauge::~MaxiGauge(void)
{

}

std::string MaxiGauge::composeRequest(std::string query)
{
    std::string requestString="";
    requestString+=query;
//     if (query!="\x05")
      requestString+=std::string("\r");

    return requestString;
  
}

  void MaxiGauge::set_default()
{
   this->setDefaultImpl(*(this->connection.get()));
}

std::string MaxiGauge::interpretAnswer(std::string s)
{  
//         this->inputOutput("\x05");  
//   std::cout << "interpretAnswer " << s<<std::endl; 
    if(s=="\x15""\r\n")
    { 
//       std::cout<<"NAK "<<std::endl;
      return "NAK";
    }
    else if(s=="\x06""\r\n"||s=="\x06""\r"||s=="\n\x06""\r") 
    { 
//             std::cout<<"ACK "<<std::endl;
 return this->inputOutput("\x05");
      
    }
    else
    {
//       std::cout<<"string "<<s<<" "<<std::endl;
//        static const boost::regex e("([\\d\\w,\\.+-]*)");
// 	 static const boost::regex e("^(\\d*)\\D\\D$");
// 	 boost::cmatch res;
// 	 boost::regex_search(s.c_str(), res, e);
	 s.erase( std::remove(s.begin(), s.end(), '\r'), s.end() );
	 s.erase( std::remove(s.begin(), s.end(), '\n'), s.end() );
// 	 std::cout<<"Regex "<<res[1]<<std::endl;
	 return s;   
    }

}


   bool MaxiGauge::sensor_on(int sensor)
{
   int j=0;
  int check_bench[6];
  std::string response_string=this->inputOutput("SEN,0,0,0,0,0,0").c_str();
//   static const boost::regex e("(\\d)");
//   boost::cmatch res;
//     boost::regex_search(  this->inputOutput("SEN,0,0,0,0,0,0").c_str(), res, e);
//   for(int i=0;i<6;i++) sensor_bench[i]=boost::lexical_cast< int >(res[i+1]);
  j=0;
  for ( std::string::iterator it=response_string.begin(); it!=response_string.end(); ++it)
  {
    sensor_bench[j]=(int)*it;
     if(it!=response_string.end()) it++;
     j++;
  } 	

  std::string request= "SEN,";
  for(int i=0;i<6;i++){
      if(i!=sensor)
      {
	  request.append(std::to_string(0)); 
	
      }
      else request.append(std::to_string(2));
  }
   
//   boost::regex_search(  this->inputOutput(request).c_str(), res, e);
//   for(int i=0;i<6;i++) check_bench[i]=boost::lexical_cast< int >(res[i+1]);
    j=0;
    for ( std::string::iterator it=response_string.begin(); it!=response_string.end(); ++it){
     
    check_bench[j]=(int)*it;
     if(it!=response_string.end()) it++;
    j++;
      
    } 
  
  for(int i=0;i<6;i++){
      if(i!=sensor&&check_bench[i]==sensor_bench[i])
      {
      }
       else if(i==sensor&&check_bench[i]==2)
      {
      }
      else return false;
  }
      for(int i=0;i<6;i++) sensor_bench[i]=check_bench[i];
  return true;
 
  
  
}

bool MaxiGauge::sensor_off(int sensor)
{
   int j=0;
  int check_bench[6];
  std::string response_string=this->inputOutput("SEN,0,0,0,0,0,0").c_str();
//   static const boost::regex e("(\\d)");
//   boost::cmatch res;
//     boost::regex_search(  this->inputOutput("SEN,0,0,0,0,0,0").c_str(), res, e);
//   for(int i=0;i<6;i++) sensor_bench[i]=boost::lexical_cast< int >(res[i+1]);
  j=0;
  for ( std::string::iterator it=response_string.begin(); it!=response_string.end(); ++it)
  {
    sensor_bench[j]=(int)*it;
     if(it!=response_string.end()) it++;
     j++;
  } 	

  std::string request= "SEN,";
  for(int i=0;i<6;i++){
      if(i!=sensor)
      {
	  request.append(std::to_string(0)); 
	
      }
      else request.append(std::to_string(1));
  }
   
//   boost::regex_search(  this->inputOutput(request).c_str(), res, e);
//   for(int i=0;i<6;i++) check_bench[i]=boost::lexical_cast< int >(res[i+1]);
    j=0;
    for ( std::string::iterator it=response_string.begin(); it!=response_string.end(); ++it){
     
    check_bench[j]=(int)*it;
     if(it!=response_string.end()) it++;
    j++;
      
    } 
  
  for(int i=0;i<6;i++){
      if(i!=sensor&&check_bench[i]==sensor_bench[i])
      {
      }
       else if(i==sensor&&check_bench[i]==1)
      {
      }
      else return false;
  }
      for(int i=0;i<6;i++) sensor_bench[i]=check_bench[i];
  return true;
 
  
}

  std::string MaxiGauge::get_status(int sensor)
{
  std::string request= "PR";
  request.append(std::to_string(sensor)); 
 
  return this->inputOutput(request);
  
}

void MaxiGauge::initImplementation()
{
//       this->inputOutput("\x03");
        this->inputOutput("\x05");
}
    

void MaxiGauge::shutdownImplementation()
{
}

measuredValue MaxiGauge::getPressure(int sensor, bool force)
{
    if(!force &&!storedPressure.getTimeStamp().is_not_a_date_time()&& storedPressure.getTimeStamp()>boost::posix_time::second_clock::local_time()+boost::posix_time::seconds(5))
  {
    return storedPressure;
   }
    
  
  std::string s = this->get_status(sensor);
  static const boost::regex e("^(\\d),(\\d*\\.\\d*E[+-]\\d*)");
  boost::cmatch res;
  boost::regex_search(s.c_str(), res, e);
  int state = save_lexical_cast<int>(res[1],-1);
  s = res[2];
  measuredValue value;
  if(!s.empty() && state == 0)
  {
    value.setValue(save_lexical_cast<double>(s,std::numeric_limits<double>::signaling_NaN()));
  }
  else if (state == 1)
  {
    value.setValue(-std::numeric_limits<double>::infinity());
  }
  else if (state == 2)
  {
    value.setValue(std::numeric_limits<double>::infinity());
  }
  else
  {
    value.setValue(std::numeric_limits<double>::signaling_NaN());
  }
    value.setTimeStamp(boost::posix_time::second_clock::local_time());
    return value;
}
