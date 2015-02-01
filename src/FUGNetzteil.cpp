// #include "StdAfx.h"

#include <ostream>
#include "tcpAsync.hpp"
#include "FUGNetzteil.hpp"

#include <string>     // std::string, std::stoi
fug::fug()
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

template <class T> void fug::setDefaultImpl(T& connection)
{
}

template <> void fug::setDefaultImpl< Connection<tcpContext> > (Connection<tcpContext>& connection)
{
  serverName= connection.server_name;  
}

fug::~fug(void)
{

}



std::string fug::composeRequest(std::string query)
{
    std::string requestString="";
//   std::string host_path=std::string("/scpi_response.html?cmd=");
  
//     requestString+=std::string("GET ");
// //     requestString+=std::string("");
//     // requestString+=std::string(host_path);
//     
//     requestString+=std::string(query);  
//     requestString+=std::string(" HTTP/1.0\r\n");  
//     requestString+=std::string("Host: ");
//     requestString+=std::string(serverName);  
//     requestString+=std::string("\r\nAccept: */*\r\nConnection: close\r\n\r\n");
    requestString+=query;
    requestString+=std::string("\r\n");

    return requestString;
  
}


void fug::set_default()
{

}
std::string fug::interpretAnswer(std::string s)
{     

//  static const boost::regex e("<body>(.*)</body>");
 boost::regex e("(.*)");
//   s.erase( std::remove(s.begin(), s.end(), '\n'), s.end() );
 boost::cmatch res;
 boost::regex_search(s.c_str(), res, e);
  
  return res[1];  

}

void fug::initImplementation()
{
}
    

void fug::shutdownImplementation()
{
}

measuredValue fug::getMeasure(bool force)
{
  if(!force &&!storedMeasure.gettimestamp().is_not_a_date_time()&& storedMeasure.gettimestamp()>boost::posix_time::second_clock::local_time()+boost::posix_time::seconds(5))
  {
    return storedMeasure;
   }
    

  std::string s = this->inputOutput("READ?");
  boost::regex e("([\\d\\.+-]*[E][\\d\\.+-]*)([\\w]*)");

  boost::cmatch res;
  boost::regex_search(s.c_str(), res, e);
  double number = save_lexical_cast<double>(res[1],-1);

  s=res[2];
  measuredValue value;
  if(!s.empty() && number == 0)
  {
    value.setvalue(save_lexical_cast<double>(s,std::numeric_limits<double>::signaling_NaN()));
    value.setunit(s);
    storedMeasure=value;
  }
    if(s.empty() && number == 0)
  {
    value.setvalue(save_lexical_cast<double>(s,std::numeric_limits<double>::signaling_NaN()));
    value.setunit("");
    storedMeasure=value;    
  }
  else 
  {
    value.setvalue(number);
    value.setunit(s);
    value.settimestamp(boost::posix_time::second_clock::local_time());
    storedMeasure=value;
  }

  return value;
  
}


int fug::switchVoltage(int i)
{
  int success=0;
  std::string command="F";
  command +=std::to_string(i);
  command=inputOutput(command);
  if (command=="E0"){ success = i;  voltagesOnOf=i;}
  else success = 3;
  return success;
  
}

int fug::setI(double I)
{
  int success=0;
  std::string command="I";
  std::string answer="";
  command +=std::to_string(I);
  answer=inputOutput(command);
  if (answer=="E0"){ success = 1;}
  else
  {
  success = 0;
  setError(command, answer);
  }

  return success;
  
}


int fug::setU(double U)
{
  int success=0;
  std::string command="U";
  std::string answer="U";
  command +=std::to_string(U);
  answer=inputOutput(command);
  if (answer=="E0"){ success = 1;}
  else
  {
  success = 0;
  setError(command, answer);
  }
  return success;
  
}

std::string fug::getLastError()
{
  return lastError;
  
}

std::string fug::getIDN()
{
  return inputOutput("?");
}


double fug::getI()
{
  int success=0;
  std::string answer="";
//   answer=inputOutput(">S1?");
   std::string iO = inputOutput(">S1?"); //Das Zwillingsparadoxon ?!?!
      std::cout<<"i/O: "<<iO<<std::endl;
      std::cout<<"answer: "<<answer<<std::endl;
  if (answer[0]=='S')
  {
   answer= answer.erase(0, 3);
  return std::stod(answer);
  }
  else if (answer[0]=='E')
  {
  setError("getI", answer);
  }

  return -1;
  
}

double fug::getU()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">M0?");
  if (answer[0]=='M')
  {
   answer= answer.erase(0, 3);
  return std::stod(answer);
  }
  else if (answer[0]=='E')
  {
  setError("getU", answer);
  }

  return -1;
  
}

void fug::setError(std::string command, std::string error)
{
  if (error=="E0") return;
  lastError ="";
  lastError += boost::posix_time::to_iso_extended_string(boost::posix_time::second_clock::local_time());
  lastError +=" ";
  lastError += command;
  lastError += " ; ";
      error.erase( std::remove(error.begin(), error.end(), '\n'), error.end() );
  lastError += error;
    lastError += ": ";
  std::string discription;
  int errorNum;
    error.erase( std::remove(error.begin(), error.end(), 'E'), error.end() );
    
  if(!error.empty())
    errorNum = std::stoi (error);
  else
    errorNum = -1;
   
   if (errorNum == 1)
   {
   discription="keine Daten verfügbar";
    }
    else if (errorNum == 2)
   {
    discription="unbekannter Register-Typ";    
    }
    else if (errorNum == 4)
   {
    discription="ungültiges Argument";    
    }
        else if (errorNum == 5)
   {
    discription="Bereich überschritten";    
    }
    else if (errorNum == 6)
   {
    discription="Register darf nur gelesen werden";    
    }
      else if (errorNum == 7)
   {
    discription="Receive Overflow";    
    }
      else if (errorNum ==8 )
   {
    discription="EEPROM ist schreibgeschützt";    
    }
          else if (errorNum == 9)
   {
    discription="Adressfehler";    
    }
          else if (errorNum ==10 )
   {
    discription="unbekannter SCPI Befehl";    
    }
          else if (errorNum ==11 )
   {
    discription="Trigger on Talk Fehler";    
    }
              else if (errorNum ==12 )
   {
    discription="~Tn Befehl hatte ungültiges Argument";    
    }
              else if (errorNum ==13 )
   {
    discription="ungültiger N-Wert";    
    }
              else if (errorNum == 14)
   {
    discription="Register darf nurbeschrieben werden";    
    }
              else if (errorNum == 15)
   {
    discription="String zu lang";    
    }
              else if (errorNum == 101||errorNum == 102||errorNum == 103)
   {
    discription="Checksumme falsch";    
    }

else
   {
      discription="unbekannter Fehler";     
  }
   lastError += discription; 
  return;
}