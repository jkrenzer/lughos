#ifndef BASIC_UI_ELEMENTS_HPP
#define BASIC_UI_ELEMENTS_HPP
#include <Wt/WToolBar>
#include <Wt/WLineEdit>
#include <Wt/WHBoxLayout>
#include <Wt/WSplitButton>
#include <Wt/WContainerWidget>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>

namespace lughos
{
  namespace ui
  {
    template <class F> class Setting : public Wt::WToolBar
    {
      Wt::WSplitButton* setButton;
      Wt::WContainerWidget* container;
    public:
      
      Setting<F> (Wt::WContainerWidget * parent = 0)
      {
	this->setButton = new Wt::WSplitButton("Set");
	this->setButton->setObjectName("setButton");
	this->container = static_cast<Wt::WContainerWidget*>(this->implementation());
	this->addWidget(new F());
	this->addButton(this->setButton);
      }
      
      void addField(Wt::WLineEdit* field)
      {
	this->container->insertBefore(field, this->setButton);
      }
      
      F* field(unsigned int index = 0)
      {
	return static_cast<F*>(this->widget(index));
      }
      
      Wt::WPushButton* button()
      {
	return this->setButton->actionButton();
      }
      
      Wt::WPushButton* menuButton()
      {
	return this->setButton->dropDownButton();
      }
      
      Wt::WPopupMenu * menu ()
      {
	return this->setButton->menu();
      }
      
      void setPopupMenu(Wt::WPopupMenu * menu)
      {
	this->setButton->setMenu(menu);
      }

    };
    
    
  }
}
#endif