// #include "StdAfx.h"

#include <ostream>
#include "FUGNetzteil.hpp"
#include "utils.hpp"

#include <string>     // std::string, std::stoi
FUGNetzteil::FUGNetzteil() : voltage("voltage",this->ioService, this->ioStrand), current("current",this->ioService, this->ioStrand), targetVoltage("targetVoltage",this->ioService, this->ioStrand), targetCurrent("targetCurrent",this->ioService, this->ioStrand), limit("limit",this->ioService, this->ioStrand), controlMode("controlMode",this->ioService, this->ioStrand), outputState("outputState",this->ioService, this->ioStrand), overcurrent("overcurrent",this->ioService, this->ioStrand), model("model",this->ioService, this->ioStrand)
{
  this->voltage.getter(boost::bind(&FUGNetzteil::readU,this));
  this->current.getter(boost::bind(&FUGNetzteil::readI,this));
  this->targetVoltage.getter(boost::bind(&FUGNetzteil::readSetpointU,this));
  this->targetVoltage.setter(boost::bind(&FUGNetzteil::setU,this,_1));
  this->targetCurrent.getter(boost::bind(&FUGNetzteil::readSetpointI,this));
  this->targetCurrent.setter(boost::bind(&FUGNetzteil::setI,this,_1));
  this->limit.getter(boost::bind(&FUGNetzteil::readLimit,this));
  this->controlMode.getter(boost::bind(&FUGNetzteil::readControlMode,this));
  this->outputState.getter(boost::bind(&FUGNetzteil::readSwitch,this));
  this->outputState.setter(boost::bind(&FUGNetzteil::switchVoltage,this,_1));
  this->overcurrent.getter(boost::bind(&FUGNetzteil::readOvercurrent,this));
  this->overcurrent.setter(boost::bind(&FUGNetzteil::setOvercurrent,this,_1));
  this->model.getter(boost::bind(&FUGNetzteil::getIDN,this));
}

void FUGNetzteil::memberDeclaration()
{
  this->addMember(voltage);
  this->addMember(current);
  this->addMember(targetVoltage);
  this->addMember(targetCurrent);
  this->addMember(limit);
  this->addMember(controlMode);
  this->addMember(outputState);
  this->addMember(overcurrent);
  this->addMember(model);
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


// measuredValue<double> FUGNetzteil::getMeasure(bool force)
// {
//   if(!force &&!storedMeasure.getTimeStamp().is_not_a_date_time()&& storedMeasure.getTimeStamp()>boost::posix_time::second_clock::local_time()+boost::posix_time::seconds(5))
//   {
//     return storedMeasure;
//    }
//     
// 
//   std::string s = this->inputOutput("READ?");
//   boost::regex e("([\\d\\.+-]*[E][\\d\\.+-]*)([\\w]*)");
// 
//   boost::cmatch res;
//   boost::regex_search(s.c_str(), res, e);
//   double number = save_lexical_cast<double>(res[1],-1);
// 
//   s=res[2];
//   measuredValue<double> value;
//   if(!s.empty() && number == 0)
//   {
//     value.setValue(save_lexical_cast<double>(s,std::numeric_limits<double>::signaling_NaN()));
//     value.setUnit(s);
//     storedMeasure=value;
//   }
//     if(s.empty() && number == 0)
//   {
//     value.setValue(save_lexical_cast<double>(s,std::numeric_limits<double>::signaling_NaN()));
//     value.setUnit("");
//     storedMeasure=value;    
//   }
//   else 
//   {
//     value.setValue(number);
//     value.setUnit(s);
//     value.setTimeStamp(boost::posix_time::second_clock::local_time());
//     storedMeasure=value;
//   }
// 
//   return value;
//   
// }


bool FUGNetzteil::switchVoltage(bool i)
{
  bool success = false;
  std::string command="F";
  command += (i ? std::string("1") : std::string("0"));
  command=inputOutput(command);
  if (command=="E0") 
    return true;
  else
    return false;
}

bool FUGNetzteil::setI(double I)
{
  int success=0;
  std::string command="I";
  std::string answer="";
  command +=std::to_string(I);
  answer=inputOutput(command);
  std::cout<<"setI answer: "<<answer<<std::endl;
  if (answer=="E0")
    return true;
  else
    return false;
}


bool FUGNetzteil::setU(double U)
{
  int success=0;
  std::string command="U";
  std::string answer="";
  command +=std::to_string(U);
  answer=inputOutput(command);
  std::cout<<"setU answer: "<<answer<<std::endl;
  if (answer=="E0")
    return true;
  else
    return false;
  
}

std::string FUGNetzteil::getLastError()
{
  return lastError;
  
}

measuredValue<std::string> FUGNetzteil::getIDN()
{
  measuredValue<std::string> tmp;
  tmp.setValue(inputOutput("?"));
  return tmp;
}


measuredValue<double> FUGNetzteil::readI()
{
  std::string answer="";
   answer=inputOutput(">M1?");
      std::cout<<"getI answer: "<<answer<<std::endl;
  measuredValue<double> tmp;
  if (answer[0]=='M')
  {
   answer= answer.erase(0, 3);
   tmp.setUnit("A");
   tmp.setValueFromString(answer);
   tmp.setTimeStamp(boost::posix_time::microsec_clock::local_time());
  return tmp;
  }
  else if (answer[0]=='E')
  {
    setError("ReadI", answer);
    return tmp;
  }
  else 
    return tmp;
}

measuredValue<double> FUGNetzteil::readU()
{
  std::string answer="";

  answer=inputOutput(">M0?");
  std::cout<<"getU answer: "<<answer<<std::endl;
  measuredValue<double> tmp;
  if (answer[0]=='M')
  {
   answer= answer.erase(0, 3);
   tmp.setUnit("V");
   tmp.setValueFromString(answer);
   tmp.setTimeStamp(boost::posix_time::microsec_clock::local_time());
  return tmp;
  }
  else if (answer[0]=='E')
  {
    setError("ReadU", answer);
    return tmp;
  }
  else 
    return tmp;
  
}

measuredValue<double> FUGNetzteil::readSetpointI()
{
  int success=0;
  std::string answer="";
  answer=inputOutput(">S1?");
      std::cout<<"readSetpointI answer: "<<answer<<std::endl;
  measuredValue<double> tmp;
  if (answer[0]=='S')
  {
   answer= answer.erase(0, 3);
   tmp.setUnit("V");
   tmp.setValueFromString(answer);
   tmp.setTimeStamp(boost::posix_time::microsec_clock::local_time());
  return tmp;
  }
  else if (answer[0]=='E')
  {
    setError("ReadSetpointI", answer);
    return tmp;
  }
  else 
    return tmp;
}

measuredValue<double> FUGNetzteil::readSetpointU()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">S0?");
  measuredValue<double> tmp;
  if (answer[0]=='S')
  {
   answer= answer.erase(0, 3);
   tmp.setUnit("V");
   tmp.setValueFromString(answer);
   tmp.setTimeStamp(boost::posix_time::microsec_clock::local_time());
  return tmp;
  }
  else if (answer[0]=='E')
  {
    setError("ReadSetpointU", answer);
    return tmp;
  }
  else 
    return tmp;
  
}

