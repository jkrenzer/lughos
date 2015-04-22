#ifndef BASIC_UI_ELEMENTS_HPP
#define BASIC_UI_ELEMENTS_HPP
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WGridLayout>
#include <Wt/WPushButton>
#include <Wt/WApplication>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>

#include "exposedMeasuredValues.hpp"

namespace lughos
{
  namespace ui
  {
    template <class F> class Measurement : public Wt::WContainerWidget
    {
    private:
      Mutex mutex;
    protected:
      F* field_;
      ExposedValueInterface* asignee_;
      boost::signals2::connection onValueChangeConnection;
      boost::signals2::connection buttonClickedConnection;
      
      
      Wt::WApplication* app()
      {
	return Wt::WApplication::instance();
      }
      
    public:
      
    Measurement(WContainerWidget * parent = 0) : WContainerWidget(parent)
      {
        this->field_ = new F();
        this->field_->setReadOnly(true);
        this->setLayout(new Wt::WHBoxLayout());
        this->layout()->addWidget(this->field_);
        this->setPadding(0);
      }
      
      F* field()
      {
	SharedLock lock(mutex);
        return dynamic_cast<F*>(this->field_);
      }
      
      void setDisabled(bool disabled = true)
      {
	ExclusiveLock lock(mutex);
        this->field_->setDisabled(true);
      }
      
      virtual void attach(ExposedValueInterface& asignee_)
      {
	ExclusiveLock lock(mutex);
	this->asignee_ = &asignee_;
	this->onValueChangeConnection = this->asignee_->onValueChange.connect(boost::bind(&Measurement<F>::pull,this));
      }
      
      virtual void detach()
      {
	ExclusiveLock lock(mutex);
	this->asignee_ = nullptr;
	this->onValueChangeConnection.disconnect();
      }
      
      virtual void pull()
      {
	ExclusiveLock lock(mutex);
	Wt::WApplication::UpdateLock uiLock(app());
	this->field_->setText(this->asignee_->getValueAsString());
	app()->triggerUpdate();
      }
      
    };
  
    template <class F> class Setting : public Measurement<F>
    {
    private:
      Mutex mutex;
    protected:
      Wt::WPushButton* button_;
     
    public:
      
      Setting (Wt::WContainerWidget * parent = 0) : Measurement<F>(parent)
      {
        this->button_ = new Wt::WPushButton("Set");
        this->field_->setReadOnly(false);
        this->field_->setDisabled(false);
        this->layout()->addWidget(this->button_);
      }
      
      Wt::WPushButton* button()
      {
	SharedLock lock(mutex);
        return this->button_;
      }
      
      void setDisabled(bool disabled = true)
      {
	ExclusiveLock lock(mutex);
	Wt::WApplication::UpdateLock uiLock(this->app());
        this->field_->setDisabled(disabled);
        this->button_->setDisabled(disabled);
        this->app()->triggerUpdate();
      }
      
      void attach(ExposedValueInterface& asignee_)
      {
	ExclusiveLock lock(mutex);
	this->asignee_ = &asignee_;
	this->onValueChangeConnection = this->asignee_->onValueChange.connect(boost::bind(&Setting<F>::pull,this));
	this->buttonClickedConnection = this->button_->clicked().connect(boost::bind(&Setting<F>::push,this));
      }
      
      void detach()
      {
	ExclusiveLock lock(mutex);
	this->asignee_ = nullptr;
	this->onValueChangeConnection.disconnect();
	this->buttonClickedConnection.disconnect();
      }
      
      void push()
      {
	SharedLock lock(mutex);
	this->asignee_->setValueFromString(this->field_->text().toUTF8());
      }
      
    };
    
    class HContainer : Wt::WContainerWidget
    {
      HContainer(WContainerWidget* parent = 0)
      {
	this->setLayout(new Wt::WHBoxLayout());
	this->setPadding(0);
      }
    };
    
    class VContainer : Wt::WContainerWidget
    {
      VContainer(WContainerWidget* parent = 0)
      {
	this->setLayout(new Wt::WVBoxLayout());
	this->setPadding(0);
      }
    };
    
    class GridContainer : Wt::WContainerWidget
    {
      GridContainer(WContainerWidget* parent = 0)
      {
	this->setLayout(new Wt::WGridLayout());
	this->setPadding(0);
      }
    };
  }
}
#endif