#include <iostream>
#include <iomanip>
#include <string>
#include <boost/regex.hpp>

using namespace std;

int main()
{
  string str("Blah/\nBlubb/Bell\n/Gurgel");
  const int subs[] = {1};
  boost::regex r("(\n)");
  cout << "Analyzing: " << endl << str << endl;
  cout << "Found matches:" << endl;
  boost::sregex_token_iterator j;
  for(boost::sregex_token_iterator i(str.begin(), str.end(), r, subs); i !=j; i++) 
    cout << "Match: " << *i << endl;
  char buffer [50];
  sprintf (buffer, "%A",10.13);
  std::cout << "And a hex: " << buffer << std::endl;
}