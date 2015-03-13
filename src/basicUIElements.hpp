#ifndef BASIC_UI_ELEMENTS_HPP
#define BASIC_UI_ELEMENTS_HPP
#include <Wt/WContainerWidget>
#include <Wt/WHBoxLayout>
#include <Wt/WPushButton>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>

namespace lughos
{
  namespace ui
  {
    class LineEdit : public Wt::WContainerWidget
    {
    protected:
      boost::shared_ptr<Wt::WHBoxLayout> layout;
      boost::shared_ptr<Wt::WLineEdit> lineEdit;
      boost::shared_ptr<Wt::WPushButton> button;
      
    public:
      
      
      
    };
  }
}
#endif