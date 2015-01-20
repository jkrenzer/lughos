#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <Wt/Dbo/Dbo>
#include <string>
#include <sstream>
#include <boost/current_function.hpp>

namespace lughos 
{
  
  class debugLogEntry
  {
  public:
    std::string functionName;
    std::string fileName;
    long int lineNumber;
    std::string message;
    
    debugLogEntry(std::string functionName, std::string fileName, long int lineNumber, std::string message) : functionName(functionName), fileName(fileName), lineNumber(lineNumber), message(message)
    {
      
    }
    
    std::String toString()
    {
      std::stringstream ss;
      ss << this->functionName << this->fileName << this->lineNumber << this->message << std::endl;
      return
    }
    
    
  };
  
  void debugLog(std::string message)
  {
    
  }
  
  void debugLogImpl(std::string functionName, std::string fileName, long int lineNumber, std::string message)
  {
    
  }
  
} //namespace lughos

#endif