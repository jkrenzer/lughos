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
#include "tcpAsync.hpp"

#include <iostream>
#include <boost/array.hpp>
#include "device.hpp"
#include "httpDict.hpp"
#include "measuredValue.hpp"

using boost::asio::ip::tcp;

using namespace lughos;


  class FUGNetzteil : public Device
  {    
    private:
	  FUGNetzteil(const FUGNetzteil &p);
	  FUGNetzteil &operator=(const FUGNetzteil &p);
	  
	  template <class T> void setDefaultImpl(T& connection);
	  virtual void set_default();
	  void initImplementation();
	  void shutdownImplementation();
	  bool isConnectedImplementation();

	  measuredValue storedMeasure;
	  int switchVoltage(int onof );


	  int voltagesOnOf=3;
	  std::string serverName;
	  
    public:
	  FUGNetzteil(void);
	  ~FUGNetzteil(void);
	  measuredValue getMeasure(bool force=false);	
	  int setI(double I );
	  int setU(double I );
	  double getSetpointI();
	  double getSetpointU();
	  double getI();
	  double getU();
	  bool hasOvercurrent();
	  bool currentLimitation();
	  bool voltageLimitation();
	  void resetOvercurrent();
	  std::string getLastError();
	  std::string getIDN();

  protected:
	  std::string lastError;
	  void setError(std::string command, std::string error);
	  std::string interpretAnswer(std::string query);
	  std::string composeRequest(std::string query);
	  
  };
#endif
