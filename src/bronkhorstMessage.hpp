#ifndef BRONKHORST_MESSAGE_HPP
#define BRONKHORST_MESSAGE_HPP
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <bitset>
#include <boost/regex.hpp>
#include "log.hpp"
#include "threadSafety.hpp"

namespace lughos
{
  class bronkhorstMessage
  {
  private:
    mutable Mutex mutex;
  public:
  
    enum Type {Status = 0, SendParamWithAnswer = 1, SendParamNoAnswer = 2, SendParamWithSourceAdr = 3, RequestParam = 4, Instruction = 5, StopProcess = 6, StartProcess = 7, ClaimProcess = 8, UnclaimProcess = 9};
    enum ParameterType {Character = '\x00', Integer = '\x20', Float = '\x40', Long = '\x40', String = '\x60'   };
    enum Parameter {Measure = 0, Setpoint = 1, ControlMode = 4, Capacity = 13};
    enum StatusMessage {NoError = '\x00', ProcessClaimed = '\x01', CommandError = '\x02', ProcessError = '\x03', ParameterError = '\x04', ParameterTypeError = '\x05', ParameterValueError = '\x06', NetworkNotAvailible = '\x07', TimeoutStartChar = '\x08', TimeOutSerialLine = '\x09', HardwareMemoryError = '\x0a', NodeNumberError = '\x0b', GeneralCommunicationError = '\x0c', ReadOnlyParameter = '\x0d', ErrorPcCommunication = '\x0e', NoRs232Connection = '\x0f', PcOutOfMemory = '\x10', WriteOnlyParameter = '\x11', SystemConfigurationUnknown = '\x12', NoFreeNodeAddress = '\x13', WrongInterfaceType = '\x14', ErrorSerialPortConnection = '\x15', ErrorOpeningCommunication = '\x16', CommunicationError = '\x17', ErrorInterfaceBusMaster = '\x18', TimeOutAnswer = '\x19', NoStartChar = '\x1a', ErrorFirstDigit = '\x1b', BufferOverflowInHost = '\x1c', BufferOverflow = '\x1d', NoAnswerFound = '\x1e', ErrorClosingCommunication = '\x1f', SynchronisationError = '\x20', SendError = '\x21', ProtocolError = '\x22', BufferOverflowInModule = '\x23'};
  
  protected:
    unsigned int type;
    unsigned int node;
    unsigned int parameterType;
    unsigned int process;
    unsigned int parameter;
    unsigned int expectedStringLength;
    bool processChained;
    bool parameterChained;
    bool isStatus;
    std::string message;
    std::string hexValue;
    std::string charValue;
    StatusMessage statusCode;
    unsigned int statusSubjectFirstByte;
    
  public:
  
    
    
    bronkhorstMessage()
    {
      this->processChained = false;
      this->parameterChained = false;
      this->isStatus = false;
      this->type = 0;
      this->node = 0;
      this->parameterType = 0;
      this->process = 0;
      this->parameter = 0;
      this->expectedStringLength = 0;
      this->statusCode = StatusMessage::NoError;
      this->statusSubjectFirstByte = 0;
      this->message.clear();
      this->hexValue.clear();
      this->charValue.clear();
    }
    
    bronkhorstMessage(std::string s)
    {
      this->processChained = false;
      this->parameterChained = false;
      this->isStatus = false;
      this->type = 0;
      this->node = 0;
      this->parameterType = 0;
      this->process = 0;
      this->parameter = 0;
      this->expectedStringLength = 0;
      this->statusCode = StatusMessage::NoError;
      this->statusSubjectFirstByte = 0;
      this->message.clear();
      this->hexValue.clear();
      this->charValue.clear();
      this->fromString(s);
    }
    
    operator std::string() const
    {
      return this->toString();
    }
    
    void operator()(std::string s)
    {
      this->fromString(s);
    }
    
    unsigned int getType() const
    {
      SharedLock lock(this->mutex);
      return this->type;
    }
    
    void setType(int type)
    {
      ExclusiveLock lock(this->mutex);
      if(type > 0 && type < 10)
	this->type = type;
    }
    
    unsigned int getParameterType() const
    {
      SharedLock lock(this->mutex);
      return this->parameterType;
    }
    
    void setParameterType(int varType)
    {
      ExclusiveLock lock(this->mutex);
      if (varType == '\x00' || varType == '\x20' || varType == '\x40' || varType == '\x60' )
	this->parameterType = varType;
    }
    
