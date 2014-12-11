#include <iostream>
#include <fstream>
#include <map>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/QueryModel>
#include "test_gui.hpp"
#include "serialAsync.hpp"
#include "device.hpp"
#include "test_gui_monitoring.hpp"
// #include "coolpak6000.hpp"
// #include "MaxiGauge.hpp"
// #include "MaxiGauge.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/thread/thread.hpp>

#define CONFIG_FILENAME "config.xml"

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
  std::ofstream ofs ("/home/irina/projects/test_gui_main.txt", std::ofstream::out);

  ofs << "IOService started and running..";
    ofs.close();
  std::cout << "IOService started and running..." << std::endl;
  
  /* preparing configuration */
    using boost::property_tree::ptree;
    ptree config;
    
    /* Try to load configuration from file */
    
    try
    {
      boost::property_tree::read_xml(CONFIG_FILENAME, config);
    }
    catch(...) // If we cannot open the configuration, we generate a new one
    {
      std::cout << "No configuration foung! Generating default! Check if it is sane!!" << std::endl;
      
      config.put("devices.compressor1.name","Compressor 1");
      config.put("devices.compressor1.type","coolpak6000");
      config.put("devices.compressor1.connection.type","serial");
      config.put("devices.compressor1.connection.mode","async");
      config.put("devices.compressor1.connection.port","COM5");
      //
      config.put("devices.compressor2.name","Compressor 2");
      config.put("devices.compressor2.type","coolpak6000");
      config.put("devices.compressor2.connection.type","serial");
      config.put("devices.compressor2.connection.mode","async");
      config.put("devices.compressor2.connection.port","COM6");
      
      config.put("devices.pressuremonitor1.name","Pressure Monitor 1");
      config.put("devices.pressuremonitor1.type","maxigauge");
      config.put("devices.pressuremonitor1.connection.type","serial");
      config.put("devices.pressuremonitor1.connection.mode","async");
      config.put("devices.pressuremonitor1.connection.port","COM4");
      
      config.put("devices.keithley1.name","Temperature Monitor 1");
      config.put("devices.keithley1.type","keithley");
      config.put("devices.keithley1.connection.type","serial");
      config.put("devices.keithley1.connection.mode","async");
      config.put("devices.keithley1.connection.port","/dev/ttyUSB0");
      boost::property_tree::write_xml(CONFIG_FILENAME, config);
    }
  
    boost::shared_ptr<serialAsync> connection1(new serialAsync(lughos::ioService) );
    boost::shared_ptr<serialAsync> connection2(new serialAsync(lughos::ioService) );
    boost::shared_ptr<serialAsync> connection3(new serialAsync(lughos::ioService) );
    boost::shared_ptr<serialAsync> connection4(new serialAsync(lughos::ioService) );
     
    connection1->port_name = config.get<std::string>("devices.compressor1.connection.port");
    connection2->port_name = config.get<std::string>("devices.compressor2.connection.port");
    connection3->port_name = config.get<std::string>("devices.pressuremonitor1.connection.port");
    connection4->port_name = config.get<std::string>("devices.keithley1.connection.port");



      boost::shared_ptr<Device> compressor1(new coolpak6000);
      boost::shared_ptr<Device> compressor2(new coolpak6000);
      boost::shared_ptr<Device> pressureMonitor1(new MaxiGauge);
      boost::shared_ptr<Device> temperatureMonitor1(new kithleighSerial);
//       MaxiGauge* pressureMonitor1 = new MaxiGauge;
      
        

      compressor1->setName(config.get<std::string>("devices.compressor1.name"));
      compressor2->setName(config.get<std::string>("devices.compressor2.name"));
      pressureMonitor1->setName(config.get<std::string>("devices.pressuremonitor1.name"));
      temperatureMonitor1->setName(config.get<std::string>("devices.keithley1.name"));
      
      compressor1->connect(connection1);
      compressor2->connect(connection2);
      pressureMonitor1->connect(connection3);
      temperatureMonitor1->connect(connection4);

//       deviceMap[compressor1->getName()]=compressor1;
  deviceMap.insert(deviceMapPair(compressor1->getName(), compressor1));
    deviceMap.insert(deviceMapPair(compressor2->getName(), compressor2));
  std::cout<< pressureMonitor1->getName()<<std::endl;
    std::cout<< pressureMonitor1.get()<<std::endl;
  deviceMap.insert(deviceMapPair(pressureMonitor1->getName(), pressureMonitor1));
  deviceMap.insert(deviceMapPair(temperatureMonitor1->getName(), temperatureMonitor1));
  
  //Start logging
  
    dbo::backend::Sqlite3 sqlite3("test.db");
  boost::shared_ptr<dbo::Session> session(new dbo::Session);
  boost::shared_ptr<dbo::Session> session1(new dbo::Session);
  boost::shared_ptr<boost::asio::io_service> ioServiceDB(new boost::asio::io_service);
//   boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*ioService));
//   boost::thread workerThread(boost::bind(&boost::asio::io_service::run, ioService));
  session->setConnection(sqlite3);
  session->mapClass<measuredDBValue>("measuredValue");
  session1->setConnection(sqlite3);
  session1->mapClass<measuredDBValue>("measuredValue");
  try 
  {
    session->createTables();
    session1->createTables();
    std::cout << "Creating tables..." << endl;
  }
  catch(...)
  {
    std::cout << "Tables already created." << endl;
  }
  
  std::cout << "Starting task-execution" << std::endl;
  
//   PressureMonitor press(session1,taskExecutor,pressureMonitor1,1);
//   press.setEvery(boost::posix_time::seconds(1));
//   press.setExecuteTimes(Task::Execute::infinite);
//   press.start();
//   PressureMonitor press1(session1,taskExecutor,pressureMonitor1,2);
//   press1.setEvery(boost::posix_time::seconds(1));
//   press1.setExecuteTimes(Task::Execute::infinite);
//   press1.start();
//   PressureMonitor press2(session1,taskExecutor,pressureMonitor1,3);
//   press2.setEvery(boost::posix_time::seconds(1));
//   press2.setExecuteTimes(Task::Execute::infinite);
//   press2.start();
  
  KeithleyTest keithley(session, taskExecutor,temperatureMonitor1);
  keithley.setEvery(boost::posix_time::seconds(10));
  keithley.setExecuteTimes(Task::Execute::infinite);
  keithley.start();
  
  
  
  
  
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

