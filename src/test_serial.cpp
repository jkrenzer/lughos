#include <iostream>

#include "basicConnections.hpp"
#include "connectionImpl.hpp"
#include <pthread.h>

int main(int argc, char **argv) {

  
  	bool rv;
	std::string port_name = "/dev/ttyUSB0";
// 	SerialPort::print_devices();
// 	std::string name = SerialPort::get_port_name(0);
	std::string name = port_name;

  
    connection<serialContext>* c = new connection<serialContext>();

//     c->context.RTS = false; 
//     c->context.DTR = true;
 
    	rv = c->start(name.c_str()); /*115200*/	
	//    c->reset();
	    	  std::cout << "rv="<< rv<< std::endl;
	if (rv == false) {
		return -1;
	}
//      std::cout << "Write="<< c->write("*IDN?\r")<< std::endl;
//           std::cout << "Write="<< c->write("*IDN?\r")<< std::endl;
//    	  std::cout << "Write="<< c->write("*RST\r")<< std::endl;
//     	  std::cout << "Write="<< c->write("READ?\r")<< std::endl;
/*    	  std::cout << "Write="<< c->write("PR1\r")<< std::endl;
	  	  	sleep(0.1);
    	  std::cout << "Write="<< c->write("\x05")<< std::endl;*/	  
//  std::cout << "Write="<< c->write("\x02")<< std::endl;
//   std::cout << "Write="<< c->write("\x02""SYS""1\r")<< std::endl;
//   	  	  	sleep(1);
//  std::cout << "Write="<< c->write("\x02""SYS""0\r")<< std::endl;
 std::cout << "Write="<< c->write("\x03")<< std::endl;
 std::cout << "Write="<< c->write("PR1""\r\n""\x05")<< std::endl;
   	  	  	sleep(1);
		
	return 0;
}
