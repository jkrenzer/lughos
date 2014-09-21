#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <map>
#include "basicTypes.hpp"

using namespace lughos;





// template <class T> valueDeclaration<T>& _valueDeclaration(valueDeclaration<T> &d)
// {
//   return d;
// }
// 
// template <class T> exposedValue<T>& _getExposedValue(exposedValue<T> &d)
// {
//   return d;
// }

int main(int argc, char **argv) {
  try 
  {
    //Test exposition of variable via pointer
    std::cout << "Testing exposed pointers" << std::endl;
    int zahl = 7;
    exposedPtr<int>* test = new exposedPtr<int>(&zahl,std::string("Zahl"),std::string("Irgendeine Zahl"));
    std::cout << _valueDeclaration(*test).getTypeName() << std::endl;
    exposedObject* o = test;
    std::cout << _valueDeclaration(*test).getTypeName() << std::endl;
  
    
    
    
    
    
     
    

  }
  catch(lughos::exception e)
  {
    std::cerr << lughos::makeErrorReport(e);
  }
    return 0;
}
