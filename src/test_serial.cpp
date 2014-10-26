#define BOOST_ASIO_HANDLER_TRACKING

#include <iostream>
#include <map>
#include "coolpak6000.hpp"
#include "serialAsync.hpp"
#include "device.hpp"
#include "MaxiGauge.hpp"
#include "kithleighSerial.hpp"
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
   exec_task(boost::shared_ptr<boost::asio::io_service>  io_service,  boost::shared_ptr<kithleighSerial> keithley): Task (io_service), keithley(keithley)
  {
    
  }
protected:
  boost::shared_ptr<kithleighSerial> keithley;

  void run()
  {
     std::cout << "Write="<< keithley->inputOutput("*IDN?")<< std::endl;
  }
};


int main(int argc, char **argv) {
	boost::shared_ptr<boost::asio::io_service> io_service (new boost::asio::io_service);
	boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*io_service));
	boost::thread thread(boost::bind(&boost::asio::io_service::run, io_service.get()));
	
	boost::shared_ptr<boost::asio::io_service> exec_service (new boost::asio::io_service);
	boost::shared_ptr<boost::asio::io_service::work> exec_work(new boost::asio::io_service::work(*exec_service));
	boost::thread exec_thread(boost::bind(&boost::asio::io_service::run, exec_service.get()));	

	std::map< std::string, boost::shared_ptr<Device> > deviceMap;

//      boost::shared_ptr<serialAsync> connection1(new serialAsync(io_service) );
//      connection1->port_name = std::string("/dev/ttyUSB0");
//      boost::shared_ptr<Device> compressor1(new coolpak6000);
//      compressor1->setName(std::string("Compressor 1"));
//      compressor1->connect(connection1);
//      deviceMap.insert(deviceMapPair(compressor1->getName(), compressor1));
// 	boost::shared_ptr<coolpak6000> coolpak = boost::dynamic_pointer_cast<coolpak6000>(compressor1);
// 	std::cout << "Write="<< coolpak->get_data()<< std::endl;
// 	std::cout << "Write="<< coolpak->compressor_on()<< std::endl;
// 	sleep(1);
// 	std::cout << "Write="<< coolpak->compressor_off()<< std::endl;
	
	boost::shared_ptr<serialAsync> connection2(new serialAsync(io_service) );
        connection2->port_name = std::string("/dev/ttyUSB0");
	boost::shared_ptr<Device> pressureMonitor1(new MaxiGauge);
	pressureMonitor1->setName(std::string("Pressure Monitor 1"));
	pressureMonitor1->connect(connection2);
	deviceMap.insert(deviceMapPair(pressureMonitor1->getName(), pressureMonitor1));
	boost::shared_ptr<MaxiGauge> maxigauge = boost::dynamic_pointer_cast<MaxiGauge>(pressureMonitor1);
// 	std::cout << "Write="<< maxigauge->inputOutput("\x05")<< std::endl;
	std::cout << "Write="<< maxigauge->get_status(1)<< std::endl;

	      
//      boost::shared_ptr<serialAsync> connection3(new serialAsync(io_service) );
//      connection3->port_name = std::string("/dev/ttyUSB0");
//      boost::shared_ptr<Device> temperatureMonitor1(new kithleighSerial);
//      temperatureMonitor1->setName(std::string("Temperature Monitor 1"));
//      temperatureMonitor1->connect(connection3);
//      deviceMap.insert(deviceMapPair(temperatureMonitor1->getName(), temperatureMonitor1));
// 	boost::shared_ptr<kithleighSerial> keithley = boost::dynamic_pointer_cast<kithleighSerial>(temperatureMonitor1);
//      std::cout << "Write="<< keithley->inputOutput("*IDN?")<< std::endl;
//     exec_task task(exec_service, keithley);
//     task.setEvery(boost::posix_time::seconds(1));
//     task.setExecuteTimes(3);
//     task.start();
      


		


// 		    std::cout << "At least i write something"<< std::endl;
// 		MaxiGauge* c = new MaxiGauge;
// 	connection<serialContext>* c = new connection<serialContext>();

//     c->context.RTS = false; 
//     c->context.DTR = true;
 

//      std::cout << "Write="<< c->write("*IDN?")<< std::endl;
//    	  std::cout << "Write="<< c->write_async("*RST")<< std::endl;
//     	  std::cout << "Write="<< c->write("READ?\r")<< std::endl;
// 	 std::cout << "Write="<< c->write(":TRAC:FEED:CONT NEVER")<< std::endl;
// 	 std::cout << "Write="<< c->write(":TRAC:CLE")<< std::endl;
// 	 std::cout << "Write="<< c->write_async("*IDN?")<< std::endl;
/*    	  std::cout << "Write="<< c->write("PR1\r")<< std::endl;
	  	  	sleep(0.1);*/
//     	  std::cout << "Write="<< c->get_data()<< std::endl;	  
//  std::cout << "Write="<< c->write("\x02")<< std::endl;
//   std::cout << "Write="<< c->inputoutput("*IDN?")<< std::endl;
//     std::cout << c->inputoutput("DAT")<< std::endl;
//    std::cout <<  c->get_software_version()<< std::endl;
//       std::cout <<  c->get_compressor_state()<< std::endl;
//             std::cout <<  c->get_operating_hours().getvalue() << c->get_operating_hours().getunit()<< std::endl;
// std::cout << "Write="<< c->inputoutput("\x02")<< std::endl;
// std::cout << "Write="<< c->inputoutput(":06030401210121")<< std::endl;

// c->on();
// c->on();
// std::cout << "Write="<<c->get_voltage()<< std::endl;
// std::cout << "Write="<<c->get_current()<< std::endl;
// std::cout << "Write="<<c->get_temperature()<< std::endl;
//   	  	  	sleep(1);
//  std::cout << "Write="<< c->exp_lenght<< std::endl;
//  std::cout << "Write="<< c->write("\x03")<< std::endl;
//  std::cout << "Write="<< c->write("PR1""\r\n""\x05")<< std::endl;
//    	  	  	sleep(1);	
// 	delete io_service;


//    ofs<< "IOService stopping..." << std::endl;
  std::cout << "IOService waiting for children..." << std::endl;
        exec_work.reset(); 
	exec_thread.join();
	work.reset();

    std::cout << "IOService stopping..." << std::endl;
   
    
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