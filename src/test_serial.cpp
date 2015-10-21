/********************************************************************************
 *   This file is part of LUGHOS.						*
 *										*
 *										*
 *  LUGHOS is free software: you can redistribute it and/or modify		*
 *  it under the terms of the GNU General Public License as published by	*
 *  the Free Software Foundation, either version 3 of the License, or		*
 *  (at your option) any later version.						*
 *										*
 *  LUGHOS is distributed in the hope that it will be useful,			*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of		*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*
 *  GNU General Public License for more details.				*
 *										*
 *  We sincerily invite everybody to participate in LUGHO'S development. 	*
 *  Together we can achieve more.						*
 *										*
 *  (c) 2014, 2015 Jörn Mankiewicz (joern.mankiewicz@gmx.de), 			*
 *                 Irina Brodski (irina@brodski.de)				*
 *										*
 ********************************************************************************/

#define BOOST_ASIO_HANDLER_TRACKING

#include <iostream>
#include <map>
#include "coolpak6000.hpp"
#include "serialAsync.hpp"
#include "device.hpp"
#include "MaxiGauge.hpp"
#include "RFG.hpp"
#include "jobQueue.hpp"
// #include "connectionImpl.hpp"
// #include "basicConnections.hpp"
#include "Relais.hpp"
#include "bronkhorst.hpp"
#include <pthread.h>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

typedef std::pair<std::string, boost::shared_ptr<Device> > deviceMapPair;

class exec_task : public lughos::Task
{
public:
   exec_task(boost::shared_ptr<boost::asio::io_service>  io_service,  boost::shared_ptr<Relais> relais): Task (io_service), relais(relais)
//    exec_task(boost::shared_ptr<boost::asio::io_service>  io_service,  boost::shared_ptr<bronkhorst> horst): Task (io_service), horst(horst)

   {
    
  }
protected:
  boost::shared_ptr<Relais> relais;
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
	      
     boost::shared_ptr<serialAsync> connection3(new serialAsync(io_service) );
     connection3->port_name = std::string("/dev/ttyUSB0");
     
     boost::shared_ptr<Device> relais1(new Relais);
     
     relais1->setName(std::string("relais 1"));
     connection3->endOfLineRegExpr('$');
     relais1->connect(connection3);
     deviceMap.insert(deviceMapPair(relais1->getName(), relais1));
     boost::shared_ptr<Relais> relais = boost::dynamic_pointer_cast<Relais>(relais1);
     std::cout << "Write="<< relais->read_channels()<< std::endl;
     std::cout << "Write="<< relais->write_channels("11111111")<< std::endl;  
          std::cout << "Write="<< relais->read_channels()<< std::endl;  
//      std::cout << "Write="<< relais->inputOutput("")<< std::endl;
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
   
      work.reset();
      thread.join();
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