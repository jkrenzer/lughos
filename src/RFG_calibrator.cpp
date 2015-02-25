#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/shared_ptr.hpp>

#include "RFG.hpp"
#include "keithley.hpp"
#include <limits>

#define CONFIG_FILENAME "config.xml"
using namespace std;

double keithleyValue(const std::string s)
{
  boost::regex regExprCurrent("([-\\+]\\d\\.\\d{8}E[-\\+]\\d\\d)ADC");
  boost::smatch res;
  boost::regex_search(s.begin(), s.end(),res,regExprCurrent);
  double d;
  if( res[1] == "")
    return std::numeric_limits<double>::quiet_NaN();
  std::stringstream(res[1]) >> d;
  return d;
}

int main(int argc, char **argv)
{
  boost::shared_ptr<boost::asio::io_service> ioService(new boost::asio::io_service);
  boost::asio::io_service::work work(*ioService);
  boost::thread thread(boost::bind(&boost::asio::io_service::run, ioService));
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
  
  boost::shared_ptr<RFG> rfg(new RFG);
  boost::shared_ptr<Keithley> keithley(new Keithley);
  
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
  
  rfg->switch_off();
  rfg->set_current_lim(3.0);
  rfg->set_voltage_max(35);
  rfg->set_voltage_min(0);
  rfg->power_supply_mode();
  rfg->use_current_controler();
  rfg->set_target_value_raw(0);
  
  boost::this_thread::sleep_for(boost::chrono::seconds(2));
  
  std::cout << "RFG OFF: " << keithleyValue(keithley->inputOutput("MEASure:CURRent:DC?",boost::regex("<body>(.*)</body>"))) << std::endl;
  rfg->switch_on();
  boost::this_thread::sleep_for(boost::chrono::seconds(2));
  std::cout << "RFG ON: " << keithleyValue(keithley->inputOutput("MEASure:CURRent:DC?",boost::regex("<body>(.*)</body>"))) << std::endl;
  rfg->switch_off();
  ofstream mfile ("Measurement.txt");
  if (!mfile.is_open())
  {
    std::cout << "Could not open file to write. aborting!" << std::endl;
  }
  std::cout << "Beginning measurement" << std::endl;
  rfg->switch_on();
  std::cout << "Waiting for RFG to settle..." << std::endl;
  boost::this_thread::sleep_for(boost::chrono::seconds(2));
  int stepSize = 100;
  int units;
  double current;
  stringstream outstream;
  mfile << "% units , current!" << std::endl;
  for (int i = 0; i < 4096/stepSize ; i++)
  {
    units = i*stepSize;
     rfg->set_target_value_raw(units);
     boost::this_thread::sleep_for(boost::chrono::seconds(2));
     current = keithleyValue(keithley->inputOutput("MEASure:CURRent:DC?",boost::regex("<body>(.*)</body>")));
     if(current >= 3.0)
     {
       std::cout << "Maximum current reached. Aborting!" << std::endl;
       mfile << "% Maximum current reached. Aborting!" << std::endl;
       rfg->switch_off();
       rfg->set_target_value_raw(0);
     }
     else if(current == std::numeric_limits< double >::quiet_NaN)
     {
       std::cout << "Keithley not answering. Aborting!" << std::endl;
       mfile << "% Keithley not answering. Aborting!" << std::endl;
       rfg->switch_off();
       rfg->set_target_value_raw(0);
     }
     outstream << units << " , " << current << std::endl;
     std::cout << outstream.str();
     mfile << outstream;
  }
  mfile.close();
  
  return 0;
}
