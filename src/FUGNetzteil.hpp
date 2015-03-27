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
    
    private:
	  FUGNetzteil(const FUGNetzteil &p);
	  FUGNetzteil &operator=(const FUGNetzteil &p);

	  void initImplementation();
	  void shutdownImplementation();
	  bool isConnectedImplementation();
	  
	  measuredValue<double> storedMeasure;
	  int voltagesOnOf=3;
	  std::string serverName;
	  
	  double readI();
	  double readU();
	  bool readOvercurrent();
	  double readSetpointU();
	  double readSetpointI();
	  bool readCurrentLimitation();
	  bool readVoltageLimitation();
	  bool readDigitalRemote();
	  bool readAnalogueRemote();
	  bool readLocalControl();

	  
    public:
	  FUGNetzteil(void);
	  virtual ~FUGNetzteil(void);
	  measuredValue<double> getMeasure(bool force=false);	
	  int setI(double I );
	  int setU(double I );
	  cachedFunction<double> getSetpointU;
	  cachedFunction<double> getSetpointI;
	  cachedFunction<double> getI;
	  cachedFunction<double> getU;
	  cachedFunction<bool> getOvercurrent;
	  cachedFunction<bool> getCurrentLimitation;
	  cachedFunction<bool> getVoltageLimitation;
	  cachedFunction<bool> getDigitalRemote;
	  cachedFunction<bool> getAnalogueRemote;
	  cachedFunction<bool> getLocalControl;
	  void resetOvercurrent();
	  std::string getLastError();
	  std::string getIDN();
	  int switchVoltage(int onof );

  protected:
	  std::string lastError;
	  void setError(std::string command, std::string error);
	  std::string interpretAnswer(std::string query);
	  std::string composeRequest(std::string query);
	  
  };
#endif
