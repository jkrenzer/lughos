// #include "StdAfx.h"

#include <ostream>
#include "serialConnections.hpp"
#include "coolpak6000.hpp"


coolpak6000::coolpak6000()
{
 
  set_default();

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

template <class T> void coolpak6000::setDefaultImpl(T& connection)
{
}

template <> void coolpak6000::setDefaultImpl< Connection<serialContext> > (Connection<serialContext>& connection)
{
  
    connection.baud_rate=boost::asio::serial_port_base::baud_rate(4800);
    connection.flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    connection.character_size=boost::asio::serial_port_base::character_size(8);
    connection.endOfLineRegExpr_='\r';
    connection.parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    connection.stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);
  
}

coolpak6000::~coolpak6000(void)
{

}

std::string coolpak6000::composeRequest(std::string query)
{

    std::string requestString="";
    requestString+=std::string("\x02");
    requestString+=query;
    requestString+=std::string("\r");
std::cout<<query<<std::endl;
    return requestString;
  
}


   void coolpak6000::set_default()
{
   this->setDefaultImpl(*(this->connection.get()));
}

std::string coolpak6000::interpretAnswer(std::string s)
{  
  std::cout<<"answer: "<<s<<std::endl;
  return s;  

}

   bool coolpak6000::compressor_on()
{
  std::string response=this->inputOutput("SYS1");
  if(response.c_str()=="SYS1")return true;
  else if(response.c_str()=="SYS2")return false;
  else return false; 
  
}

   bool coolpak6000::compressor_off()
{
  std::string response=this->inputOutput("SYS0");
  if(response.c_str()=="SYS0")return true;
  else if(response.c_str()=="SYS2")return false;
  else return false; 
  
}

   std::string coolpak6000::get_data()
{

	std::string  s =this->inputOutput("DAT");

  
//   	 static const boost::regex e("\\/(.*)\\/");
	 static const boost::regex e("([^/]*)/([^/]*)/([^/]*)/([^/]*)/([^/]*)/([^/]*)/([^/]*)/([^/]*)/([^/]*)/([^/]*)/([^/]*)/([^/]*)/([^/]*)/([^/]*)");

	 boost::cmatch res;
	 boost::regex_search(s.c_str(), res, e);
	 
	 software_version= res[1];
	 operating_hours.setUnitvalue(save_lexical_cast<double>(res[3],-1), "h");
	 delay.setUnitvalue(save_lexical_cast<double>(res[7],-1), "sec");
	 compressor_state=save_lexical_cast<int>(res[8],-1);
	 command_state=save_lexical_cast<int>(res[9],-1);
	 coolhead1_state=save_lexical_cast<int>(res[10],-1);
	 coolhead2_state=save_lexical_cast<int>(res[11],-1);
	 number_of_errors= save_lexical_cast<int>(res[12],-1);
	std::string errorstring =res[13];
	int number_of_saved_errors = save_lexical_cast<int>(res[14],-1);
// 	 std::cout<<res[1]<<std::endl;
//  for(int i=1; i<2; i++)std::cout<<res[i]<<std::endl;
  return s;
  
}

   std::string coolpak6000::get_error_list()
{
  return this->inputOutput("ERR");
  
}

   bool coolpak6000::coolhead_on(int head)
{
 
  std::string request1("SC" +std::to_string(head)+std::to_string(1));
   
  std::string response=this->inputOutput(request1);
  if(response.c_str()==request1.c_str())return true;
  else return false; 
  
}

   bool coolpak6000::coolhead_off(int head)
{
  std::string request("SC" +std::to_string(head)+std::to_string(0));
   
  std::string response=this->inputOutput(request);
  if(response.c_str()==request.c_str())return true;
  else return false; 
  
}

std::string coolpak6000::get_software_version()
{
  get_data();
  return software_version;
}

int coolpak6000::get_compressor_state()
{
  get_data();
  return compressor_state;
}

int coolpak6000::get_command_state()
{
  get_data();
  return command_state;
}
int coolpak6000::get_coolhead1_state()
{
  get_data();
  return coolhead1_state;
}
int coolpak6000::get_coolhead2_state()
{
  get_data();
  return coolhead2_state;
}
int coolpak6000::get_number_of_errors()
{
  get_data();
  return number_of_errors;
}
std::string coolpak6000::get_errorstring(){
  get_data();
  return errorstring;
}
int coolpak6000::get_number_of_saved_errors(){
  get_data();
  return number_of_saved_errors;
}

unitValue coolpak6000::get_delay(){
  get_data();
  unitValue del =delay;
  return del;
}

unitValue coolpak6000::get_operating_hours(){
  get_data();
  unitValue op=operating_hours;
  return op;
}

void coolpak6000::initImplementation()
{
}
    

void coolpak6000::shutdownImplementation()
{
}