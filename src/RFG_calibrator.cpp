#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/shared_ptr.hpp>

#include "RFG.hpp"
#include "keithley.hpp"

#define CONFIG_FILENAME "config.xml"

int main(int argc, char **argv)
{
  boost::shared_ptr<boost::asio::io_service> ioService;
  using boost::property_tree::ptree;
  ptree config;
  try
  {
    boost::property_tree::read_xml(CONFIG_FILENAME, config);
  }
      catch(...)
  {
    std::cout << "No valid config found!" << std::endl;
  }
  boost::shared_ptr<serialAsync> connection1(new RFGConnection(ioService) );
  boost::shared_ptr<tcpAsync> connection2(new KeithleyConnection(ioService) );
  connection1->port_name = config.get<std::string>("devices.rfg1.connection.port");
  connection2->port_name = config.get<std::string>("devices.keithley1.connection.port");
  connection2->server_name = config.get<std::string>("devices.keithley1.connection.server");
  
  boost::shared_ptr<Device> rfg(new RFG);
  boost::shared_ptr<Device> keithley(new Keithley);
  
  rfg->setName(config.get<std::string>("devices.rfg1.name"));
  keithley->setName(config.get<std::string>("devices.keithley1.name"));
  
  if(!rfg->connect(connection1) && rfg->isConnected())
  {
    std::cout << "Could not connect to RFG on port " << config.get<std::string>("devices.rfg1.connection.port") << std::endl;
    return 1;
  }
  if(!keithley->connect(connection2) && keithley->isConnected())
  {
    std::cout << "Could not connect to keithley on " << config.get<std::string>("devices.keithley1.connection.server") << ":" << config.get<std::string>("devices.keithley1.connection.port") << std::endl;
  }

  std::cout << "Connected to devices."  << std::endl;
  
  return 0;
}