    unsigned int getNode() const
    {
      SharedLock lock(this->mutex);
      return this->node;
    }
    
    void setNode(int node)
    {
      ExclusiveLock lock(this->mutex);
      this->node = node;
    }
    
    unsigned int getProcess() const
    {
      SharedLock lock(this->mutex);
      return this->getProcess();
    }
    
    void setProcess(int process)
    {
      ExclusiveLock lock(this->mutex);
      this->process = process;
    }
    
    unsigned int getParameter() const
    {
      SharedLock lock(this->mutex);
      return this->parameter;
    }
    
    void setParameter(int parameter)
    {
      ExclusiveLock lock(this->mutex);
      if (parameter > 0 && parameter < 32 )
	this->parameter = parameter;
    }
    
    bool getProcessChained() const
    {
      SharedLock lock(this->mutex);
      return this->processChained;
    }
    
    void setProcessChained(bool processChained)
    {
      ExclusiveLock lock(this->mutex);
      this->processChained = processChained;
    }
    
    bool getParameterChained() const
    {
      SharedLock lock(this->mutex);
      return this->parameterChained;
    }
    
    void setParameterChained(bool parameterChained)
    {
      ExclusiveLock lock(this->mutex);
      this->parameterChained = parameterChained;
    }
    
    unsigned int getlength() const
    {
      SharedLock lock(this->mutex);
      return (this->message.size() - 2)/2;
    }
    
    unsigned int getParameterByte(bool ommitChained = false) const
    {
      SharedLock lock(this->mutex);
      std::bitset<8> bs(this->parameter);
      if(this->parameterChained && !ommitChained)
	bs[7] = true;
      else
	bs[7] = false;
      std::cout << "Calculated PARAMETERBYTE: " << bs.to_ulong() + this->parameterType << " = " << bs.to_ulong() << " + " << this->parameterType << " = " << std::hex << bs.to_ulong() + this->parameterType << std::endl;
      return bs.to_ulong() + this->parameterType;
    }
    
    void setParameterByte(int parameterType)
    {
      ExclusiveLock lock(this->mutex);
      std::bitset<8> bs(parameterType);
      this->processChained = bs[7];
      bs[7] = false;
      std::bitset<8> tbs = bs.to_ulong() & 96;
      std::bitset<8> pbs = bs.to_ulong() & 31;
      this->parameterType = tbs.to_ulong();
      this->parameter = pbs.to_ulong();
      std::cout << "Set PARAMETERBYTE: " << parameterType << " - " << bs.to_ulong() << " - " << tbs.to_ulong() << " - " << pbs.to_ulong() << std::endl;
    }
    
    unsigned int getProcessByte(bool ommitChained = false) const
    {
      SharedLock lock(this->mutex);
      std::bitset<8> bs(this->process);
      if(this->processChained && !ommitChained)
	bs[7] = true;
      else
	bs[7] = false;
      return bs.to_ulong();
    }
    
    void setProcessByte(int processType)
    {
      ExclusiveLock lock(this->mutex);
      std::bitset<8> bs(processType);
      this->processChained = bs[7];
      bs[7] = false;
      this->process = bs.to_ulong();
      std::cout << "PROCESSBYTE: " << processType << " - " << bs.to_ulong() << std::endl;
    }
    
    unsigned int getExpectedStringLength() const
    {
      SharedLock lock(this->mutex);
      return this->expectedStringLength;
    }
    
    void setExpectedStringLength(int expectedStringLength)
    {
      ExclusiveLock lock(this->mutex);
      if(expectedStringLength > 1)
	this->expectedStringLength = expectedStringLength;
    }
    
    unsigned int getStatusSubjectFirstByte() const
    {
      SharedLock lock(this->mutex);
      if(this->statusSubjectFirstByte > 0)
        return statusSubjectFirstByte;
    }
    
    bool isEmpty() const
    {
      SharedLock lock(this->mutex);
      return this->message.empty();
    }
    
    bool hasValue() const
    {
      SharedLock lock(this->mutex);
      return !this->hexValue.empty();
    }
    
    bool isStatusMessage() const
    {
      SharedLock lock(this->mutex);
      return this->isStatus;
    }
    
    bool isErrorMessage() const
    {
      SharedLock lock(this->mutex);
      return this->isStatus && this->statusCode != '\x00';
    }
    
     StatusMessage getStatusCode() const
     {
       SharedLock lock(this->mutex);
       return this->statusCode;
     }
     
