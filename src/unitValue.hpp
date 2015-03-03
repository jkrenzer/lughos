#ifndef UNITVALUE_HPP
#define UNITVALUE_HPP



#include <string>
#include <cstring>
#include <iostream>
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
	double getvalue() const;
	std::string getStringValue() const;
	std::string getString() const;
	void setStringValue(std::string str);
	std::string getunit() const;
	void setvalue(double putvalue);
	void setunit(std::string putunit);
	bool isPositiveInfinity();
	bool isNegativeInfinity();
	bool isNotANumber();
	bool isValidValue();
	
	unitValue operator*(double d);
	unitValue operator+(double d);
	unitValue operator-(double d);
	unitValue operator/(double d);
	
	operator double();


protected:
	double value=0.0;
	std::string unit="";
	
};



#endif