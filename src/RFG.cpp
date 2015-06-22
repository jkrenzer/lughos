// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "RFG.hpp"

RFG::RFG() :  voltage("voltage"), current("current"), power("power"), temperature("temperature"),
              voltageLimitMax("voltageLimitMax"), voltageLimitMin("voltageLimitMin"), 
              currentLimitMax("currentLimitMax"), mode("mode"), output("output"), 
              controller("controller"), resistanceCorrection("resistanceCorrection"), 
              target("target"), bccOutputSignal("bccOutputSignal"), bccFeedbackSignal("bccFeedbackSignal"),
              aux1("aux1"),aux2("aux2")
{
  this->voltage.refresher(boost::bind(&RFG::readoutChannels,this));
  this->current.refresher(boost::bind(&RFG::readoutChannels,this));
  this->power.refresher(boost::bind(&RFG::readoutChannels,this));
  this->temperature.refresher(boost::bind(&RFG::readoutChannels,this));
  this->aux1.refresher(boost::bind(&RFG::readoutChannels,this));
  this->aux2.refresher(boost::bind(&RFG::readoutChannels,this));
  this->voltageLimitMax.getter(boost::bind(&RFG::getLimitMaxVoltage,this));
  this->voltageLimitMax.setter(boost::bind(&RFG::set_voltage_max,this,_1));
  this->voltageLimitMin.getter(boost::bind(&RFG::getLimitMinVoltage,this));
  this->voltageLimitMin.setter(boost::bind(&RFG::set_voltage_min,this,_1));
  this->currentLimitMax.getter(boost::bind(&RFG::getLimitMaxCurrent,this));
  this->currentLimitMax.setter(boost::bind(&RFG::set_current_lim,this,_1));
  this->target.getter(boost::bind(&RFG::getTargetValue,this));
  this->target.setter(boost::bind(&RFG::set_target_value,this,_1));
  this->mode.setter(boost::bind(&RFG::set_mode,this,_1));
  this->mode.setValue(RFG::Mode::Powersupply);
  this->mode.expires(false);
  this->output.setter(boost::bind(&RFG::set_output,this,_1));
  this->output.expires(false);
  this->output.setValue(false);
  this->controller.setter(boost::bind(&RFG::set_controller,this,_1));
  this->controller.setValue(RFG::Controller::Voltage);
  this->controller.expires(false);
  this->resistanceCorrection.setValue(0.139);
  this->resistanceCorrection.setUnit("Ohm");
  this->resistanceCorrection.expires(false);
  this->bccFeedbackSignal.refresher(boost::bind(&RFG::readoutChannels,this));
  this->bccOutputSignal.refresher(boost::bind(&RFG::readoutChannels,this));
  ExclusiveLock lock(mutex);
  for (int i=0;i<8;i++)
    {
      channel_output[i].setValueAndUnit(0,"");
    }
    
  unitsToVoltageReg.fromFile("calibration_voltage_DAC.csv");

  SplineTransformation::XToYMap& x2y1a = unitsToVoltageLimMax.valueMap.left;
  x2y1a.insert(SplineTransformation::XYPair(0,     0.22));
  x2y1a.insert(SplineTransformation::XYPair(100,   1.436));
  x2y1a.insert(SplineTransformation::XYPair(200,   2.649));
  x2y1a.insert(SplineTransformation::XYPair(300,   3.861));
  x2y1a.insert(SplineTransformation::XYPair(400,   5.073));
  x2y1a.insert(SplineTransformation::XYPair(500,   6.286));
  x2y1a.insert(SplineTransformation::XYPair(600,   7.499));
  x2y1a.insert(SplineTransformation::XYPair(700,   8.712));
  x2y1a.insert(SplineTransformation::XYPair(800,   9.925));
  x2y1a.insert(SplineTransformation::XYPair(900,  11.137));
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
  x2y1b.insert(SplineTransformation::XYPair(0,     0.154));
  x2y1b.insert(SplineTransformation::XYPair(100,   1.37));
  x2y1b.insert(SplineTransformation::XYPair(200,   2.583));
  x2y1b.insert(SplineTransformation::XYPair(300,   3.796));
  x2y1b.insert(SplineTransformation::XYPair(400,   5.009));
  x2y1b.insert(SplineTransformation::XYPair(500,   6.221));
  x2y1b.insert(SplineTransformation::XYPair(600,   7.434));
  x2y1b.insert(SplineTransformation::XYPair(700,   8.646));
  x2y1b.insert(SplineTransformation::XYPair(800,   9.859));
  x2y1b.insert(SplineTransformation::XYPair(900,  11.072));
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
  
  unitsToCurrentReg.fromFile("calibration_current_DAC.txt");
    
  SplineTransformation::XToYMap& x2y4 = unitsToPowerMeas.valueMap.left;
  x2y4.insert(SplineTransformation::XYPair(0, 0));
  x2y4.insert(SplineTransformation::XYPair(3100, 190));
  unitsToPowerMeas.init();
  
  SplineTransformation::XToYMap& x2y5 = unitsToPowerReg.valueMap.left;
  x2y5.insert(SplineTransformation::XYPair(0, 0));
  x2y5.insert(SplineTransformation::XYPair(3100, 190));
  unitsToPowerReg.init();
  
  unitsToVoltageMeas.fromFile("calibration_voltage_ADC.csv");
  
  unitsToCurrentMeas.fromFile("calibration_current_ADC.csv");
  
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

void RFG::memberDeclaration()
{
  ExclusiveLock lock(mutex);
  addMember(voltage);
  addMember(current);
  addMember(power);
  addMember(target);
  addMember(currentLimitMax);
  addMember(voltageLimitMax);
  addMember(voltageLimitMin);
  addMember(controller);
  addMember(mode);
  addMember(output);
  addMember(resistanceCorrection);
}


RFGConnection::RFGConnection()
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
//   ExclusiveLock lock(mutex);
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

void RFG::set_mode(RFG::Mode mode)
{
  if(mode == RFG::Mode::Powersupply)
    this->inputOutput("A");
  else
    this->inputOutput("B");
}

void RFG::set_controller(RFG::Controller controller)
{
  switch (controller)
  {
    case RFG::Controller::Current: this->inputOutput("G"); break;
    case RFG::Controller::Power: this->inputOutput("H"); break;
    default:
    case RFG::Controller::Voltage: this->inputOutput("F"); break;
  }
}

void RFG::set_output(bool state)
{
  if(state)
    this->inputOutput("N");
  else
  this->inputOutput("O");
}

measuredValue<double> RFG::getLimitMaxVoltage()
{
  measuredValue<double> value;
  unsigned int valueTemp = readoutSetting("U","A");
  value.setValue(unitsToVoltageLimMax.xToY(valueTemp));
  return value;
}

measuredValue<double> RFG::getLimitMinVoltage()
{
  measuredValue<double> value;
  unsigned int valueTemp = readoutSetting("M","B");
  value.setValue(unitsToVoltageLimMin.xToY(valueTemp));
  return value;
}

measuredValue<double> RFG::getLimitMaxCurrent()
{
  measuredValue<double> value;
  unsigned int valueTemp = readoutSetting("I","C");
  value.setValue(unitsToCurrentLim.xToY(valueTemp));
  return value;
}

measuredValue<double> RFG::getTargetValue()
{
  measuredValue<double> value;
  unsigned int valueTemp = readoutSetting("P","D");
  switch (this->controller)
  {
    case Controller::Voltage: value.setValue(unitsToVoltageReg.xToY(valueTemp)); value.setUnit("V"); break;
    case Controller::Current: value.setValue(unitsToCurrentReg.xToY(valueTemp)); value.setUnit("A"); break;
    case Controller::Power:   value.setValue(unitsToPowerReg.xToY(valueTemp)); value.setUnit("W"); break;
  }
  return value;
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

void RFG::set_voltage_max_raw(int i)
{
  std::stringstream stream;
  std::string answer = this->inputOutput(std::string("\x00")+"U"+intToBinaryStr(i)+"\r",boost::regex("A\\w\\w\\w\\w"));
  boost::regex exp1("A(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
  //TODO
}


void RFG::set_voltage_max(double f)
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
  //TODO
}

void RFG::set_voltage_min_raw(int i)
{
  std::stringstream stream;
  std::string answer = this->inputOutput(std::string("\x00")+"M"+intToBinaryStr(i)+"\r",boost::regex("B\\w\\w\\w\\w"));
  boost::regex exp1("B(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
  //TODO
}


void RFG::set_voltage_min(double  f)
{
  std::stringstream stream;
  std::string answer = this->inputOutput(std::string("\x00")+"M"+floatToBinaryStr(f,unitsToVoltageLimMin)+"\r",boost::regex("B\\w\\w\\w\\w"));
  boost::regex exp1("B(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
 //TODO
}

void RFG::set_current_lim_raw(int i)
{
  std::stringstream stream;
  std::string answer = this->inputOutput(std::string("\x00")+"I"+intToBinaryStr(i)+"\r",boost::regex("C\\w\\w\\w\\w"));
  boost::regex exp1("C(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
  //TODO
}


void RFG::set_current_lim(double f)
{
  std::stringstream stream;
  std::string answer = this->inputOutput(std::string("\x00")+"I"+floatToBinaryStr(f,unitsToCurrentLim)+"\r",boost::regex("C\\w\\w\\w\\w"));
  boost::regex exp1("C(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
  //TODO 
}

void RFG::set_target_value_raw(int i)
{
  std::stringstream stream;

  std::string answer = this->inputOutput(std::string("\x00")+"P"+intToBinaryStr(i)+"\r",boost::regex("D\\w\\w\\w\\w"));
  boost::regex exp1("D(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
  //TODO
}


void RFG::set_target_value(double setpoint)
{
  std::stringstream stream;
   SplineTransformation* trafo;
  switch (this->controller.getValue())
  {
    case Controller::Voltage:
      trafo = &this->unitsToVoltageReg;
      break;
    case Controller::Current:
      trafo = &this->unitsToCurrentReg;
      break;
    case Controller::Power:
      trafo = &this->unitsToPowerReg;
      break;
    default:
      trafo = &this->unitsToVoltageReg;
      break;
  }
  std::string answer = this->inputOutput(std::string("\x00")+"P"+floatToBinaryStr(setpoint,*trafo)+"\r",boost::regex("D\\w\\w\\w\\w"));
  boost::regex exp1("D(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
  //TODO Throw when unable to set.
}

bool RFG::readoutChannels()
{
  int value=0;
  std::stringstream stream;
//   std::cout<<"S: "<<s<<std::endl;
  std::string s = this->inputOutput("\x20");
  boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
//   this->inputOutput("\r");
  std::cout << std::dec << "###>>> Answer Size: "<< s.size() <<std::endl;
  boost::match_results<std::string::iterator> res1;
  try
  {
    boost::regex exp1("L([.\\s\\S]{5})([.\\s\\S]{24})\r");
    boost::regex_search(s.begin(), s.end(), res1, exp1);
  }
  catch(...)
  {
    std::cout << "###>>> Regex exception fired!" << std::endl;
  }
  s = res1[2];
  std::cout << "###>>> Got answer with " << s.size() << " characters and " << res1.size() << " matching-groups. (" << res1[1] << ") (" << res1[2] << ")" << std::endl;
  if (s.size() < 24)
    return false;
  std::vector<int> results;
  std::string tmp("");
  int ii = 0;
  for(int i =0;i<8;i++)
  {
    ii = i*3;
    tmp.clear();
    tmp += s[ii+2]; 
    tmp += s[ii+1];
    tmp += s[ii];
    tmp += '\x00'; //Do we need this? And is it the right place?
    results.push_back(0);
    results[i] = 0;
    memcpy(&results[i],tmp.c_str(),tmp.size());
//     channel_output[i].setTimeStamp(now);
//     this->channel_output_raw[i] = results[i];
  }
  double rawVoltage = unitsToVoltageMeas.xToY(results[0]);
  double rawCurrent = unitsToCurrentMeas.xToY(results[1]);
  double rawPower = unitsToPowerMeas.xToY(results[2]);
  ExclusiveLock lock(mutex);
  this->voltage.setValueAndUnit(rawVoltage - ( this->resistanceCorrection.getValue() * rawCurrent),"V"); //Voltage thevenin-correction
  this->current.setValueAndUnit(rawCurrent,"A");
  this->power.setValueAndUnit(rawPower,"W");
  this->bccOutputSignal.setValueAndUnit(results[3],""); //Regler Out
  this->bccFeedbackSignal.setValueAndUnit(results[4],""); //Regler Feedback
  this->aux1.setValueAndUnit(results[5],"Aux1");
  this->aux2.setValueAndUnit(results[6],"Aux2");
  this->temperature.setValueAndUnit(results[7],"°C");
//   channel_output[0].setValueAndUnit(rawVoltage - ( this->resistanceCorrection.getValue() * rawCurrent),"V"); //Voltage thevenin-correction
//   channel_output[1].setValueAndUnit(rawCurrent,"A");
//   channel_output[2].setValueAndUnit(results[2],"W");
//   channel_output[3].setValueAndUnit(results[3],""); //Regler Out
//   channel_output[4].setValueAndUnit(results[4],""); //Regler Feedback
//   channel_output[5].setValueAndUnit(results[5],"Aux1");
//   channel_output[6].setValueAndUnit(results[6],"Aux2");
//   channel_output[7].setValueAndUnit(results[7],"°C");
//   std::cout << "RFG Channel Measurements:" << std::endl;
//   for (int i = 0; i < 8; i++)
//   {
//     std::cout << "Channel " << i << ": " << channel_output[i].getValueAsString() << " (" << results[i] << ") " << std::endl;
//   }
//   std::cout << "-------------------------" << std::endl << std::endl;
  
  return true;
}

unsigned int RFG::readoutSetting(std::string controlChar, std::string answerChar)
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
    std::cout << "RECEIVED: " << res1[1] << " - " << valueTemp <<  std::endl;
    s = this->inputOutput(std::string("\x00")+controlChar+intToBinaryStr(valueTemp)+std::string("\r"),boost::regex(answerChar + std::string("\\w\\w\\w\\w")));
  }
}



measuredValue<double> RFG::get_channel(int i, bool force)
{
 if(!force &&!channel_output[0].getTimeStamp().is_not_a_date_time()&& channel_output[0].getTimeStamp() > boost::posix_time::second_clock::local_time()-boost::posix_time::seconds(1))
  {
    return channel_output[i];
  }
  
  this->readoutChannels();
 return channel_output[i];
}

int RFG::get_channel_raw(int i, bool force)
{
  if(!force &&!channel_output[0].getTimeStamp().is_not_a_date_time()&& channel_output[0].getTimeStamp() > boost::posix_time::second_clock::local_time()-boost::posix_time::seconds(1))
  {
    return channel_output_raw[i];
  }
  
  this->readoutChannels();
 return channel_output_raw[i];
}


void RFG::initImplementation()
{
if(isConnected())
  std::cout << "RFG answered correctly! YAY!!!!" << std::endl;
}
    
bool RFG::isConnectedImplementation()
{
  std::string initStr = this->inputOutput(std::string("\x00")+std::string("AF")+std::string("\r"),boost::regex("@"));
  LUGHOS_LOG(log::SeverityLevel::informative) << std::string("RFG is testing connection. Answer was: ") << initStr;
  return initStr.substr(0,1) == std::string("@");
}


void RFG::shutdownImplementation()
{
}

