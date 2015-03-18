#ifndef BASIC_UI_ELEMENTS_HPP
#define BASIC_UI_ELEMENTS_HPP
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WGridLayout>
#include <Wt/WPushButton>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>

namespace lughos
{
  namespace ui
  {
    template <class F> class Setting : public Wt::WContainerWidget
    {
    protected:
      Wt::WLineEdit* field_;
      Wt::WPushButton* button_;
      
    public:
      
      Setting (WContainerWidget * parent = 0) : WContainerWidget(parent)
      {
        this->field_ = new Wt::WLineEdit();
        this->button_ = new Wt::WPushButton("Set");
        this->setLayout(new Wt::WHBoxLayout());
        this->layout()->addWidget(this->field_);
        this->layout()->addWidget(this->button_);
      }
      
      F* field()
      {
        return static_cast<F*>(this->field_);
      }
      
      Wt::WPushButton* button()
      {
        return this->button_;
      }
      
      void setDisabled(bool disabled = true)
      {
        this->field_->setDisabled(disabled);
        this->button_->setDisabled(disabled);
      }
      
    };
    
    class HContainer : Wt::WContainerWidget
    {
      HContainer(WContainerWidget* parent = 0)
      {
	this->setLayout(new Wt::WHBoxLayout());
      }
    };
    
    class VContainer : Wt::WContainerWidget
    {
      VContainer(WContainerWidget* parent = 0)
      {
	this->setLayout(new Wt::WVBoxLayout());
      }
    };
    
    class GridContainer : Wt::WContainerWidget
    {
      GridContainer(WContainerWidget* parent = 0)
      {
	this->setLayout(new Wt::WGridLayout());
      }
    };
  }
}
#endif