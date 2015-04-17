// #include "StdAfx.h"

#include <ostream>
#include "FUGNetzteil.hpp"
#include "utils.hpp"

#include <string>     // std::string, std::stoi
FUGNetzteil::FUGNetzteil()
{
  this->getI.setReadFunction(boost::bind(&FUGNetzteil::readI,this));
  this->getU.setReadFunction(boost::bind(&FUGNetzteil::readU,this));
  this->getSetpointI.setReadFunction(boost::bind(&FUGNetzteil::readSetpointI,this));
  this->getSetpointU.setReadFunction(boost::bind(&FUGNetzteil::readSetpointU,this));
  this->getOvercurrent.setReadFunction(boost::bind(&FUGNetzteil::readOvercurrent,this));
  this->getCurrentLimitation.setReadFunction(boost::bind(&FUGNetzteil::readCurrentLimitation,this));
  this->getVoltageLimitation.setReadFunction(boost::bind(&FUGNetzteil::readVoltageLimitation,this));
  this->getDigitalRemote.setReadFunction(boost::bind(&FUGNetzteil::readDigitalRemote,this));
  this->getAnalogueRemote.setReadFunction(boost::bind(&FUGNetzteil::readAnalogueRemote,this));
  this->getLocalControl.setReadFunction(boost::bind(&FUGNetzteil::readLocalControl,this));
}

FUGNetzteilConnection::FUGNetzteilConnection()
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
  std::string test = this->getIDN();
  std::cout << ">>>>>>>>>>>> IDN:" << test << std::endl;
  return test.substr(0,3) == std::string("FUG");
}


measuredValue<double> FUGNetzteil::getMeasure(bool force)
{
  if(!force &&!storedMeasure.getTimeStamp().is_not_a_date_time()&& storedMeasure.getTimeStamp()>boost::posix_time::second_clock::local_time()+boost::posix_time::seconds(5))
  {
    return storedMeasure;
   }
    

  std::string s = this->inputOutput("READ?");
  boost::regex e("([\\d\\.+-]*[E][\\d\\.+-]*)([\\w]*)");

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
  return inputOutput("?");
}


double FUGNetzteil::readI()
{
  std::string answer="";
   answer=inputOutput(">M1?");
      std::cout<<"getI answer: "<<answer<<std::endl;
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

double FUGNetzteil::readU()
{
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

double FUGNetzteil::readSetpointI()
{
  int success=0;
  std::string answer="";
  answer=inputOutput(">S1?");
      std::cout<<"readSetpointI answer: "<<answer<<std::endl;
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

double FUGNetzteil::readSetpointU()
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

bool FUGNetzteil::readOvercurrent()
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

bool FUGNetzteil::readCurrentLimitation()
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
}

bool FUGNetzteil::readVoltageLimitation()
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
}

bool FUGNetzteil::readAnalogueRemote()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">DSA?");
  if (answer[0]=='D')
  {
   answer= answer.erase(0, 4);
  return std::stod(answer) == 1;
  }
  else if (answer[0]=='E')
  {
  setError("analogueRemote", answer);
  }
}

bool FUGNetzteil::readDigitalRemote()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">DSD?");
  if (answer[0]=='D')
  {
   answer= answer.erase(0, 4);
  return std::stod(answer) == 1;
  }
  else if (answer[0]=='E')
  {
  setError("digitalRemote", answer);
  }
}

bool FUGNetzteil::readLocalControl()
{
  return !this->readDigitalRemote() && !this->readAnalogueRemote();
}


void FUGNetzteil::resetOvercurrent()
{
  inputOutput(">B1 1"); //Check
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