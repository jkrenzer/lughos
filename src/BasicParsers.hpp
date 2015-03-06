#include <iostream>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/include/qi_auto.hpp>
#include <boost/spirit/include/qi_char.hpp>

#define PARSER_DEF(a) using type = decltype(boost::proto::deep_copy(a)); static type call() { return boost::proto::deep_copy(a); }

namespace boost { namespace spirit { namespace traits {
  
  using namespace boost::spirit::qi;
  
  template <> 
  struct create_parser<std::string>
  {
    PARSER_DEF(lexeme['"' >> +(char_ - '"') >> '"']);
  };
  
}}}

#undef PARSER_DEF