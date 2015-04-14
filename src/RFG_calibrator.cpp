#include <iostream>
#include <csignal>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

#include "RFG.hpp"
#include "keithley.hpp"
#include <limits>
#include <cmath>
#include <vector>
#include <sstream>

#define CONFIG_FILENAME "config.xml"
using namespace std;

double interpretKeithleyValue(const std::string s)
{
  boost::regex regExprCurrent("([-\\+]\\d\\.\\d{8}E[-\\+]\\d\\d)[AV]DC");
  boost::smatch res;
  boost::regex_search(s.begin(), s.end(),res,regExprCurrent);
  double d;
  if( res[1] == "")
    return std::numeric_limits<double>::quiet_NaN();
  std::stringstream(res[1]) >> d;
  return d;
}

template <class T>
T calculateMean(std::vector<T> values)
{
  T sum=0;
  for(typename std::vector<T>::const_iterator i = values.begin(); i != values.end(); i++ )
  {
    sum += *i;
  }
  return sum/values.size();
}

/* Function used to check that 'opt1' and 'opt2' are not specified
   at the same time. */
void conflicting_options(const boost::program_options::variables_map& vm, 
                         const char* opt1, const char* opt2)
{
    if (vm.count(opt1) && !vm[opt1].defaulted() 
        && vm.count(opt2) && !vm[opt2].defaulted())
        throw logic_error(string("Conflicting options '") 
                          + opt1 + "' and '" + opt2 + "'.");
}

int main(int argc, char **argv)
{
  
  int measureTimes;
  int measureChannel;
  std::string keithleyQuery;
  std::string typeDesignation;
  double measureLimit;
  
  // Declare the supported options.
boost::program_options::options_description desc("Allowed options");
desc.add_options()
    ("help", "produce help message")
    ("measurements,t", boost::program_options::value<int>(&measureTimes)->default_value(3), "set how many measurements to take for mean value")
    ("voltage,v" , "calibrate voltage, default")
    ("current,c" , "calibrate current")
    ("limit,l",boost::program_options::value(&measureLimit)->default_value(0), "limit-value on keithley which will abort measurement" );

boost::program_options::variables_map vm;
boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
boost::program_options::notify(vm);
  try 
  {
    conflicting_options(vm, "voltage", "current");
  }
  catch(std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  
  if (vm.count("help")) 
  {
    std::cout << desc << std::endl;
    return 1;
  }
  
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
  boost::shared_ptr<serialConnection> connection1(new RFGConnection() );
  boost::shared_ptr<tcpConnection> connection2(new KeithleyConnection() );
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
  
  if(vm.count("current"))
  {
    measureChannel = 1;
    keithleyQuery = string("MEASure:CURRent:DC?");
    typeDesignation = string("current");
    rfg->use_current_controler();
    measureLimit = (measureLimit > 0 && measureLimit <= 3) ? measureLimit : 3.0;
  }
  else
  {
    measureChannel = 0;
    keithleyQuery = string("MEASure:VOLTage:DC?");
    typeDesignation = string("voltage");
    rfg->use_voltage_controler();
    measureLimit = (measureLimit > 0 && measureLimit <= 35) ? measureLimit : 35.0;
  }
  
  rfg->switch_off();
  rfg->set_current_lim_raw(2600);
  rfg->set_voltage_max_raw(4095);
  rfg->set_voltage_min(0);
  rfg->power_supply_mode();
  rfg->set_target_value_raw(0);
  
  stringstream filename;
  filename << "calibration" << "_" << typeDesignation << "_"; 
  ofstream mfileDAC(filename.str()+"DAC.txt");
  ofstream mfileADC(filename.str()+"ADC.txt");
  ofstream mfileRaw(filename.str()+"RAW.txt");
  if (!mfileDAC.is_open() || !mfileDAC.is_open())
  {
    std::cout << "Could not open file to write. aborting!" << std::endl;
  }
  std::cout << "Beginning measurement" << std::endl
	    << "Will measure till keithley shows a value greater than " << measureLimit << std::endl;
  rfg->switch_on();
  std::cout << "Waiting 5 seconds for RFG to settle..." << std::endl;
  boost::this_thread::sleep_for(boost::chrono::seconds(5));
  int stepSize = 100;
  int unitsDAC = 0;
  int unitsADC = 0;
  double keithleyValue = 0;
  std::vector<int> unitsADCValues;
  std::vector<double> keithleyValues;
  mfileDAC << "% units(DAC) , " << typeDesignation << std::endl;
  mfileADC << "% units(ADC) , " << typeDesignation << std::endl;
  mfileRaw << "% units(ADC) , " << "units(DAC)" << typeDesignation << std::endl;
  
  try 
  {
    for (int i = 0; i < 4096/stepSize ; i++)
    {
        unitsDAC = i*stepSize;
	rfg->set_target_value_raw(unitsDAC);
	boost::this_thread::sleep_for(boost::chrono::seconds(2));
        double keithleyMeasurement = 0.0;
        int unitsADCSetting = 0;
	for (int j = 0; j < measureTimes; j++)
	{
          keithleyMeasurement = interpretKeithleyValue(keithley->inputOutput(keithleyQuery,boost::regex("<body>(.*)</body>")));
          if(keithleyMeasurement >= measureLimit)
          {
            std::cout << "Maximum reached. Aborting!" << std::endl;
            mfileDAC << "% Maximum reached. Aborting!" << std::endl;
            mfileADC << "% Maximum reached. Aborting!" << std::endl;
            mfileRaw << "% Maximum reached. Aborting!" << std::endl;
            rfg->switch_off();
            rfg->set_target_value_raw(0);
            goto EndOfMeasurement;
          }
          else if(std::isnan(keithleyMeasurement))
          {
            std::cout << "Keithley not answering. Aborting!" << std::endl;
            mfileDAC << "% Keithley not answering. Aborting!" << std::endl;
            mfileADC << "% Keithley not answering. Aborting!" << std::endl;
            mfileRaw << "% Keithley not answering. Aborting!" << std::endl;
            rfg->switch_off();
            rfg->set_target_value_raw(0);
            mfileADC.flush();
            mfileDAC.flush();
            mfileRaw.flush();
            goto EndOfMeasurement;
            
          }
          unitsADCSetting = rfg->get_channel_raw(measureChannel,true);
	  keithleyValues.push_back(keithleyMeasurement);
	  unitsADCValues.push_back(unitsADCSetting);
          mfileRaw << unitsADCSetting << " , " << unitsDAC << " , " << keithleyMeasurement;
	}
	keithleyValue = calculateMean(keithleyValues);
	unitsADC = calculateMean(unitsADCValues);
	std::cout << unitsDAC << " , " << unitsADC << " , " << keithleyValue << std::endl;
	mfileDAC << unitsDAC << " , " << keithleyValue << std::endl;
	mfileADC << unitsADC << " , " << keithleyValue << std::endl;
	keithleyValues.clear();
	unitsADCValues.clear();
	keithleyValue = 0;
	unitsADC = 0;
	
    }
  }
  catch(...)
  {
    std::cout << "Crashed. Setting save and aborting!" << std::endl;
    mfileDAC << "% Crashed. Setting save and aborting!" << std::endl;
    mfileADC << "% Crashed. Setting save and aborting!" << std::endl;
    mfileRaw << "% Crashed. Setting save and aborting!" << std::endl;
  }
EndOfMeasurement:
  std::cout << "Finished.Switching off." << std::endl;
  rfg->switch_off();
  rfg->set_target_value_raw(0);
  mfileDAC.close();
  mfileADC.close();
  mfileRaw.close();
  
  return 0;
}
