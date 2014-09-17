#include <iostream>
#include "basicTypes.hpp"
#include "guiForms.hpp"

using namespace lughos;

int multiply(int x, int y)
{
  return x * y;
}

class multiplyW : public exposedFunction<int>
{
public:
  multiplyW(std::string name, std::string description = "") : exposedFunction< int >(name,description)
  {}
  
  bool runable()
  {
    return true;
  }
  
  int exec()
  {
    return multiply(this->getValue<int>(children[0]),this->getValue<int>(children[1]));
  }
};

int main(int argc, char **argv) {
  try 
  {
    //Test exposition of variable via pointer
    std::cout << "Testing form output" << std::endl;
    int zahl = 7;
    exposedPtr<int>* test = new exposedPtr<int>(&zahl,std::string("Zahl"),std::string("Irgendeine Zahl"));
    form<consoleContext> f;
    f.addChild(test);
    f.render();
    
  }
  catch(lughos::exception e)
  {
    std::cerr << lughos::makeErrorReport(e);
  }
    return 0;
}
