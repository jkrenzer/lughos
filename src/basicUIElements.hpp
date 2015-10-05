#ifndef BASIC_UI_ELEMENTS_HPP
#define BASIC_UI_ELEMENTS_HPP
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WGridLayout>
#include <Wt/WPushButton>
#include <Wt/WApplication>
#include <Wt/WServer>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>

#include "exposedMeasuredValues.hpp"
#include "log.hpp"

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
      Wt::WServer* wtServer_;
      Wt::WApplication* wtApp_;
      
    public:
      
    Measurement(WContainerWidget * parent = 0) : WContainerWidget(parent)
      {
        this->field_ = new F();
        this->field_->setReadOnly(true);
        this->setLayout(new Wt::WHBoxLayout());
        this->layout()->addWidget(this->field_);
        this->setPadding(0);
        this->wtApp_ = Wt::WApplication::instance();
        this->wtServer_ = Wt::WServer::instance();
      }
      
    virtual ~Measurement()
    {
      detach();
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
	LUGHOS_LOG_FUNCTION();
	ExclusiveLock lock(mutex);
	this->asignee_ = &asignee_;
	LUGHOS_LOG(log::SeverityLevel::informative) << "Attaching UI-Element to value-object \"" << this->asignee_->getName() << "\"." ;
 	this->onValueChangeConnection = this->asignee_->onValueChange.connect(boost::bind(&Measurement::pull,this));
      }
      
      virtual void detach()
      {
	LUGHOS_LOG_FUNCTION();
	ExclusiveLock lock(mutex);
	LUGHOS_LOG(log::SeverityLevel::informative) << "Detaching UI-Element from value-object \"" << this->asignee_->getName() << "\"." ;
	this->asignee_ = nullptr;
	this->onValueChangeConnection.disconnect();
      }
      
      virtual void pull()
      {
	LUGHOS_LOG_FUNCTION();
	ExclusiveLock lock(mutex);
        if(this->asignee_ != nullptr)
        {
	  Wt::WApplication::UpdateLock lock(this->wtApp_);
          this->field_->setText(this->asignee_->getValueAsString());
          this->wtApp_->triggerUpdate();
          LUGHOS_LOG(log::SeverityLevel::informative) << "Pulled new value \"" << this->field_->text().toUTF8() << "\" from value-object \"" << this->asignee_->getName() << "\"." ;
        }
        else
        {
          BOOST_THROW_EXCEPTION( exception() << errorName("pull_invalid_ptr") << errorDescription("A pull was emitted but the object from which we shoul pull is a null-pointer!") << errorSeverity(severity::MustNot) );
          LUGHOS_LOG(log::SeverityLevel::error) << "Tried to pull from value-object but we have a null-pointer!" ;
        }
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
      
      virtual ~Setting()
      {
	detach();
      }
      
      Wt::WPushButton* button()
      {
	SharedLock lock(mutex);
        return this->button_;
      }
      
      void setDisabled(bool disabled = true)
      {
	ExclusiveLock lock(mutex);
        this->field_->setDisabled(disabled);
        this->button_->setDisabled(disabled);
      }
      
      void attach(ExposedValueInterface& asignee_)
      {
        LUGHOS_LOG_FUNCTION();
	ExclusiveLock lock(mutex);
	this->asignee_ = &asignee_;
	LUGHOS_LOG(log::SeverityLevel::informative) << "Attaching UI-Element to value-object \"" << this->asignee_->getName() << "\"." ;
	this->onValueChangeConnection = this->asignee_->onValueChange.connect(boost::bind(&Setting<F>::pull,this));
	this->buttonClickedConnection = this->button_->clicked().connect(boost::bind(&Setting<F>::push,this));
      }
      
      void detach()
      {
        LUGHOS_LOG_FUNCTION();
	ExclusiveLock lock(mutex);
	LUGHOS_LOG(log::SeverityLevel::informative) << "Detaching UI-Element from value-object \"" << this->asignee_->getName() << "\"." ;
	this->asignee_ = nullptr;
	this->onValueChangeConnection.disconnect();
	this->buttonClickedConnection.disconnect();
      }
      
      void push()
      {
	LUGHOS_LOG_FUNCTION();
	SharedLock lock(mutex);
	if(this->asignee_ != nullptr)
        {
	  this->asignee_->setValueFromString(this->field_->text().toUTF8());
	  LUGHOS_LOG(log::SeverityLevel::informative) << "Pushing new value \"" << this->field_->text().toUTF8() << "\" from UI-Element to underlying value-object \"" << this->asignee_->getName() << "\"." ;
	}
	else
	{
	  BOOST_THROW_EXCEPTION( exception() << errorName("push_invalid_ptr") << errorDescription("A push was emitted but the object to which we should push is a null-pointer!") << errorSeverity(severity::MustNot) );
          LUGHOS_LOG(log::SeverityLevel::error) << "Tried to push to value-object but we have a null-pointer!" ;
	}
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