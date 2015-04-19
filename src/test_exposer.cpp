#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <map>
#include "basicTypes.hpp"
#include "exposedClasses.hpp"
#include "exposedValues.hpp"
#include "exposedFunctions.hpp"
#include "errorHandling.hpp"
#include "exposedMeasuredValues.hpp"

using namespace lughos;


class A : public ExposedClass
{
public:
  int zahl;
  std::string str;
  exposedMeasurement<double> meas;
  
  A(int i, std::string s) : meas("xxx")
  {
    this->zahl = i;
    this->str = s;
  }
  
  void output(int i, std::string str1)
  {
    std::cout << i << " ### " << str1 << std::endl;
  }
  
  void memberDeclaration()
  {
    addMember(zahl,"zahl");
    addMember(str,"str");
    addMember(new ExposedFunction<void,int,std::string>(boost::bind(&A::output,this,_1,_2),"output"));
    this->meas.setName("Measurement 1");
    addMember(meas);
  }
  
};

double mult(double a, double b)
{
  return a * b;
}

template <class T>
void petze(T t)
{
  std::cout << "PETZE: " << t << std::endl;
}

bool limiter(int i)
{
  return i < 256 && i >= 0;
}

namespace lughos
{

// template <> class ExposedClass<A> : public ExposedClassImplementation<A>
// {
// public:
//   
//   ExposedClass(A c)
//   {this->exposeClass(c);}
//   
//   void exposeClass(A c)
//   {
//     this->addChild(new Value<int>(c.zahl));
//     this->addChild(new Value<std::string>(c.str));
//   }
//     
// };

}//namespace lughos

// template <class T> ValueDeclaration<T>& _ValueDeclaration(ValueDeclaration<T> &d)
// {
//   return d;
// }
// 
// template <class T> Value<T>& _getExposedValue(Value<T> &d)
// {
//   return d;
// }

void logger(std::string str)
{
  std::cout << "LOG: " << str << std::endl;
}

double getter()
{
  return 123.456;
}

void setter(double d)
{
  std::cout << "Setting setter to " << d << std::endl;
}

int main(int argc, char **argv) {
  try 
  {
    int i = 42;
    ExposedValue<int> eI(i,"i");
    eI.onValueChange.connect(&petze<int>);
    eI.beforeValueChange.connect(&limiter);
    std::cout << "Test1: " << eI.showStructure() << ": " << eI.getValueAsString() << std::endl;
    eI = 127;
    eI = 512;
    eI = -1;
    eI = 123;
    ExposedFunction<double, double, double> eMult(&mult,"mult");
    eMult.onExecute.connect(&petze<double>);
    std::cout << "Test2: " << eMult(2,3) << std::endl;
    A a(254,"Klasse!");
    a["output"]->parse("1234 \"Hallo Welt!\"");
    std::cout << "Zahl: " << a.getValue<int>("zahl") << std::endl;
    exposedMeasurement<double>& m1 = *(a.get<exposedMeasurement<double> >("Measurement 1")); //TODO Write constructors
    double test = 0.0;
//     m1.onRequestValue.connect(boost::bind(&logger,std::string("Requested value of m1")));
    m1.getter(&getter);
    m1.setter(&setter);
//     m1.onValueChange.connect(boost::bind(&logger,std::string("Set value of m1")));
    m1.setValue(987.654);
    m1.setUnit("blah");
    boost::posix_time::time_duration corr = boost::posix_time::microsec_clock::local_time() - boost::posix_time::microsec_clock::local_time();
    std::cout << "Time needed for allocation of timestamp: " << corr << std::endl;
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    double v1 = m1.getValue();
    boost::posix_time::time_duration lapse1 = now - boost::posix_time::microsec_clock::local_time();
    std::cout << "Measured value: " << v1 << " Time: " << lapse1 <<  std::endl;
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    now = boost::posix_time::microsec_clock::local_time();
    v1 = m1.getValue();
    lapse1 = now - boost::posix_time::microsec_clock::local_time();
    std::cout << "Measured value: " << v1 << " Time: " << lapse1 <<  std::endl;
  }
  catch(lughos::exception e)
  {
    std::cerr << lughos::makeErrorReport(e);
  }
  
    return 0;
}
