#include <iostream>
#include <fstream>
#include <map>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/QueryModel>
#include "test_gui2.hpp"
#include "device.hpp"
#include "bronkhorst.hpp"
#include "FUGNetzteil.hpp"
#include "PSAPowersupply.hpp"
// #include "test_gui_monitoring2.hpp"
#include "RFG.hpp"
#include "Relais.hpp"
#include <boost/thread/thread.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#if WIN32
#include <windows.h> // for EXCEPTION_ACCESS_VIOLATION
#include <excpt.h>
#endif

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

boost::shared_ptr<Device> makeInstance(std::string typeIdentifier)
{
    if (typeIdentifier == std::string("rfg"))
        return boost::shared_ptr<Device>(new RFG);
    else if (typeIdentifier == std::string("bronkhorst"))
        return boost::shared_ptr<Device>(new bronkhorst);
    else if (typeIdentifier == std::string("relais"))
        return boost::shared_ptr<Device>(new Relais);
    else if (typeIdentifier == std::string("fug"))
        return boost::shared_ptr<Device>(new FUGNetzteil);
}

int main(int argc, char **argv)
{
    try {

        lughos::ioService= boost::shared_ptr<boost::asio::io_service>(new boost::asio::io_service);
        boost::shared_ptr<boost::asio::io_service> taskExecutor(new boost::asio::io_service);

        boost::asio::io_service::work work(*lughos::ioService);
        boost::asio::io_service::work work2(*taskExecutor);
        boost::thread thread(boost::bind(&boost::asio::io_service::run, lughos::ioService));
        boost::thread thread2(boost::bind(&boost::asio::io_service::run, taskExecutor));


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
            config.put("devices.flowcontroll1.name","Flow Controll 1");
            config.put("devices.flowcontroll1.type","bronkhorst");
            config.put("devices.flowcontroll1.connection.type","serial");
            config.put("devices.flowcontroll1.connection.mode","async");
            config.put("devices.flowcontroll1.connection.port","/dev/ttyMUE10");

            config.put("devices.flowcontroll2.name","Flow Controll 2");
            config.put("devices.flowcontroll2.type","bronkhorst");
            config.put("devices.flowcontroll2.connection.type","serial");
            config.put("devices.flowcontroll2.connection.mode","async");
            config.put("devices.flowcontroll2.connection.port","/dev/ttyMUE11");
            //
            config.put("devices.rfg1.name","RFG 1");
            config.put("devices.rfg1.type","RFG");
            config.put("devices.rfg1.connection.type","serial");
            config.put("devices.rfg1.connection.mode","async");
            config.put("devices.rfg1.connection.port","/dev/ttyMUE8");

            config.put("devices.relais1.name","Relais 1");
            config.put("devices.relais1.type","relais");
            config.put("devices.relais1.connection.type","serial");
            config.put("devices.relais1.connection.mode","async");
            config.put("devices.relais1.connection.port","/dev/ttyMUE3");
	    
	    config.put("devices.fug1.name","FUGNetzteil 1");
            config.put("devices.fug1.type","fug");
            config.put("devices.fug1.connection.type","tcp");
            config.put("devices.fug1.connection.mode","async");
	    config.put("devices.fug1.connection.server","192.168.178.46");
            config.put("devices.fug1.connection.port","2101");
	    
	    config.put("devices.fug2.name","FUGNetzteil 2");
            config.put("devices.fug2.type","fug");
            config.put("devices.fug2.connection.type","tcp");
            config.put("devices.fug2.connection.mode","async");
	    config.put("devices.fug2.connection.server","192.168.178.47");
            config.put("devices.fug2.connection.port","2101");
	    
	    config.put("devices.psa1.name","PSA 1");
            config.put("devices.psa1.type","psa-powersupply");
            config.put("devices.psa1.connection.type","serial");
            config.put("devices.psa1.connection.mode","async");
            config.put("devices.psa1.connection.port","/dev/ttyMUE0");
	    
	    config.put("devices.psa2.name","PSA 2");
            config.put("devices.psa2.type","psa-powersupply");
            config.put("devices.psa2.connection.type","serial");
            config.put("devices.psa2.connection.mode","async");
            config.put("devices.psa2.connection.port","/dev/ttyMUE1");
	    
	    config.put("devices.psa3.name","PSA 3");
            config.put("devices.psa3.type","psa-powersupply");
            config.put("devices.psa3.connection.type","serial");
            config.put("devices.psa3.connection.mode","async");
            config.put("devices.psa3.connection.port","/dev/ttyMUE2");
            boost::property_tree::write_xml(CONFIG_FILENAME, config);
        }

        //TODO Make a loop which iterates over declared devices

        boost::shared_ptr<serialConnection> connection1(new bronkhorstConnection() );
        boost::shared_ptr<serialConnection> connection2(new RFGConnection() );
        boost::shared_ptr<serialConnection> connection3(new RelaisConnection() );
        boost::shared_ptr<serialConnection> connection4(new bronkhorstConnection() );
        boost::shared_ptr<tcpConnection> connection5(new FUGNetzteilConnection() );
        boost::shared_ptr<tcpConnection> connection6(new FUGNetzteilConnection() );
	boost::shared_ptr<serialConnection> connection7(new PSAPowersupplyConnection() );
	boost::shared_ptr<serialConnection> connection8(new PSAPowersupplyConnection() );
	boost::shared_ptr<serialConnection> connection9(new PSAPowersupplyConnection() );


        connection1->port_name = std::string(config.get<std::string>("devices.flowcontroll1.connection.port"));
        connection2->port_name = std::string(config.get<std::string>("devices.rfg1.connection.port"));
        connection3->port_name = std::string(config.get<std::string>("devices.relais1.connection.port"));
        connection4->port_name = std::string(config.get<std::string>("devices.flowcontroll2.connection.port"));
        connection5->port_name = std::string(config.get<std::string>("devices.fug1.connection.port"));
	connection6->port_name = std::string(config.get<std::string>("devices.fug2.connection.port"));
	connection5->server_name = std::string(config.get<std::string>("devices.fug1.connection.server"));
	connection6->server_name = std::string(config.get<std::string>("devices.fug2.connection.server"));
	connection7->port_name = std::string(config.get<std::string>("devices.psa1.connection.port"));
	connection8->port_name = std::string(config.get<std::string>("devices.psa2.connection.port"));
	connection9->port_name = std::string(config.get<std::string>("devices.psa3.connection.port"));

        boost::shared_ptr<bronkhorst> flowcontroll1(new bronkhorst);
        boost::shared_ptr<bronkhorst> flowcontroll2(new bronkhorst);
        boost::shared_ptr<RFG> RFG1(new RFG);
        boost::shared_ptr<Relais> relais1(new Relais);
	boost::shared_ptr<FUGNetzteil> fug1(new FUGNetzteil);
        boost::shared_ptr<FUGNetzteil> fug2(new FUGNetzteil);
	boost::shared_ptr<PSAPowersupply> psa1(new PSAPowersupply);
	boost::shared_ptr<PSAPowersupply> psa2(new PSAPowersupply);
        boost::shared_ptr<PSAPowersupply> psa3(new PSAPowersupply);
	

        flowcontroll1->setName(config.get<std::string>("devices.flowcontroll1.name"));
        flowcontroll2->setName(config.get<std::string>("devices.flowcontroll2.name"));
        RFG1->setName(config.get<std::string>("devices.rfg1.name"));
        relais1->setName(config.get<std::string>("devices.relais1.name"));
	fug1->setName(config.get<std::string>("devices.fug1.name"));
        fug2->setName(config.get<std::string>("devices.fug2.name"));
	psa1->setName(config.get<std::string>("devices.psa1.name"));
	psa2->setName(config.get<std::string>("devices.psa2.name"));
	psa3->setName(config.get<std::string>("devices.psa3.name"));
	

        if(!flowcontroll1->connect(connection1))
            std::cout << ">>>>>>>>>>>>>>>> Could not connect to flowcontroll1!!!" << std::endl;
	connection1->baud_rate = boost::asio::serial_port_base::baud_rate(38400);
        if(!flowcontroll2->connect(connection4))
            std::cout << ">>>>>>>>>>>>>>>> Could not connect to flowcontroll2!!!" << std::endl;
	connection4->baud_rate = boost::asio::serial_port_base::baud_rate(38400);
        if(!RFG1->connect(connection2))
            std::cout << ">>>>>>>>>>>>>>>> Could not connect to rfg1!!!" << std::endl;
//         if(!relais1->connect(connection2))
	connection3->endOfLineRegExpr(boost::regex("\\$"));
        if(!relais1->connect(connection3))
                std::cout << ">>>>>>>>>>>>>>>> Could not connect to relais1!!!" << std::endl;
	if(!fug1->connect(connection5))
            std::cout << ">>>>>>>>>>>>>>>> Could not connect to fug1!!!" << std::endl;
        if(!fug2->connect(connection6))
            std::cout << ">>>>>>>>>>>>>>>> Could not connect to fug2!!!" << std::endl;
	if(!psa1->connect(connection7))
	  std::cout << ">>>>>>>>>>>>>>>> Could not connect to psa1!!!" << std::endl;
	if(!psa2->connect(connection8))
	  std::cout << ">>>>>>>>>>>>>>>> Could not connect to psa2!!!" << std::endl;
	if(!psa3->connect(connection9))
	  std::cout << ">>>>>>>>>>>>>>>> Could not connect to psa3!!!" << std::endl;

	//Adding devices to map
        deviceMap.insert(deviceMapPair(flowcontroll1->getName(), flowcontroll1));
        deviceMap.insert(deviceMapPair(flowcontroll2->getName(), flowcontroll2));
        deviceMap.insert(deviceMapPair(RFG1->getName(), RFG1));
        deviceMap.insert(deviceMapPair(relais1->getName(), relais1));
        deviceMap.insert(deviceMapPair(fug1->getName(), fug1));
        deviceMap.insert(deviceMapPair(fug2->getName(), fug2));
	deviceMap.insert(deviceMapPair(psa1->getName(), psa1));
        deviceMap.insert(deviceMapPair(psa2->getName(), psa2));
	deviceMap.insert(deviceMapPair(psa3->getName(), psa3));
        


        Wt::Dbo::backend::Sqlite3 sqlite3("test.db");
        boost::shared_ptr<Wt::Dbo::Session> session(new Wt::Dbo::Session);
        boost::shared_ptr<Wt::Dbo::Session> session1(new Wt::Dbo::Session);
//   boost::shared_ptr<Wt::Dbo::Session> session2(new Wt::Dbo::Session);
        boost::shared_ptr<boost::asio::io_service> ioServiceDB(new boost::asio::io_service);
//   boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*ioService));
        boost::thread workerThread(boost::bind(&boost::asio::io_service::run, ioService));
        session->setConnection(sqlite3);
        session->mapClass<measuredDBValue<double>>("measuredValue_double");
//         session1->setConnection(sqlite3);
//         session1->mapClass<measuredDBValue<double>>("measuredValue_double");
//   session1->setConnection(sqlite3);
//   session1->mapClass<measuredDBValue>("measuredValue");
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
        
        DataAcquisition data(ioService,session);
        data.acquire(flowcontroll1->flow);
        data.acquire(flowcontroll1->setpoint);
        data.acquire(flowcontroll2->flow);
        data.acquire(flowcontroll2->setpoint);
        data.acquire(RFG1->voltage);
        data.acquire(RFG1->current);
        data.acquire(RFG1->power);
        data.acquire(fug1->voltage);
        data.acquire(fug1->current);
        data.acquire(fug2->voltage);
        data.acquire(fug2->current);
	data.start();
        std::cout << "Starting Acquisition!" << std::endl;
        
        
        

//   RFGTest rfg(session1,taskExecutor,RFG1,1);
//   rfg.setEvery(boost::posix_time::seconds(5));
//   rfg.setExecuteTimes(Task::Execute::infinite);
//   rfg.start();

//         BronkhorstTest horst1(session, taskExecutor,flowcontroll1);
//         horst1.setEvery(boost::posix_time::seconds(1));
//         horst1.setExecuteTimes(Task::Execute::infinite);
//         horst1.start();
// 
//         BronkhorstTest horst2(session, taskExecutor,flowcontroll2);
//         horst2.setEvery(boost::posix_time::seconds(1));
//         horst2.setExecuteTimes(Task::Execute::infinite);
//         horst2.start();

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
        data.stop();
        lughos::ioService->stop();
//    ofs<< "IOService stopping..." << std::endl;
        std::cout << "IOService stopping..." << std::endl;


        lughos::ioService->reset();
        thread.join();
        std::cout << "Everything cleaned up, quitting..." << std::endl;
//   ofs<< "Everything cleaned up, quitting..." << std::endl;

        return result;

    }
    catch(std::exception& e)
    {
        std::cout << "#############################################################" << std::endl
                  << "Unexpected program termination!" << std::endl
                  << "Error-Message: " << e.what() << std::endl;
    }

}

