#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <map>
#include "basicTypes.hpp"
#include "exposedClasses.hpp"
#include "exposedValues.hpp"

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

namespace lughos
{

template <> class ExposedClass<A> : public ExposedClassImplementation<A>
{
public:
  
  ExposedClass(A c)
  {this->exposeClass(c);}
  
  void exposeClass(A c)
  {
    this->addChild(new Value<int>(c.zahl));
    this->addChild(new Value<std::string>(c.str));
  }
    
};

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
    //Test exposition of variable via pointer
    std::cout << "Testing raw pointers" << std::endl;
    int zahl = 7;
    ExposedPointer<int>* test = new ExposedPointer<int>(&zahl,std::string("Zahl"));
    std::cout << test->getAs<Value<int> >("value")->getTypeName() << std::endl;
    A a(12345,std::string("Test der Klasse"));
    ExposedClass< A > eA(a);
    std::cout << eA.getAs<ValueInterface>(0)->getValueAsString() << std::endl;
    std::cout << eA.getAs<ValueInterface>(1)->getValueAsString() << std::endl;
    std::cout << "Testing Exposed objects" << std::endl;
    ExposedObject eO;
    eO.setName("Testobject");
    std::cout << "Name of the object: " << eO.getAs<Values>("name")->getValueAsString() << std::endl;
    eO.addChild(new Value<int>(12345,"Ne Zahl"));
    std::cout << eO.getAs<Values>(2)->getName() << " " << eO.getAs<Values>(2)->getValueAsString() << std::endl;
    eO.removeChild(eO.get(2));
    eO.addChild(new Value<int>(54321,"Ne andere Zahl"));
    std::cout << eO.getAs<Values>(2)->getName() << " " << eO.getAs<Values>(2)->getValueAsString() << std::endl;
    delete test;
  }
  catch(lughos::exception e)
  {
    std::cerr << lughos::makeErrorReport(e);
  }
  
    return 0;
}
