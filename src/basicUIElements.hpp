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
      boost::shared_ptr<Wt::WHBoxLayout> layout;
      boost::shared_ptr<Wt::WLineEdit> lineEdit;
      boost::shared_ptr<Wt::WPushButton> button;
      
    public:
      
      SettingField (WContainerWidget * parent = 0) : WContainerWidget(parent)
      {
        this->layout.reset(Wt::WHBoxLayout());
        this->lineEdit.reset(Wt::WLineEdit());
        this->button.reset(Wt::WPushButton("Set"));
        this->setLayout(layout.get());
        this->layout()->addWidget(this->lineEdit.get());
        this->layout()->addWidget(this->button.get());
      }
      
      boost::shared_ptr<Wt::WHBoxLayout> layout()
      {
        return this->layout;
      }
      
      boost::shared_ptr<Wt::WLineEdit> lineEdit()
      {
        return this->lineEdit();
      }
      
      boost::shared_ptr<Wt::WPushButton> button()
      {
        return this->button();
      }
      
      void setDisabled(bool disabled = true)
      {
        this->lineEdit->setDisabled(disabled);
        this->button->setDisabled(disabled);
      }
      
    };
    
    
  }
}
#endif