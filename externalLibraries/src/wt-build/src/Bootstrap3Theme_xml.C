// This is automatically generated code -- do not edit!
// Generated from  
#include <vector>
namespace skeletons
{


  const char * Bootstrap3Theme_xml1 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
  "<messages xmlns:if=\"Wt.WTemplate.conditions\">\r\n"
  "  <message id=\"Wt.Auth.field\">\r\n"
  "    <![CDATA[\r\n"
  "    ${<if:{1}>}\r\n"
  "    <div class=\"form-group\">\r\n"
  "      <label for=\"${id:{1}}\" class=\"control-label col-sm-2\">\r\n"
  "        ${tr:Wt.Auth.{1}}\r\n"
  "      </label>\r\n"
  "      <div class=\"col-sm-5\">\r\n"
  "	${{1}}\r\n"
  "      </div>\r\n"
  "      <div class=\"help-block col-sm-5\">\r\n"
  "        ${{1}-info}\r\n"
  "      </div>\r\n"
  "    </div>\r\n"
  "    ${</if:{1}>}\r\n"
  "    ]]>\r\n"
  "  </message>\r\n"
  "\r\n"
  "  <message id=\"Wt.WNavigationBar.template\">\r\n"
  "    <div>\r\n"
  "      ${collapse-button}\r\n"
  "      <div class=\"navbar-header\">\r\n"
  "	${expand-button}\r\n"
  "	${title-link}\r\n"
  "      </div>\r\n"
  "      ${contents}\r\n"
  "    </div>\r\n"
  "  </message>\r\n"
  "\r\n"
  "</messages>\r\n"
  "";
  std::vector<const char *> Bootstrap3Theme_xml()
  {
    std::vector<const char *> retval;

    retval.push_back(Bootstrap3Theme_xml1);

    return retval;
  }
}