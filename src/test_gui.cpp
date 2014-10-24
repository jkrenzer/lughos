#include <iostream>
#include <fstream>
#include <map>
#include "test_gui.hpp"
#include "serialAsync.hpp"
#include "device.hpp"
// #include "coolpak6000.hpp"
#include "MaxiGauge.hpp"
#include <boost/thread/thread.hpp>

typedef std::pair<std::string, boost::shared_ptr<Device> > deviceMapPair;

namespace lughos 
{
boost::shared_ptr<boost::asio::io_service> ioService;
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
  

  
  lughos::ioService= boost::shared_ptr<boost::asio::io_service>(new boost::asio::io_service);

  boost::asio::io_service::work work(*lughos::ioService);
  boost::thread thread(boost::bind(&boost::asio::io_service::run, lughos::ioService));
  std::ofstream ofs ("/home/irina/projects/test_gui_main.txt", std::ofstream::out);

  ofs << "IOService started and running..";
    ofs.close();
  std::cout << "IOService started and running..." << std::endl;
  
    boost::shared_ptr<serialAsync> connection1(new serialAsync(lughos::ioService) );
    boost::shared_ptr<serialAsync> connection2(new serialAsync(lughos::ioService) );
     
     #ifdef WIN32 
      connection1.port_name = std::string("COM1");
      connection2.port_name = std::string("COM2");

     #else
      connection1->port_name = std::string("/dev/ttyUSB0");
      connection2->port_name = std::string("/dev/ttyUSB1");
    #endif

      boost::shared_ptr<Device> compressor1(new coolpak6000);
      boost::shared_ptr<Device> pressureMonitor1(new MaxiGauge);
//       MaxiGauge* pressureMonitor1 = new MaxiGauge;
      
      compressor1->setName(std::string("Compressor 1"));
      pressureMonitor1->setName(std::string("Pressure Monitor 1"));
      
      compressor1->connect(connection1);
      pressureMonitor1->connect(connection2);
//       deviceMap[compressor1->getName()]=compressor1;
  deviceMap.insert(deviceMapPair(compressor1->getName(), compressor1));
  std::cout<< pressureMonitor1->getName()<<std::endl;
    std::cout<< pressureMonitor1.get()<<std::endl;
  deviceMap.insert(deviceMapPair(pressureMonitor1->getName(), pressureMonitor1));

  
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
//   char* arg1=new char;
//   arg1="--docroot=./";
//   char* arg2 = new char;
//   arg2="--http-address=127.0.0.1";
  
  
//   char* myargv[3];
// 
//   myargv[0]=argv[0];
//   myargv[1] = new char [100];
//   strcpy(myargv[1],"--docroot=./");
//   myargv[2] = new char [100];
//   strcpy(myargv[2],"--http-address=127.0.0.1");
// 
//   std::printf("argv[1] = %s", myargv[1]);
//   std::printf("argv[2] = %s", myargv[2]);
//   
  int result = Wt::WRun(argc, argv, &createApplication);
//   int result = Wt::WRun(3, myargv, &createApplication);

   

//   ofs<< "Shutting down Webserver" << std::endl;

  std::cout << "Shutting down Webserver" << std::endl;
  lughos::ioService->stop();
//    ofs<< "IOService stopping..." << std::endl;
  std::cout << "IOService stopping..." << std::endl;
  lughos::ioService->reset();
  thread.join();
  std::cout << "Everything cleaned up, quitting..." << std::endl;
//   ofs<< "Everything cleaned up, quitting..." << std::endl;

  return result;
  
}