     std::string getStatusCodeAsString() const
     {
        SharedLock lock(this->mutex);
	switch(this->statusCode)
	{
	  case StatusMessage::NoError: return std::string("No error"); break;
	  case StatusMessage::BufferOverflow: return std::string("Buffer overflow"); break;
	  case StatusMessage::BufferOverflowInHost: return std::string("Buffer overflow in Host"); break;
	  case StatusMessage::BufferOverflowInModule: return std::string("Buffer overflow in module"); break;
	  case StatusMessage::CommandError: return std::string("Command error"); break;
	  case StatusMessage::CommunicationError: return std::string("Communication error"); break;
	  case StatusMessage::ErrorClosingCommunication: return std::string("Error closing communication"); break;
	  case StatusMessage::ErrorFirstDigit: return std::string("Error first digit"); break;
	  case StatusMessage::ErrorInterfaceBusMaster: return std::string("Error interface bus master"); break;
	  case StatusMessage::ErrorOpeningCommunication: return std::string("Error opening communication"); break;
	  case StatusMessage::ErrorPcCommunication: return std::string("Error PC communication"); break;
	  case StatusMessage::ErrorSerialPortConnection: return std::string("Error serial port connection"); break;
	  case StatusMessage::GeneralCommunicationError: return std::string("Serial communication error"); break;
	  case StatusMessage::HardwareMemoryError: return std::string("Hardware memory error"); break;
	  case StatusMessage::NetworkNotAvailible: return std::string("Network not availible"); break;
	  case StatusMessage::NoAnswerFound: return std::string("No answer found"); break;
	  case StatusMessage::NodeNumberError: return std::string("Node number Error"); break;
	  case StatusMessage::NoFreeNodeAddress: return std::string("No free node address"); break;
	  case StatusMessage::NoRs232Connection: return std::string("No RS232 connection"); break;
	  case StatusMessage::NoStartChar: return std::string("No start character"); break;
	  case StatusMessage::ParameterError: return std::string("Parameter error"); break;
	  case StatusMessage::ParameterTypeError: return std::string("Parameter type error"); break;
	  case StatusMessage::ParameterValueError: return std::string("Parameter value error"); break;
	  case StatusMessage::PcOutOfMemory: return std::string("PC out of memory"); break;
	  case StatusMessage::ProcessClaimed: return std::string("Process claimed"); break;
	  case StatusMessage::ProcessError: return std::string("Process error"); break;
	  case StatusMessage::ProtocolError: return std::string("Protocol error"); break;
	  case StatusMessage::ReadOnlyParameter: return std::string("Read only parameter"); break;
	  case StatusMessage::SendError: return std::string("Send error"); break;
	  case StatusMessage::SynchronisationError: return std::string("Synchronisation error"); break;
	  case StatusMessage::SystemConfigurationUnknown: return std::string("System configuration unknown"); break;
	  case StatusMessage::TimeOutAnswer: return std::string("Timeout answer"); break;
	  case StatusMessage::TimeOutSerialLine: return std::string("Timeout serial line"); break;
	  case StatusMessage::TimeoutStartChar: return std::string("Timeout start character"); break;
	  case StatusMessage::WriteOnlyParameter: return std::string("Write only parameter"); break;
	  case StatusMessage::WrongInterfaceType: return std::string("Wrong interface type"); break;
	  default: return std::string("Unknown error!"); break;
	  
	}
     }
     
     std::string getValueString() const
     {
        SharedLock lock(this->mutex);
	if(this->parameterType == ParameterType::String)
	  return charValue;
	std::stringstream returnStream;
	std::cout << "getValueString HEXVALUE: " << this->hexValue << std::endl;
	long unsigned int value = 0;
	std::stringstream(this->hexValue) >> std::hex >> value;
	std::cout << "getValueString VALUE: " << value << std::endl;
	switch(this->parameterType)
	{
	  case ParameterType::Character : char c; memcpy(&c,&value,sizeof(c)); returnStream << c; break;
	  case ParameterType::Float : float f; memcpy(&f,&value,sizeof(f)); returnStream << f; break;
	  case ParameterType::Integer : uint16_t i; memcpy(&i,&value,sizeof(i)); returnStream << i; break;
	  default: return std::string("ERROR");
	}
	std::cout << "getValueString STRING: " << returnStream.str() << std::endl;
	return returnStream.str();
     }
     
