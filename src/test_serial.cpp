#include <iostream>
#include "coolpak6000.hpp"
// #include "MaxiGauge.hpp"
// #include "kithleighSerial.hpp"
// #include "connectionImpl.hpp"
// #include "basicConnections.hpp"
#include <pthread.h>

int main(int argc, char **argv) {

  
  	bool rv;
	std::string port_name = "/dev/ttyUSB0";
// 	SerialPort::print_devices();
// 	std::string name = SerialPort::get_port_name(0);
	std::string name = port_name;

  
//     kithleighSerial* c = new kithleighSerial;
	coolpak6000* c = new coolpak6000;
// 		MaxiGauge* c = new MaxiGauge;
// 	connection<serialContext>* c = new connection<serialContext>();

//     c->context.RTS = false; 
//     c->context.DTR = true;
 
    	rv = c->start(name.c_str()); /*115200*/	
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
	  	  	sleep(0.1);
    	  std::cout << "Write="<< c->write("\x05")<< std::endl;*/	  
//  std::cout << "Write="<< c->write("\x02")<< std::endl;
//   std::cout << "Write="<< c->inputoutput("*IDN?")<< std::endl;
//     std::cout << "Write="<< c->inputoutput("PR1")<< std::endl;
    std::cout << "Write="<< c->inputoutput("DAT",1)<< std::endl;


//   	  	  	sleep(1);
//  std::cout << "Write="<< c->write("SYS0")<< std::endl;
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