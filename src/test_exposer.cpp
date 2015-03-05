#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <map>
#include "basicTypes.hpp"
#include "exposedClasses.hpp"
#include "exposedValues.hpp"
#include "exposedFunctions.hpp"
#include "errorHandling.hpp"

using namespace lughos;


class A : public ExposedClass
{
public:
  int zahl;
  std::string str;
  
  A(int i, std::string s)
  {
    this->zahl = i;
    this->str = s;
  }
  
  void output(std::string str1)
  {
    std::cout << zahl << " ### " << str1 << std::endl;
  }
  
  void memberDeclaration()
  {
    addMember(zahl,"zahl");
    addMember(str,"str");
    addMember(new ExposedFunction<void>(boost::bind(&A::output,this,_1),"output"));
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

int main(int argc, char **argv) {
  try 
  {
    int i = 42;
    ExposedValue<int> eI(i,"i");
    eI.onValueChange.connect(&petze<int>);
    eI.beforeValueChange.connect(&limiter);
    std::cout << "Test1: " << eI.showStructure() << ": " << eI << std::endl;
    eI = 127;
    eI = 512;
    eI = -1;
    eI = 123;
    ExposedFunction<double, double, double> eMult(&mult,"mult");
    eMult.onExecute.connect(&petze<double>);
    std::cout << "Test2: " << eMult(2,3) << std::endl;
    A a(254,"Klasse!");
    a["output"]->parse("\"Hallo Welt!\"");
  }
  catch(lughos::exception e)
  {
    std::cerr << lughos::makeErrorReport(e);
  }
  
    return 0;
}
