// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "serialAsync.hpp"
#include "RFG.hpp"

RFG::RFG()
{
  for (int i=0;i<8;i++)
    {
      channel_output[i].setunitvalue(0,"");
    }
  SplineTransformation::XToYMap& x2y = unitsToVoltageReg.valueMap.left;
  //Callibration 2015-02-13;
  x2y.insert(SplineTransformation::XYPair(0, 0.154032));
  x2y.insert(SplineTransformation::XYPair(10, 0.271));
  x2y.insert(SplineTransformation::XYPair(20, 0.394));
  x2y.insert(SplineTransformation::XYPair(30, 0.517));
  x2y.insert(SplineTransformation::XYPair(40, 0.637));
  x2y.insert(SplineTransformation::XYPair(50, 0.760));
  x2y.insert(SplineTransformation::XYPair(60, 0.881));
  x2y.insert(SplineTransformation::XYPair(70, 1.003));
  x2y.insert(SplineTransformation::XYPair(80, 1.125));
  x2y.insert(SplineTransformation::XYPair(90, 1.245));
  x2y.insert(SplineTransformation::XYPair(100, 1.367));
  x2y.insert(SplineTransformation::XYPair(200, 2.579));
  x2y.insert(SplineTransformation::XYPair(300, 3.793));
  x2y.insert(SplineTransformation::XYPair(400, 5.006));
  x2y.insert(SplineTransformation::XYPair(500, 6.219));
  x2y.insert(SplineTransformation::XYPair(600, 7.432));
  x2y.insert(SplineTransformation::XYPair(700, 8.645));
  x2y.insert(SplineTransformation::XYPair(800, 9.858));
  x2y.insert(SplineTransformation::XYPair(900, 11.070));
  x2y.insert(SplineTransformation::XYPair(1000, 12.282));
  x2y.insert(SplineTransformation::XYPair(1100, 13.496));
  x2y.insert(SplineTransformation::XYPair(1200, 14.708));
  x2y.insert(SplineTransformation::XYPair(1300, 15.921));
  x2y.insert(SplineTransformation::XYPair(1400, 17.134));
  x2y.insert(SplineTransformation::XYPair(1500, 18.347));
  x2y.insert(SplineTransformation::XYPair(1600, 19.560));
  x2y.insert(SplineTransformation::XYPair(1700, 20.772));
  x2y.insert(SplineTransformation::XYPair(1800, 21.984));
  x2y.insert(SplineTransformation::XYPair(1900, 23.197));
  x2y.insert(SplineTransformation::XYPair(2000, 24.409));
  x2y.insert(SplineTransformation::XYPair(2100, 25.623));
  x2y.insert(SplineTransformation::XYPair(2200, 26.835));
  x2y.insert(SplineTransformation::XYPair(2300, 28.048));
  x2y.insert(SplineTransformation::XYPair(2400, 29.259));
  x2y.insert(SplineTransformation::XYPair(2500, 30.473));
  x2y.insert(SplineTransformation::XYPair(2600, 31.684));
  x2y.insert(SplineTransformation::XYPair(2700, 32.898));
  x2y.insert(SplineTransformation::XYPair(2800, 34.111));
  x2y.insert(SplineTransformation::XYPair(2900, 35.324));
  x2y.insert(SplineTransformation::XYPair(3000, 36.536));
  x2y.insert(SplineTransformation::XYPair(3100, 37.749));

  unitsToVoltageReg.init();
  
  SplineTransformation::XToYMap& x2y1a = unitsToVoltageLimMax.valueMap.left;
  x2y1a.insert(SplineTransformation::XYPair(0, 0.22));
  x2y1a.insert(SplineTransformation::XYPair(100, 1.436));
  x2y1a.insert(SplineTransformation::XYPair(200, 2.649));
  x2y1a.insert(SplineTransformation::XYPair(300, 3.861));
  x2y1a.insert(SplineTransformation::XYPair(400, 5.073));
  x2y1a.insert(SplineTransformation::XYPair(500, 6.286));
  x2y1a.insert(SplineTransformation::XYPair(600, 7.499));
  x2y1a.insert(SplineTransformation::XYPair(700, 8.712));
  x2y1a.insert(SplineTransformation::XYPair(800, 9.925));
  x2y1a.insert(SplineTransformation::XYPair(900, 11.137));
  x2y1a.insert(SplineTransformation::XYPair(1000, 12.35));
  x2y1a.insert(SplineTransformation::XYPair(1100, 13.563));
  x2y1a.insert(SplineTransformation::XYPair(1200, 14.776));
  x2y1a.insert(SplineTransformation::XYPair(1300, 15.988));
  x2y1a.insert(SplineTransformation::XYPair(1400, 17.2));
  x2y1a.insert(SplineTransformation::XYPair(1500, 18.413));
  x2y1a.insert(SplineTransformation::XYPair(1600, 19.626));
  x2y1a.insert(SplineTransformation::XYPair(1700, 20.839));
  x2y1a.insert(SplineTransformation::XYPair(1800, 22.052));
  x2y1a.insert(SplineTransformation::XYPair(1900, 23.264));
  x2y1a.insert(SplineTransformation::XYPair(2000, 24.477));
  x2y1a.insert(SplineTransformation::XYPair(2100, 25.69));
  x2y1a.insert(SplineTransformation::XYPair(2200, 26.903));
  x2y1a.insert(SplineTransformation::XYPair(2300, 28.115));
  x2y1a.insert(SplineTransformation::XYPair(2400, 29.328));
  x2y1a.insert(SplineTransformation::XYPair(2500, 30.542));
  x2y1a.insert(SplineTransformation::XYPair(2600, 31.754));
  x2y1a.insert(SplineTransformation::XYPair(2700, 32.967));
  unitsToVoltageLimMax.init();
  
  SplineTransformation::XToYMap& x2y1b = unitsToVoltageLimMin.valueMap.left;
  x2y1b.insert(SplineTransformation::XYPair(0, 0.154));
  x2y1b.insert(SplineTransformation::XYPair(100, 1.37));
  x2y1b.insert(SplineTransformation::XYPair(200, 2.583));
  x2y1b.insert(SplineTransformation::XYPair(300, 3.796));
  x2y1b.insert(SplineTransformation::XYPair(400, 5.009));
  x2y1b.insert(SplineTransformation::XYPair(500, 6.221));
  x2y1b.insert(SplineTransformation::XYPair(600, 7.434));
  x2y1b.insert(SplineTransformation::XYPair(700, 8.646));
  x2y1b.insert(SplineTransformation::XYPair(800, 9.859));
  x2y1b.insert(SplineTransformation::XYPair(900, 11.072));
  x2y1b.insert(SplineTransformation::XYPair(1000, 12.284));
  x2y1b.insert(SplineTransformation::XYPair(1100, 13.498));
  x2y1b.insert(SplineTransformation::XYPair(1200, 14.711));
  x2y1b.insert(SplineTransformation::XYPair(1300, 15.923));
  x2y1b.insert(SplineTransformation::XYPair(1400, 17.136));
  x2y1b.insert(SplineTransformation::XYPair(1500, 18.349));
  x2y1b.insert(SplineTransformation::XYPair(1600, 19.561));
  x2y1b.insert(SplineTransformation::XYPair(1700, 20.774));
  x2y1b.insert(SplineTransformation::XYPair(1800, 21.986));
  x2y1b.insert(SplineTransformation::XYPair(1900, 23.199));
  x2y1b.insert(SplineTransformation::XYPair(2000, 24.411));
  x2y1b.insert(SplineTransformation::XYPair(2100, 25.624));
  x2y1b.insert(SplineTransformation::XYPair(2200, 26.837));
  x2y1b.insert(SplineTransformation::XYPair(2300, 28.05));
  x2y1b.insert(SplineTransformation::XYPair(2400, 29.261));
  x2y1b.insert(SplineTransformation::XYPair(2500, 30.474));
  x2y1b.insert(SplineTransformation::XYPair(2600, 31.686));
  x2y1b.insert(SplineTransformation::XYPair(2700, 32.899));
  unitsToVoltageLimMin.init();
  
  SplineTransformation::XToYMap& x2y2 = unitsToCurrentLim.valueMap.left;
  x2y2.insert(SplineTransformation::XYPair(0, 0.02));
  x2y2.insert(SplineTransformation::XYPair(10, 0.034));
  x2y2.insert(SplineTransformation::XYPair(20, 0.043));
  x2y2.insert(SplineTransformation::XYPair(50, 0.082));
  x2y2.insert(SplineTransformation::XYPair(100, 0.143));
  x2y2.insert(SplineTransformation::XYPair(150, 0.202));
  x2y2.insert(SplineTransformation::XYPair(200, 0.263));
  x2y2.insert(SplineTransformation::XYPair(250, 0.323));
  x2y2.insert(SplineTransformation::XYPair(300, 0.384));
  x2y2.insert(SplineTransformation::XYPair(350, 0.444));
  x2y2.insert(SplineTransformation::XYPair(400, 0.504));
  x2y2.insert(SplineTransformation::XYPair(450, 0.564));
  x2y2.insert(SplineTransformation::XYPair(500, 0.625));
  x2y2.insert(SplineTransformation::XYPair(550, 0.685));
  x2y2.insert(SplineTransformation::XYPair(600, 0.745));
  x2y2.insert(SplineTransformation::XYPair(650, 0.805));
  x2y2.insert(SplineTransformation::XYPair(700, 0.865));
  x2y2.insert(SplineTransformation::XYPair(750, 0.925));
  x2y2.insert(SplineTransformation::XYPair(800, 0.985));
  x2y2.insert(SplineTransformation::XYPair(850, 1.046));
  x2y2.insert(SplineTransformation::XYPair(900, 1.106));
  x2y2.insert(SplineTransformation::XYPair(950, 1.166));
  x2y2.insert(SplineTransformation::XYPair(1000, 1.227));
  x2y2.insert(SplineTransformation::XYPair(1050, 1.287));
  x2y2.insert(SplineTransformation::XYPair(1100, 1.347));
  x2y2.insert(SplineTransformation::XYPair(1150, 1.408));
  x2y2.insert(SplineTransformation::XYPair(1200, 1.468));
  x2y2.insert(SplineTransformation::XYPair(1250, 1.528));
  x2y2.insert(SplineTransformation::XYPair(1300, 1.589));
  x2y2.insert(SplineTransformation::XYPair(1350, 1.649));
  x2y2.insert(SplineTransformation::XYPair(1400, 1.709));
  x2y2.insert(SplineTransformation::XYPair(1450, 1.769));
  x2y2.insert(SplineTransformation::XYPair(1500, 1.83));
  x2y2.insert(SplineTransformation::XYPair(1550, 1.89));
  x2y2.insert(SplineTransformation::XYPair(1600, 1.951));
  x2y2.insert(SplineTransformation::XYPair(1650, 2.011));
  x2y2.insert(SplineTransformation::XYPair(1700, 2.072));
  x2y2.insert(SplineTransformation::XYPair(1750, 2.132));
  x2y2.insert(SplineTransformation::XYPair(1800, 2.192));
  x2y2.insert(SplineTransformation::XYPair(1850, 2.253));
  x2y2.insert(SplineTransformation::XYPair(1900, 2.314));
  x2y2.insert(SplineTransformation::XYPair(1950, 2.374));
  x2y2.insert(SplineTransformation::XYPair(2000, 2.434));
  x2y2.insert(SplineTransformation::XYPair(2050, 2.494));
  x2y2.insert(SplineTransformation::XYPair(2100, 2.555));
  x2y2.insert(SplineTransformation::XYPair(2150, 2.615));
  x2y2.insert(SplineTransformation::XYPair(2200, 2.676));
  x2y2.insert(SplineTransformation::XYPair(2250, 2.736));
  x2y2.insert(SplineTransformation::XYPair(2300, 2.797));
  x2y2.insert(SplineTransformation::XYPair(2350, 2.846));
  x2y2.insert(SplineTransformation::XYPair(2400, 2.918));
  
  unitsToCurrentLim.init();
  SplineTransformation::XToYMap& x2y3 = unitsToCurrentReg.valueMap.left;
  x2y3.insert(SplineTransformation::XYPair(0, 0.063));
  x2y3.insert(SplineTransformation::XYPair(50, 0.09));
  x2y3.insert(SplineTransformation::XYPair(100, 0.15));
  x2y3.insert(SplineTransformation::XYPair(150, 0.211));
  x2y3.insert(SplineTransformation::XYPair(200, 0.271));
  x2y3.insert(SplineTransformation::XYPair(250, 0.331));
  x2y3.insert(SplineTransformation::XYPair(300, 0.391));
  x2y3.insert(SplineTransformation::XYPair(350, 0.452));
  x2y3.insert(SplineTransformation::XYPair(400, 0.512));
  x2y3.insert(SplineTransformation::XYPair(450, 0.572));
  x2y3.insert(SplineTransformation::XYPair(500, 0.632));
  x2y3.insert(SplineTransformation::XYPair(550, 0.692));
  x2y3.insert(SplineTransformation::XYPair(600, 0.753));
  x2y3.insert(SplineTransformation::XYPair(650, 0.813));
  x2y3.insert(SplineTransformation::XYPair(700, 0.873));
  x2y3.insert(SplineTransformation::XYPair(750, 0.933));
  x2y3.insert(SplineTransformation::XYPair(800, 0.994));
  x2y3.insert(SplineTransformation::XYPair(850, 1.054));
  x2y3.insert(SplineTransformation::XYPair(900, 1.114));
  x2y3.insert(SplineTransformation::XYPair(950, 1.174));
  x2y3.insert(SplineTransformation::XYPair(1000, 1.235));
  x2y3.insert(SplineTransformation::XYPair(1050, 1.295));
  x2y3.insert(SplineTransformation::XYPair(1100, 1.355));
  x2y3.insert(SplineTransformation::XYPair(1150, 1.416));
  x2y3.insert(SplineTransformation::XYPair(1200, 1.476));
  x2y3.insert(SplineTransformation::XYPair(1250, 1.536));
  x2y3.insert(SplineTransformation::XYPair(1300, 1.597));
  x2y3.insert(SplineTransformation::XYPair(1350, 1.657));
  x2y3.insert(SplineTransformation::XYPair(1400, 1.717));
  x2y3.insert(SplineTransformation::XYPair(1450, 1.778));
  x2y3.insert(SplineTransformation::XYPair(1500, 1.838));
  x2y3.insert(SplineTransformation::XYPair(1550, 1.899));
  x2y3.insert(SplineTransformation::XYPair(1600, 1.959));
  x2y3.insert(SplineTransformation::XYPair(1650, 2.019));
  x2y3.insert(SplineTransformation::XYPair(1700, 2.079));
  x2y3.insert(SplineTransformation::XYPair(1750, 2.14));
  x2y3.insert(SplineTransformation::XYPair(1800, 2.2));
  x2y3.insert(SplineTransformation::XYPair(1850, 2.261));
  x2y3.insert(SplineTransformation::XYPair(1900, 2.321));
  x2y3.insert(SplineTransformation::XYPair(1950, 2.382));
  x2y3.insert(SplineTransformation::XYPair(2000, 2.442));
  x2y3.insert(SplineTransformation::XYPair(2050, 2.503));
  x2y3.insert(SplineTransformation::XYPair(2100, 2.564));
  x2y3.insert(SplineTransformation::XYPair(2150, 2.624));
  x2y3.insert(SplineTransformation::XYPair(2200, 2.684));
  x2y3.insert(SplineTransformation::XYPair(2250, 2.744));
  x2y3.insert(SplineTransformation::XYPair(2300, 2.805));
  x2y3.insert(SplineTransformation::XYPair(2350, 2.865));
  x2y3.insert(SplineTransformation::XYPair(2400, 2.926));
  unitsToCurrentReg.init();
  
  SplineTransformation::XToYMap& x2y4 = unitsToPowerLim.valueMap.left;
  x2y4.insert(SplineTransformation::XYPair(0, 0));
  x2y4.insert(SplineTransformation::XYPair(3100, 190));
  unitsToPowerLim.init();
  
  SplineTransformation::XToYMap& x2y5 = unitsToPowerReg.valueMap.left;
  x2y5.insert(SplineTransformation::XYPair(0, 0));
  x2y5.insert(SplineTransformation::XYPair(3100, 190));
  unitsToPowerReg.init();
}

