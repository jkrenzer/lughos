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

    c->context.RTS = false; 
    c->context.DTR = true;
 
    	rv = c->start(name.c_str()); /*115200*/	
	//    c->reset();
// 	    	  std::cout << "rv="<< rv<< std::endl;
	if (rv == false) {
		return -1;
	}
    

    	  std::cout << "Write="<< c->write("D")<< std::endl;
	  
	  	sleep(5);

		
	return 0;
}
