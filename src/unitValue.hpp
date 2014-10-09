#ifndef UNITVALUE_HPP
#define UNITVALUE_HPP



#include <string>
#include <cstring>
#include <iostream>
#include <ctime>
// #include "Dict.hpp"


class unitValue 
{
  private:
// 	unitValue(const unitValue &p);
// 	unitValue &operator=(const unitValue &p);
	

  public:
	unitValue(void);
	unitValue(double putvalue, std::string putunit);
	~unitValue(void);


	void setunitvalue(double putvalue, std::string putunit);
	void setunitvalue(int putvalue, std::string putunit);
	double getvalue();
	std::string getunit();
	void setvalue(double putvalue);
	void setunit(std::string putunit);
	std::time_t gettimestamp();

protected:
	double value=0.0;
	std::string unit="";
// 	std::time_t  timestamp;
};



#endif