template <class T, class S> T save_lexical_cast(S& source, T saveDefault)
{
  try
  {
    return boost::lexical_cast<T>(source);
  }
  catch(boost::bad_lexical_cast e)
  {
    return saveDefault;
  }
  
}



RFGConnection::RFGConnection(boost::shared_ptr< boost::asio::io_service > io_service): serialAsync(io_service), Connection<serialContext>(io_service)
{
  std::cout << "--------------------->>>>>>>>>>>>>>>>> !!!!!!!!!!!!!!!!! <<<<<<<<<<<<<<<<<<<------------------------" << std::endl << std::endl << std::endl;
    this->baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    this->flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    this->character_size=boost::asio::serial_port_base::character_size(8);//unconfirmed
    this->endOfLineRegExpr_= boost::regex("\x0d");//unconfirmed
    this->parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    this->stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);
    
}


RFG::~RFG(void)
{

}

std::string RFG::composeRequest(std::string query)
{
    std::string requestString="";
    requestString+=query;
//     requestString+=std::string("\r");
    std::cout<<"R: "<<requestString<<std::endl;
    return requestString;
}



std::string RFG::interpretAnswer(std::string s)
{ 
//   s.erase( std::remove(s.begin(), s.end(), '\r'), s.end() );
//   s.erase( std::remove(s.begin(), s.end(), '\n'), s.end() );
      std::cout<<"Answer: "<<s<<std::endl;
  return s;
}

