#include <iostream>
#include <fstream>
#include <map>
#include "test_gui.hpp"
#include "serialAsync.hpp"
#include "device.hpp"
// #include "coolpak6000.hpp"
// #include "MaxiGauge.hpp"
#include <boost/thread/thread.hpp>

typedef std::pair<std::string, boost::shared_ptr<Device> > deviceMapPair;

namespace lughos 
{

boost::asio::io_service * ioService;
std::map< std::string, boost::shared_ptr<Device> > deviceMap;

} //namespace lughos

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new lughos::mainApplication(env);
}



int main(int argc, char **argv)
{
  

  
  lughos::ioService = new boost::asio::io_service;

  boost::asio::io_service::work work(*lughos::ioService);
  boost::thread thread(boost::bind(&boost::asio::io_service::run, lughos::ioService));
  std::ofstream ofs ("/home/irina/projects/test_gui_main.txt", std::ofstream::out);

  ofs << "IOService started and running..";
    ofs.close();
  std::cout << "IOService started and running..." << std::endl;
     serialAsync connection1(lughos::ioService);
     serialAsync connection2(lughos::ioService);
     
     #ifdef WIN32 
      connection1.port_name = std::string("COM1");
      connection2.port_name = std::string("COM2");

     #else
      connection1.port_name = std::string("/dev/ttyUSB0");
      connection2.port_name = std::string("/dev/ttyS1");
    #endif

      boost::shared_ptr<Device> compressor1(new coolpak6000);
//       MaxiGauge* pressureMonitor1 = new MaxiGauge;
      
      compressor1->setName(std::string("Compressor 1"));
//       pressureMonitor1->setName(std::string("Pressure Monitor 1"));
      
      compressor1->connect(&connection1);
//       deviceMap[compressor1->getName()]=compressor1;
  deviceMap.insert(deviceMapPair(compressor1->getName(), compressor1));
  

  
  /*
   * Your main method may set up some shared resources, but should then
   * start the server application (FastCGI or httpd) that starts listening
   * for requests, and handles all of the application life cycles.
   *
   * The last argument to WRun specifies the function that will instantiate
   * new application objects. That function is executed when a new user surfs
   * to the Wt application, and after the library has negotiated browser
   * support. The function should return a newly instantiated application
   * object.
   */
  
  std::cout << "Starting Webserver" << std::endl;
  int result = Wt::WRun(argc, argv, &createApplication);



//   ofs<< "Shutting down Webserver" << std::endl;

  std::cout << "Shutting down Webserver" << std::endl;
  lughos::ioService->stop();
//    ofs<< "IOService stopping..." << std::endl;
  std::cout << "IOService stopping..." << std::endl;
  lughos::ioService->reset();
  delete lughos::ioService;
  thread.join();
  std::cout << "Everything cleaned up, quitting..." << std::endl;
//   ofs<< "Everything cleaned up, quitting..." << std::endl;

  return result;
  
}

