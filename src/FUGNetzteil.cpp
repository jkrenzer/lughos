// #include "StdAfx.h"

#include <ostream>
#include "tcpAsync.hpp"
#include "FUGNetzteil.hpp"
#include "utils.hpp"

#include <string>     // std::string, std::stoi
FUGNetzteil::FUGNetzteil()
{
}

FUGNetzteilConnection::FUGNetzteilConnection(boost::shared_ptr< boost::asio::io_service > io_service): tcpAsync(io_service), Connection<tcpContext>(io_service)
{
}

FUGNetzteil::~FUGNetzteil(void)
{

}



std::string FUGNetzteil::composeRequest(std::string query)
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

std::string FUGNetzteil::interpretAnswer(std::string s)
{     

//  static const boost::regex e("<body>(.*)</body>");
//  boost::regex e("(.*)");
// //   s.erase( std::remove(s.begin(), s.end(), '\n'), s.end() );
//  boost::cmatch res;
//  boost::regex_search(s.c_str(), res, e);
//   
//   return res[1];  
  
  return s;

}

void FUGNetzteil::initImplementation()
{
}
    

void FUGNetzteil::shutdownImplementation()
{
}

bool FUGNetzteil::isConnectedImplementation()
{
  return true;
}


measuredValue FUGNetzteil::getMeasure(bool force)
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


int FUGNetzteil::switchVoltage(int i)
{
  int success=0;
  std::string command="F";
  command +=std::to_string(i);
  command=inputOutput(command);
  if (command=="E0"){ success = i;  voltagesOnOf=i;}
  else success = 3;
  return success;
  
}

int FUGNetzteil::setI(double I)
{
  int success=0;
  std::string command="I";
  std::string answer="";
  command +=std::to_string(I);
  answer=inputOutput(command);
  std::cout<<"setI answer: "<<answer<<std::endl;
  if (answer=="E0"){ success = 1;}
  else
  {
  success = 0;
  setError(command, answer);
  }

  return success;
  
}


int FUGNetzteil::setU(double U)
{
  int success=0;
  std::string command="U";
  std::string answer="";
  command +=std::to_string(U);
  answer=inputOutput(command);
  std::cout<<"setU answer: "<<answer<<std::endl;
  if (answer=="E0"){ success = 1;}
  else
  {
  success = 0;
  setError(command, answer);
  }
  return success;
  
}

std::string FUGNetzteil::getLastError()
{
  return lastError;
  
}

std::string FUGNetzteil::getIDN()
{
  std::cout << "FIRST TRY: " << inputOutput("?") << std::endl;
  return inputOutput("?");
}


double FUGNetzteil::getI()
{
  int success=0;
  std::string answer="";
   answer=inputOutput(">M1?");
      std::cout<<"i/O: "<<iO<<std::endl;
      std::cout<<"answer: "<<answer<<std::endl;
  if (answer[0]=='M')
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

double FUGNetzteil::getU()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">M0?");
  std::cout<<"getU answer: "<<answer<<std::endl;
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

double FUGNetzteil::getSetpointI()
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
  setError("getSetpointI", answer);
  }

  return -1;
  
}

double FUGNetzteil::getSetpointU()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">S0?");
  if (answer[0]=='S')
  {
   answer= answer.erase(0, 3);
  return std::stod(answer);
  }
  else if (answer[0]=='E')
  {
  setError("getSetpointU", answer);
  }

  return -1;
  
}

bool FUGNetzteil::hasOvercurrent()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">D3R?");
  if (answer[0]=='D')
  {
   answer= answer.erase(0, 4);
  return std::stod(answer) == 1;
  }
  else if (answer[0]=='E')
  {
  setError("hasOvercurrent", answer);
  }

  return false;
}

bool FUGNetzteil::currentLimitation()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">DIR?");
  if (answer[0]=='D')
  {
   answer= answer.erase(0, 4);
  return std::stod(answer) == 1;
  }
  else if (answer[0]=='E')
  {
  setError("currentLimitation", answer);
}

bool FUGNetzteil::voltageLimitation()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">DVR?");
  if (answer[0]=='D')
  {
   answer= answer.erase(0, 4);
  return std::stod(answer) == 1;
  }
  else if (answer[0]=='E')
  {
  setError("voltageLimitation", answer);
}

void FUGNetzteil::resetOvercurrent()
{
  inputOutput(">B1 1");
}


void FUGNetzteil::setError(std::string command, std::string error)
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