void RFG::power_supply_mode()
{
 if(bccMode==false)this->inputOutput("A"); 
}

void RFG::bcc_mode()
{
 if(bccMode==true)this->inputOutput("B"); 
}

void RFG::use_voltage_controler()
{
 this->inputOutput("F");
 this->controllerMode = ControllerMode::Voltage;
}

void RFG::use_current_controler()
{
 this->inputOutput("G");
 this->controllerMode = ControllerMode::Current;
}

void RFG::use_power_controler()
{
 this->inputOutput("H");
 this->controllerMode = ControllerMode::Power;
}

void RFG::switch_on()
{
  this->inputOutput("N");
}

void RFG::switch_off()
{
  this->inputOutput("O");
}

float RFG::getLimitMaxVoltage()
{
  return this->maxVoltage.getvalue();
}

float RFG::getLimitMinVoltage()
{
  return this->minVoltage.getvalue();
}

float RFG::getLimitMaxCurrent()
{
  return this->maxCurrent.getvalue();
}

float RFG::getTargetValue()
{
  return this->maxPower.getvalue();
}

std::string RFG::floatToBinaryStr(float f, SplineTransformation& transformation)
{
  uint16_t tmp = transformation.yToX(f); //Convert between arbitrary units and volts
  std::string request = intToBinaryStr(tmp);
  std::cout << "Converted " << f << " to " << request << std::endl;
  return request;
}