     template <class T> void setValueString(T newValue)
     {
       ExclusiveLock lock(this->mutex);
       std::stringstream returnStream;
       long unsigned int value = 0;
       
	switch(this->parameterType)
	{
	  case ParameterType::Character : memcpy(&value,&newValue,sizeof(char)); returnStream << std::hex << std::setw(sizeof(char)) << std::setfill('0') << value;  break;
	  case ParameterType::Float : memcpy(&value,&newValue,sizeof(float)); returnStream << std::hex << std::setw(sizeof(float)) << std::setfill('0') << value;  break;
	  case ParameterType::Integer : memcpy(&value,&newValue,sizeof(int)); returnStream << std::hex << std::setw(sizeof(int)) << std::setfill('0') << value;  break;
	  case ParameterType::String : std::stringstream ss; ss << newValue; std::string s = ss.str();
					for (std::string::const_iterator it = s.begin(); it != s.end(); it++)
					{
					  returnStream << std::setw(2) << std::setfill('0') << std::hex  << (int) *it;
					}
					break;
	}
	this->hexValue = returnStream.str();
	std::cout << "setValueString HEXVALUE: " << this->hexValue;
     }
    
    std::string toString() const
    {
      SharedLock lock(this->mutex);
      std::stringstream ss("");
      switch (this->type) 
      {
	case 0: ss << std::hex << std::setw(2) << std::setfill('0') << this->node << std::setw(2) << this->type << std::setw(2) << this->statusCode << std::setw(2) << this->statusSubjectFirstByte ; break;
	case 1:
	case 2: ss  << std::hex  << std::setw(2) << std::setfill('0') << this->node << std::setw(2) << this->type << std::setw(2) << this->getProcessByte() << std::setw(2) << this->getParameterByte() << std::setw(2) << this->hexValue; break;
	case 4: ss  << std::hex  << std::setw(2) << std::setfill('0') <<  this->node << std::setw(2) << this->type << std::setw(2) << this->getProcessByte() << std::setw(2) << this->getParameterByte() << std::setw(2) << this->getProcessByte(true) << std::setw(2) << this->getParameterByte(true);
		if(this->type == bronkhorstMessage::ParameterType::String) 
		  ss << std::hex << std::setw(2) << std::setfill('0') << this->expectedStringLength;
		break;
	default:	return std::string("ERROR!"); break;
      }
      std::string s = ss.str();
      ss.str("");
      ss << std::setw(1) << ":" << std::hex << std::setw(2) << std::setfill('0') << ( s.size() / 2) << s << "\r\n";
      return ss.str();
    }
    
    void fromString(std::string message)
    {
      ExclusiveLock lock(this->mutex);
      boost::regex exp1(":(([a-fA-F0-9]{2})*)\r");
      boost::cmatch res1;
      if(!boost::regex_search(message.c_str(), res1, exp1))
	return;
      else
	message = res1[1];
      
      boost::regex exp2("([a-fA-F0-9]{2})");
      boost::sregex_token_iterator nomatchIt;
      std::vector<std::string> res2;
      for (boost::sregex_token_iterator matchIt(message.begin(), message.end(), exp2, 1); matchIt != nomatchIt; matchIt++)
      {
	res2.push_back(*matchIt);
      }
      std::cout << "GOT MATCHES: " << res2.size() << " - " << res2[0] << " - " << res2[1] << " - " << res2[2] << " - " << res1[1] << std::endl;
      if(res2.size() < 5)
      {
	std::cout << "Bronkhorst message corrupt or of unknown type!" << std::endl;
	return;
      }
      this->message = res2[0];
      std::stringstream(res2[1]) >> std::hex >> this->node;
      std::stringstream(res2[2]) >> std::hex >> this->type;
      switch(this->type)
      {
	case 0: this->isStatus = true;
		unsigned int tmp;
		std::stringstream(res2[3]) >> std::hex >> tmp;
		this->statusCode = (StatusMessage) tmp;
		std::stringstream(res2[4]) >> std::hex >> this->statusSubjectFirstByte;
		break;
	case 2: unsigned int byte;
		this->isStatus = false;
		lock.unlock();
		std::stringstream(res2[3]) >> std::hex >> byte; setProcessByte(byte);
		std::stringstream(res2[4]) >> std::hex >> byte; setParameterByte(byte);
		lock.lock();
		char c = '\x00';
		for(std::vector<std::string>::const_iterator it = res2.begin() + 5; it != res2.end(); it++)
		{
		  this->hexValue += *it;
		  std::stringstream(*it) >> std::hex >> c; 
		  this->charValue += c;
		}
		break;
      }
    }
  };
  
}//namespace lughos

#endif