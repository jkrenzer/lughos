#include <iostream>
#include <string>
#include <boost/regex.hpp>

using namespace std;

int main()
{
  string str("Blah/Blubb/Bell/Gurgel");
  const int subs[] = {1};
  boost::regex r("(([^/]*)/?)");
  cout << "Analyzing: " << endl << str << endl;
  cout << "Found matches:" << endl;
  boost::sregex_token_iterator j;
  for(boost::sregex_token_iterator i(str.begin(), str.end(), r, subs); i !=j; i++) 
    cout << "Match: " << *i << endl;
  return 0;
}