std::string RFG::intToBinaryStr(uint16_t i)
{
  char buffer[sizeof(uint16_t)];
  memcpy(buffer,&i,sizeof(uint16_t)); //Transfer binary data to char-array
  std::string rawRequest(buffer,sizeof(uint16_t));
  std::string request(rawRequest.rbegin(),rawRequest.rend()); //Reverse to keep the endian straight!
  return request;
}

int RFG::set_voltage_max_raw(int i)
{
  std::stringstream stream;
  std::string answer = this->inputOutput(std::string("\x00")+"U"+intToBinaryStr(i)+"\r",boost::regex("A\\w\\w\\w\\w"));
  boost::regex exp1("A(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
 return value;
}


float RFG::set_voltage_max(float f)
{
//   if(voltage_min>f) return 0;
  std::stringstream stream;
//   std::cout << "#### DEBUG: " << f << " - " << unitsToVoltage.yToX(f) << " - " << (uint16_t) unitsToVoltage.yToX(f) << " - " << stream.str() << std::endl;
//   union Request { char chars[2]; uint16_t value;  } request;
//   request.value = (uint16_t) (unitsToVoltage.yToX(f));
  std::string answer = this->inputOutput(std::string("\x00")+"U"+floatToBinaryStr(f,unitsToVoltageLimMax)+"\r",boost::regex("A\\w\\w\\w\\w"));
  boost::regex exp1("A(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
 return value; 
}

int RFG::set_voltage_min_raw(int i)
{
  std::stringstream stream;
  std::string answer = this->inputOutput(std::string("\x00")+"M"+intToBinaryStr(i)+"\r",boost::regex("B\\w\\w\\w\\w"));
  boost::regex exp1("B(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
 return value;
}


float RFG::set_voltage_min(float  f)
{
  std::stringstream stream;
  std::string answer = this->inputOutput(std::string("\x00")+"M"+floatToBinaryStr(f,unitsToVoltageLimMin)+"\r",boost::regex("B\\w\\w\\w\\w"));
  boost::regex exp1("B(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
 return value; 
}

int RFG::set_current_lim_raw(int i)
{
  std::stringstream stream;
  std::string answer = this->inputOutput(std::string("\x00")+"I"+intToBinaryStr(i)+"\r",boost::regex("C\\w\\w\\w\\w"));
  boost::regex exp1("C(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
  return value;
}


float RFG::set_current_lim(float  f)
{
  std::stringstream stream;
  std::string answer = this->inputOutput(std::string("\x00")+"I"+floatToBinaryStr(f,unitsToCurrentLim)+"\r",boost::regex("C\\w\\w\\w\\w"));
  boost::regex exp1("C(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
  return value; 
}

int RFG::set_target_value_raw(int i)
{
  std::stringstream stream;
  SplineTransformation* trafo;
  switch (this->controllerMode)
  {
    case ControllerMode::Voltage:
      trafo = &this->unitsToVoltageReg;
      break;
    case ControllerMode::Current:
      trafo = &this->unitsToCurrentReg;
      break;
    case ControllerMode::Power:
      trafo = &this->unitsToPowerReg;
      break;
    default:
      trafo = &this->unitsToVoltageReg;
      break;
  }
  std::string answer = this->inputOutput(std::string("\x00")+"P"+intToBinaryStr(i)+"\r",boost::regex("D\\w\\w\\w\\w"));
  boost::regex exp1("D(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
 return value;
}


float RFG::set_target_value(float f)
{
  std::stringstream stream;
  SplineTransformation* trafo;
  switch (this->controllerMode)
  {
    case ControllerMode::Voltage:
      trafo = &this->unitsToVoltageReg;
      break;
    case ControllerMode::Current:
      trafo = &this->unitsToCurrentReg;
      break;
    case ControllerMode::Power:
      trafo = &this->unitsToPowerReg;
      break;
    default:
      trafo = &this->unitsToVoltageReg;
      break;
  }
  std::string answer = this->inputOutput(std::string("\x00")+"P"+floatToBinaryStr(f,*trafo)+"\r",boost::regex("D\\w\\w\\w\\w"));
  boost::regex exp1("D(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
 return value; 
}

bool RFG::readoutChannels()
{
  int value=0;
  std::stringstream stream;
//   std::cout<<"S: "<<s<<std::endl;
  std::string s = this->inputOutput("\x00\x20\x00");
//   this->inputOutput("\r");
  std::cout<<"S: "<<s<<std::endl;
  boost::regex exp1("....(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)");
  boost::cmatch res1;
  boost::regex_search(s.c_str(), res1, exp1);
  for(int i =0;i<8;i++)
  {
    stream <<res1[i+1];
    stream >> value;
    channel_output[i].setvalue(value);
    if (this->controllerMode==ControllerMode::Voltage)channel_output[i].setunit("V");
    if (this->controllerMode==ControllerMode::Current)channel_output[i].setunit("I");
    if (this->controllerMode==ControllerMode::Power)channel_output[i].setunit("Watt");
    channel_output[i].settimestamp(boost::posix_time::second_clock::local_time());
  }
  
  return true;
}

bool RFG::readoutSetting(measuredValue& value, std::string unit, std::string controlChar, std::string answerChar, SplineTransformation& transformation, bool raw)
{
  std::string s = this->inputOutput(std::string("\x00")+controlChar+controlChar+controlChar+controlChar+std::string("\r"),boost::regex(answerChar + std::string("\\w\\w\\w\\w"))); //Provoke Error to get setting
  boost::regex exp1(answerChar + std::string("(\\w\\w\\w\\w)"));
  boost::cmatch res1;
  boost::regex_search(s.c_str(), res1, exp1);
  unsigned int valueTemp = 0;
  std::stringstream stream;
  if(!res1.empty())
  {
    stream << res1[1];
    stream >> std::hex >> valueTemp;
    if(raw)
      value.setvalue(valueTemp);
    else
      value.setvalue(transformation.xToY(valueTemp));
    std::cout << "RECEIVED: " << res1[1] << " - " << valueTemp << " -- " << exp1.str() << " - " << answerChar << " === " << value.getStringValue() << unit <<  std::endl;
    value.setunit(unit);
  }
}

bool RFG::readout(bool raw)
{
  bool result;
  try
  {
    SplineTransformation* trafo;
    std::string targetUnit;
    switch (this->controllerMode)
  {
    case ControllerMode::Voltage:
      trafo = &this->unitsToVoltageReg;
      targetUnit = std::string("V");
      break;
    case ControllerMode::Current:
      trafo = &this->unitsToCurrentReg;
      targetUnit = std::string("A");
      break;
    case ControllerMode::Power:
      trafo = &this->unitsToPowerReg;
      targetUnit = std::string("W");
      break;
    default:
      trafo = &this->unitsToVoltageReg;
      targetUnit = std::string("V");
      break;
  }
    result = this->readoutChannels() && result;
    result = this->readoutSetting(this->maxVoltage,"V","U","A",this->unitsToVoltageLimMax,raw) && result;
    result = this->readoutSetting(this->minVoltage,"V","M","B",this->unitsToVoltageLimMin,raw) && result;
    result = this->readoutSetting(this->maxCurrent,"A","I","C",this->unitsToCurrentLim,raw) && result;
    result = this->readoutSetting(this->maxPower,targetUnit,"P","D",*trafo,raw) && result;
  }
  catch(...)
  {
    return false;
  }
  return result;
}


measuredValue RFG::get_channel(int i, bool force)
{
 if(!force &&!channel_output[0].gettimestamp().is_not_a_date_time()&& channel_output[0].gettimestamp() > boost::posix_time::second_clock::local_time()-boost::posix_time::seconds(3))
  {
    return channel_output[i];
  }
  
  this->readout();
 return channel_output[i];
}


void RFG::initImplementation()
{
if(isConnected())
  std::cout << "RFG answered correctly! YAY!!!!" << std::endl;
this->bccMode=true;
this->controllerMode=ControllerMode::Voltage;
  
}
    
bool RFG::isConnectedImplementation()
{
  std::string initStr = this->inputOutput(std::string("\x00")+std::string("AF")+std::string("\r"),boost::regex("@"));
  this->connected = initStr == std::string("@");
  return this->connected;
}


void RFG::shutdownImplementation()
{
}

