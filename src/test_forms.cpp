#include <iostream>
#include "basicTypes.hpp"
#include "io.hpp"
#include "guiForms.hpp"

using namespace lughos;

int main(int argc, char **argv) {
  try 
  {
    //Test exposition of variable via pointer
    std::cout << "Testing form output" << std::endl;
    int zahl = 7;
    exposedPtr<int>* test = new exposedPtr<int>(&zahl,std::string("Zahl"),std::string("Irgendeine Zahl"));
    renderValue<consoleContext,int> r(*test);
    testRenderer* rendr = new testRenderer() ;
    r.output();
    r.setRenderer(rendr);
    r.output();
    
    form <consoleContext> f;
    formElements fe(*test);
    f.addChild(fe);
    f.render();
    
  }
  catch(lughos::exception e)
  {
    std::cerr << lughos::makeErrorReport(e);
  }
    return 0;
}
