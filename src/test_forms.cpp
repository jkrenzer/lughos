#include <iostream>
#include "basicTypes.hpp"
#include "io.hpp"
#include "transformations.hpp"

using namespace lughos;

int main(int argc, char **argv) {
  try 
  {
    //Test exposition of variable via pointer
    std::cout << "Testing form output" << std::endl;
    int zahl = 7;
    exposedPtr<int>* test = new exposedPtr<int>(&zahl,std::string("Zahl"),std::string("Irgendeine Zahl"));
    
    exposedValue<int> i(7);
    exposedValue<double> d(13.7);
    
    std::cout << i.getValue() << " " << d.getValue() << std::endl;
//     ioRenderer<consoleContext> renderer;
//     renderer.render(*test);
    
    textBox<consoleContext> box1(i);
    textBox<consoleContext> box2(d);
    std::cout << "Outputting box1 and box2..." << std::endl;
    std::cout << box1.output() << std::endl;
    std::cout << box2.output() << std::endl;
    std::cout << "Inputting box1 and box2..." << std::endl;
    std::cout << "New Value Box1 (" << box1.output() << ") :";
    std::string a1;
    std::cin >> a1;
    std::cin.get();
    std::cout << std::endl;
    std::cout << "New Value Box2 (" << box2.output() << ") :";
    std::string a2;
    std::cin >> a2;
    std::cout << std::endl;
    box1.input(a1);
    box2.input(a2);
    std::cout << "Outputting box1 and box2..." << std::endl;
    std::cout << box1.output() << std::endl;
    std::cout << box2.output() << std::endl;
    
  }
  catch(lughos::exception e)
  {
    std::cerr << lughos::makeErrorReport(e);
  }
    return 0;
}
