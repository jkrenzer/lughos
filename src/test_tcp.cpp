#define BOOST_ASIO_HANDLER_TRACKING
#include <boost/chrono.hpp>
#include <iostream>
#include <map>
// #include "coolpak6000.hpp"
#include "tcpAsync.hpp"
#include "device.hpp"
#include "FUGNetzteil.hpp"
// #include "RFG.hpp"
#include "jobQueue.hpp"
// #include "connectionImpl.hpp"
// #include "basicConnections.hpp"
// #include "PSANetzteil.hpp"
// #include "bronkhorst.hpp"
#include <pthread.h>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

typedef std::pair<std::string, boost::shared_ptr<Device> > deviceMapPair;

class exec_task : public lughos::Task
{
public:
   exec_task(boost::shared_ptr<boost::asio::io_service>  io_service,  boost::shared_ptr<fug> rfg): Task (io_service), rfg(rfg)
//    exec_task(boost::shared_ptr<boost::asio::io_service>  io_service,  boost::shared_ptr<bronkhorst> horst): Task (io_service), horst(horst)

   {
    
  }
protected:
  boost::shared_ptr<fug> rfg;
//   boost::shared_ptr<bronkhorst> horst;
  void run()
  {
//      std::cout << "Write="<< rfg->readout()<< std::endl;
  }
};


int main(int argc, char **argv) {
	boost::shared_ptr<boost::asio::io_service> io_service (new boost::asio::io_service);
	boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*io_service));
	boost::thread thread(boost::bind(&boost::asio::io_service::run, io_service.get()));
	
// 	boost::shared_ptr<boost::asio::io_service> exec_service (new boost::asio::io_service);
// 	boost::shared_ptr<boost::asio::io_service::work> exec_work(new boost::asio::io_service::work(*exec_service));
// 	boost::thread exec_thread(boost::bind(&boost::asio::io_service::run, exec_service.get()));	

     std::map< std::string, boost::shared_ptr<Device> > deviceMap;
	      
     boost::shared_ptr<tcpAsync> connection3(new tcpAsync(io_service) );
     connection3->server_name = std::string("192.168.178.46");
     connection3->set_port("2101");
     boost::shared_ptr<Device> kith(new fug);
     
     kith->setName(std::string("RFG 1"));
     kith->connect(connection3);
     deviceMap.insert(deviceMapPair(kith->getName(), kith));
	boost::shared_ptr<fug> rfg = boost::dynamic_pointer_cast<fug>(kith);
//      std::cout << "Write="<< rfg->get_channel(1).getvalue()<< std::endl;
// 	rfg->input("/LICENSE_1_0.txt\n");
// 	std::cout << "Write="<< rfg->getIDN()<< std::endl;
//      std::cout << "Error="<< rfg->getLastError()<< std::endl;
//      	std::cout << "Write="<< rfg->getIDN()<< std::endl;
//      std::cout << "Error="<< rfg->getLastError()<< std::endl;
//     std::cout << "Write="<< rfg->setI(0.15)<< std::endl;
//     std::cout << "Error="<< rfg->getLastError()<< std::endl;
// 	 std::cout << "Write="<< rfg->inputOutput("")<< std::endl;
     std::cout << "Write="<< rfg->getI()<< std::endl;
     std::cout << "Error="<< rfg->getLastError()<< std::endl;
//      boost::shared_ptr<Device> horst1(new bronkhorst);
//      
//      horst1->setName(std::string("RFG 1"));
//      horst1->connect(connection3);
//      deviceMap.insert(deviceMapPair(horst1->getName(), horst1));
// 	boost::shared_ptr<bronkhorst> horst = boost::dynamic_pointer_cast<bronkhorst>(horst1);
//      std::cout << "Write="<< horst->get_value().getvalue()<< std::endl;
   
//   std::cout << "Execution-service stopping..." << std::endl;
//         exec_work.reset(); 
// 	exec_thread.join();
	

    std::cout << "IOService stopping..." << std::endl;
    io_service.reset();
      std::cout << thread.joinable() << std::endl;
      work.reset();
      if(thread.joinable()) thread.timed_join( boost::posix_time::seconds(3) );
   std::cout << "IOService stopped..." << std::endl;
	return 0;
}

// #include <iostream>
// 
// #include "basicConnections.hpp"
// #include "connectionImpl.hpp"
// #include <pthread.h>
// 
// int main(int argc, char **argv) {
// 
//   
//   	bool rv;
// 	std::string port_name = "/dev/ttyUSB0";
// // 	SerialPort::print_devices();
// // 	std::string name = SerialPort::get_port_name(0);
// 	std::string name = port_name;
// 
//   
//     connection<serialContext>* c = new connection<serialContext>();
// 
// //     c->context.RTS = false; 
// //     c->context.DTR = true;
//  
//     	rv = c->start(name.c_str()); /*115200*/	
// 	//    c->reset();
// 	    	  std::cout << "rv="<< rv<< std::endl;
// 	if (rv == false) {
// 		return -1;
// 	}
//      std::cout << "Write="<< c->write("*IDN?\r")<< std::endl;
// //           std::cout << "Write="<< c->write("*IDN?\r")<< std::endl;
// //    	  std::cout << "Write="<< c->write("*RST\r")<< std::endl;
// //     	  std::cout << "Write="<< c->write("READ?\r")<< std::endl;
// /*    	  std::cout << "Write="<< c->write("PR1\r")<< std::endl;
// 	  	  	sleep(0.1);
//     	  std::cout << "Write="<< c->write("\x05")<< std::endl;*/	  
// //  std::cout << "Write="<< c->write("\x02")<< std::endl;
// //   std::cout << "Write="<< c->write("\x02""SYS""1\r")<< std::endl;
// //   	  	  	sleep(1);
// //  std::cout << "Write="<< c->write("\x02""SYS""0\r")<< std::endl;
// //  std::cout << "Write="<< c->write("\x03")<< std::endl;
// //  std::cout << "Write="<< c->write("PR1""\r\n""\x05")<< std::endl;
//    	  	  	sleep(1);
// 		
// 	return 0;
    
// }