#include <iostream>
#include "coolpak6000.hpp"
// #include "MaxiGauge.hpp"
// #include "kithleighSerial.hpp"
// #include "connectionImpl.hpp"
// #include "basicConnections.hpp"
// #include "PSANetzteil.hpp"
// #include "bronkhorst.hpp"
#include <pthread.h>
#include <boost/asio.hpp>

int main(int argc, char **argv) {
// boost::asio::io_service * io_service;
  
  	bool rv;

// 	SerialPort::print_devices();
// 	std::string name = SerialPort::get_port_name(0);
// 	std::string name = port_name;

  
//     kithleighSerial* c = new kithleighSerial;
	boost::shared_ptr<coolpak6000> c(new coolpak6000);
		c->port_name = "COM1";
// 		MaxiGauge* c = new MaxiGauge;
// 	connection<serialContext>* c = new connection<serialContext>();

//     c->context.RTS = false; 
//     c->context.DTR = true;
 
    	rv = c->start(); /*115200*/	
	//    c->reset();
	    	  std::cout << "rv="<< rv<< std::endl;
	if (rv == false) {
		return -1;
	}
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
   std::cout <<  c->get_software_version()<< std::endl;
      std::cout <<  c->get_compressor_state()<< std::endl;
            std::cout <<  c->get_operating_hours().getvalue() << c->get_operating_hours().getunit()<< std::endl;
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