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


class A 
{
public:
  int zahl;
  std::string str;
  
  A(int i, std::string s)
  {
    this->zahl = i;
    this->str = s;
  }
  
};

double mult(double a, double b)
{
  return a * b;
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
    std::cout << "Test1: " << eI.show() << ": " << eI << std::endl;
    ExposedFunction<double, double, double> eMult(&mult,"mult");
    std::cout << "Test2: " << eMult(2,3) << std::endl;
  }
  catch(lughos::exception e)
  {
    std::cerr << lughos::makeErrorReport(e);
  }
  
    return 0;
}
