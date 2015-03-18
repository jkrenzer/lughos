#ifndef BASIC_UI_ELEMENTS_HPP
#define BASIC_UI_ELEMENTS_HPP
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WHBoxLayout>
#include <Wt/WPushButton>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>

namespace lughos
{
  namespace ui
  {
    class SettingField : public Wt::WContainerWidget
    {
    protected:
      boost::shared_ptr<Wt::WLineEdit> field_;
      boost::shared_ptr<Wt::WPushButton> button_;
      
    public:
      
      SettingField (WContainerWidget * parent = 0) : WContainerWidget(parent)
      {
        
        this->field().reset(new Wt::WLineEdit());
        this->button().reset(new Wt::WPushButton("Set"));
        this->setLayout(new Wt::WHBoxLayout());
        this->layout()->addWidget(this->field().get());
        this->layout()->addWidget(this->button().get());
      }
      
      boost::shared_ptr<Wt::WLineEdit> field()
      {
        return this->field_;
      }
      
      boost::shared_ptr<Wt::WPushButton> button()
      {
        return this->button_;
      }
      
      void setDisabled(bool disabled = true)
      {
        this->field_->setDisabled(disabled);
        this->button_->setDisabled(disabled);
      }
      
    };
    
    
  }
}
#endif