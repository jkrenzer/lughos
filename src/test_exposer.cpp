#include <iostream>
#include <boost/concept_check.hpp>
#include "basicTypes.hpp"

using namespace exposer;

int multiply(int x, int y)
{
  return x * y;
}

class multiplyW : exposedFunction<int>
{
  bool runable()
  {
    return true;
  }
  
  int exec()
  {
    return multiply(this->getValue<int>(children[0]),this->getValue<int>(children[0]));
  }
};

int main(int argc, char **argv) {
  try 
  {
    int zahl = 7;
    exposedPtr<int>* test = new exposedPtr<int>(&zahl,std::string("Zahl"),std::string("Irgendeine Zahl"));
    ioRenderer<consoleContext> renderer;
    renderer.output<int>(*test);
    renderer.input<int>(*test);
    renderer.output<int>(*test);
    test->getValue();
  }
  catch(exposer::exception e)
  {
    std::cerr << exposer::makeErrorReport(e);
  }
    return 0;
}
