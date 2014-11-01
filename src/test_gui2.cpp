#include <iostream>
#include <fstream>
#include <map>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/QueryModel>
#include "test_gui2.hpp"
#include "serialAsync.hpp"
#include "device.hpp"
#include "bronkhorst.hpp"
#include "test_gui_monitoring2.hpp"
#include "RFG.hpp"
// #include "MaxiGauge.hpp"
// #include "MaxiGauge.hpp"
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
  boost::shared_ptr<boost::asio::io_service> taskExecutor(new boost::asio::io_service);

  boost::asio::io_service::work work(*lughos::ioService);
  boost::asio::io_service::work work2(*taskExecutor);
  boost::thread thread(boost::bind(&boost::asio::io_service::run, lughos::ioService));
  boost::thread thread2(boost::bind(&boost::asio::io_service::run, taskExecutor));


  std::cout << "IOService started and running..." << std::endl;
  
    boost::shared_ptr<serialAsync> connection1(new serialAsync(lughos::ioService) );
    boost::shared_ptr<serialAsync> connection2(new serialAsync(lughos::ioService) );
//     boost::shared_ptr<serialAsync> connection3(new serialAsync(lughos::ioService) );
     
     #ifdef WIN32 
      connection1->port_name = std::string("COM1");
      connection2->port_name = std::string("COM2");
//       connection3->port_name = std::string("COM3");

     #else
      connection1->port_name = std::string("/dev/ttyUSB0");
      connection2->port_name = std::string("/dev/ttyUSB1");
//       connection3->port_name = std::string("/dev/ttyUSB0");
    #endif
      
      

      boost::shared_ptr<Device> flowcontroll1(new bronkhorst);
      boost::shared_ptr<Device> RFG1(new RFG);
// // //       boost::shared_ptr<Device> temperatureMonitor1(new kithleighSerial);
//       MaxiGauge* pressureMonitor1 = new MaxiGauge;
      
        
      flowcontroll1->setName(std::string("Flow Controll 1"));
      RFG1->setName(std::string("RFG 1"));
//       temperatureMonitor1->setName(std::string("Temperature Monitor 1"));
      
      flowcontroll1->connect(connection1);
      RFG1->connect(connection2);
//       temperatureMonitor1->connect(connection3);
//       deviceMap[compressor1->getName()]=compressor1;
  deviceMap.insert(deviceMapPair(flowcontroll1->getName(), flowcontroll1));
//   std::cout<< pressureMonitor1->getName()<<std::endl;
//     std::cout<< pressureMonitor1.get()<<std::endl;
  deviceMap.insert(deviceMapPair(RFG1->getName(), RFG1));
//   deviceMap.insert(deviceMapPair(temperatureMonitor1->getName(), temperatureMonitor1));
  
  
      dbo::backend::Sqlite3 sqlite3("test.db");
  boost::shared_ptr<dbo::Session> session(new dbo::Session);
  boost::shared_ptr<dbo::Session> session1(new dbo::Session);
  boost::shared_ptr<boost::asio::io_service> ioServiceDB(new boost::asio::io_service);
//   boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*ioService));
  boost::thread workerThread(boost::bind(&boost::asio::io_service::run, ioService));
  session->setConnection(sqlite3);
  session->mapClass<measuredDBValue>("measuredValue");
  session1->setConnection(sqlite3);
  session1->mapClass<measuredDBValue>("measuredValue");
  try 
  {
    session->createTables();
//     session1->createTables();
    std::cout << "Creating tables..." << endl;
  }
  catch(...)
  {
    std::cout << "Tables already created." << endl;
  }
  
  std::cout << "Starting task-execution" << std::endl;
  
  RFGTest rfg(session1,taskExecutor,RFG1,1);
  rfg.setEvery(boost::posix_time::seconds(5));
  rfg.setExecuteTimes(Task::Execute::infinite);
  rfg.start();
  
  BronkhorstTest horst(session, taskExecutor,flowcontroll1);
  horst.setEvery(boost::posix_time::seconds(10));
  horst.setExecuteTimes(Task::Execute::infinite);
  horst.start();
  
  
  
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