measuredValue<bool> FUGNetzteil::readOvercurrent()
{
  LUGHOS_LOG_FUNCTION();
  int success=0;
  std::string answer="";

  answer=inputOutput(">D3R?");
  measuredValue<bool> tmp;
  if (answer[0]=='D')
  {
   answer= answer.erase(0, 4);
   if (std::stod(answer) == 1)
   {
    LUGHOS_LOG(log::SeverityLevel::informative) << std::string("FUG Powersupply ") << this->name << std::string(" reports overcurrent!");
    tmp.setValue(true);
    return tmp;
   }
   else
   {
     tmp.setValue(false);
     return tmp;
   }
  }
  else if (answer[0]=='E')
  {
    setError("ReadOvercurrent", answer);
    return tmp;
  }
  else 
    return tmp;
}

measuredValue<bool> FUGNetzteil::readSwitch()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">DON?");
  measuredValue<bool> tmp;
  if (answer[0]=='D')
  {
   answer= answer.erase(0, 4);
   if (std::stod(answer) == 1)
   {
    tmp.setValue(true);
    return tmp;
   }
   else
   {
     tmp.setValue(false);
     return tmp;
   }
  }
  else if (answer[0]=='E')
  {
    setError("ReadSwitch", answer);
    return tmp;
  }
  else 
    return tmp;
}

measuredValue<FUGNetzteil::OnLimit> FUGNetzteil::readLimit()
{
  bool currentLimit = readCurrentLimitation();
  bool voltageLimit = readVoltageLimitation();
  measuredValue<OnLimit> tmp;
  if(voltageLimit)
    tmp.setValue(OnLimit::Voltage);
  else if (currentLimit)
    tmp.setValue(OnLimit::Current);
  else
    tmp.setValue(OnLimit::None);
  return tmp;
}

bool FUGNetzteil::readCurrentLimitation()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">DIR?");
  if (answer[0]=='D')
  {
   answer= answer.erase(0, 4);
   if (std::stod(answer) == 1)
   {
    return true;
   }
   else
   {
     return false;
   }
  }
  else if (answer[0]=='E')
  {
    setError("ReadCurrentLimitation", answer);
    return false;
  }
  else 
    return false;
}

bool FUGNetzteil::readVoltageLimitation()
{
  int success=0;
  std::string answer="";

  answer=inputOutput(">DVR?");
  if (answer[0]=='D')
  {
   answer= answer.erase(0, 4);
   if (std::stod(answer) == 1)
   {
    return true;
   }
   else
   {
     return false;
   }
  }
  else if (answer[0]=='E')
  {
    setError("ReadCurrentLimitation", answer);
    return false;
  }
  else 
    return false;
}


measuredValue<FUGNetzteil::ControlMode> FUGNetzteil::readControlMode()
{
  ControlMode mode;
  bool analogue = readAnalogueRemote();
  bool digital = readDigitalRemote();
  if(analogue)
    mode = ControlMode::AnalogueRemote;
  else if(digital)
    mode = ControlMode::DigitalRemote;
  else
    mode = ControlMode::Local;
   measuredValue<ControlMode> tmp;
   tmp.setValue(mode);
   return tmp;
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


void FUGNetzteil::setOvercurrent(bool state)
{
  if(state)
    inputOutput(">B1 0"); //Check
  else
    inputOutput(">B1 1"); //Check
}


void FUGNetzteil::setError(std::string command, std::string error)
{
  SharedLock lock(this->mutex);
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