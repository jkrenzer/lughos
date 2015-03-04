#include <iostream>
#include <boost/fusion/adapted.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/include/qi_auto.hpp>
#include <boost/spirit/include/qi_char.hpp>

using namespace std;
using namespace boost::spirit;

// namespace boost { namespace spirit { namespace traits {   
//     // be careful copying expression templates. Boost trunk has `qi::copy` for this too, now
//     #define PARSER_DEF(a) using type = decltype(boost::proto::deep_copy(a)); static type call() { return boost::proto::deep_copy(a); }
// 
//     template<typename T1, typename T2> 
//         struct create_parser<std::pair<T1, T2> > 
//         {
//             PARSER_DEF('(' >> create_parser<T1>::call() >> ',' >> create_parser<T2>::call() >> ')');
//         };
// 
//     template<typename TV, typename... TArgs>
//         struct create_parser<std::vector<TV, TArgs...> >
//         {
//             PARSER_DEF('[' >> qi::omit[qi::uint_] >> ',' >> '(' >> create_parser<TV>::call() % ',' >> ')' >> ']' );
//         };
// 
//     #undef PARSER_DEF
// } } }

#define PARSER_DEF(a) using type = decltype(boost::proto::deep_copy(a)); static type call() { return boost::proto::deep_copy(a); }

namespace boost { namespace spirit { namespace traits {
  
  template <> 
  struct create_parser<std::string>
  {
    PARSER_DEF('"' >> +(boost::spirit::qi::char_ - '"') >> '"');
  };
  
}}} //TODO boost.fusion invoke !

boost::fusion::vector<int,std::string> p;

#undef PARSER_DEF

int main()
{
  std::string input("42 \"Test\"");
    if(qi::phrase_parse(input.begin(),input.end(),p,qi::space))
    {
      cout << "MATCH: " << boost::fusion::at_c<0>(p) << " - " << boost::fusion::at_c<1>(p) <<  endl;
    }
  return 0;
}