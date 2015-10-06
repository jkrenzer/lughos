#ifndef FUG_HPP
#define FUG_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp> 

#include <string>
#include <vector>
#include <cstring>
#include <boost/asio.hpp>
#include "connectionImpl.hpp"
#include "tcpConnections.hpp"
#include <iostream>
#include <boost/array.hpp>
#include "device.hpp"
#include "httpDict.hpp"
#include "measuredValue.hpp"
#include "cachedFunction.hpp"
#include "exposedMeasuredValues.hpp"

using boost::asio::ip::tcp;

using namespace lughos;

class FUGNetzteilConnection : public tcpConnection
{
public:
    FUGNetzteilConnection(boost::shared_ptr< boost::asio::io_service > io_service);
    FUGNetzteilConnection();
};

  class FUGNetzteil : public Device
  {    
    friend cachedFunction<double>;
    friend cachedFunction<bool>;
    
    public:
    
    	  enum class OnLimit {None, Current, Voltage};
	  enum class ControlMode {Local, AnalogueRemote, DigitalRemote};
    
    private:
	  FUGNetzteil(const FUGNetzteil &p);
	  FUGNetzteil &operator=(const FUGNetzteil &p);

	  void initImplementation();
	  void shutdownImplementation();
	  bool isConnectedImplementation();
	  
	  measuredValue<double> storedMeasure;
	  int voltagesOnOf=3;
	  std::string serverName;
	  
	  measuredValue<double> readI();
	  measuredValue<double> readU();
	  measuredValue<bool> readOvercurrent();
	  measuredValue<bool> readSwitch();
	  measuredValue<double> readSetpointU();
	  measuredValue<double> readSetpointI();
	  measuredValue<ControlMode> readControlMode();
	  measuredValue<OnLimit> readLimit();
	  
	  bool readCurrentLimitation();
	  bool readVoltageLimitation();
	  bool readDigitalRemote();
	  bool readAnalogueRemote();
	  bool readLocalControl();

	  
    public:
	  FUGNetzteil(void);
	  virtual ~FUGNetzteil(void);
	  
	  exposedMeasurement<double> targetVoltage;
	  exposedMeasurement<double> targetCurrent;
	  exposedMeasurement<double> voltage;
	  exposedMeasurement<double> current;
	  exposedMeasurement<bool> overcurrent;
	  exposedMeasurement<bool> outputState;
	  exposedMeasurement<OnLimit> limit;
	  exposedMeasurement<ControlMode> controlMode;
	  exposedMeasurement<std::string> model;
	  
	  
    protected:
    
	  void memberDeclaration();
   
	  measuredValue<double> getMeasure(bool force=false);	
	  bool setI(double I );
	  bool setU(double U );
	  void setOvercurrent(bool state);
	  std::string getLastError();
	  measuredValue<std::string> getIDN();
	  bool switchVoltage(bool state );

  protected:
	  std::string lastError;
	  void setError(std::string command, std::string error);
	  std::string interpretAnswer(std::string query);
	  std::string composeRequest(std::string query);
	  
  };
#endif
