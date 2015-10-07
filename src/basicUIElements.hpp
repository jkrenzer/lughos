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
      boost::signals2::connection onFocusConnection;
      Wt::WServer* wtServer_;
      Wt::WApplication* wtApp_;
      bool syncValueEnabled;
      
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
        this->syncValueEnabled = true;
      }
      
    virtual ~Measurement()
    {
      this->onValueChangeConnection.disconnect();
    }
      
      void setSyncValue(bool enable = true)
      {
	ExclusiveLock lock(mutex);
	this->syncValueEnabled = enable;
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
	lock.unlock();
 	this->pull(); //Fetching initial value
 	lock.lock();
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
        if(this->asignee_ != nullptr && this->syncValueEnabled)
        {
          std::string strValue = this->asignee_->getValueAsString();
	  Wt::WApplication::UpdateLock wtlock(this->wtApp_);
          this->field_->setText(strValue);
          this->wtApp_->triggerUpdate();
          LUGHOS_LOG(log::SeverityLevel::informative) << "Pulled new value \"" << strValue << "\" from value-object \"" << this->asignee_->getName() << "\"." ;
        }
        else if(this->asignee_ == nullptr)
        {
          BOOST_THROW_EXCEPTION( exception() << errorName("pull_invalid_ptr") << errorDescription("A pull was emitted but the object from which we shoul pull is a null-pointer!") << errorSeverity(severity::MustNot) );
          LUGHOS_LOG(log::SeverityLevel::error) << "Tried to pull from value-object but we have a null-pointer!" ;
        }
        else if(!this->syncValueEnabled)
        {
	  LUGHOS_LOG(log::SeverityLevel::informative) << "Tried to pull from value-object but sync is disabled. Skipping." ;
        }
        else
        {
	  LUGHOS_LOG(log::SeverityLevel::informative) << "Could not pull for unknown reason!" ;
        }
      }
      
    };
  
    template <class F> class Setting : public Measurement<F>
    {
    private:
      Mutex mutex;
    protected:
      Wt::WPushButton* button_;
      
      void markTainted(bool isTainted = true)
      {
	LUGHOS_LOG_FUNCTION();
	ExclusiveLock lock(this->mutex);
	if (isTainted)
	{
	  this->syncValueEnabled = false;
	  LUGHOS_LOG(log::SeverityLevel::informative) << "Marking field of \"" << this->objectName() << "\" tainted." ;
	  Wt::WApplication::UpdateLock wtlock(this->wtApp_);
	  this->field_->decorationStyle().setBackgroundColor(Wt::WColor(255,255,0));
	  this->wtApp_->triggerUpdate();
	}
	else
	{
	  this->syncValueEnabled = true;
	  LUGHOS_LOG(log::SeverityLevel::informative) << "Marking field of \"" << this->objectName() << "\" clean." ;
	  Wt::WApplication::UpdateLock wtlock(this->wtApp_);
	  this->field_->decorationStyle().setBackgroundColor(Wt::WColor(255,255,255));
	  this->wtApp_->triggerUpdate();
	}
      }
      
      void checkTainted()
      {
	SharedLock lock(this->mutex);
	if(this->asignee_ != nullptr)
	{
	  if(this->asignee_->compareStringValue(this->field_->text().toUTF8()))
	  {
	    lock.unlock();
	    this->markTainted(false);
	  }
	  else
	  {
	    lock.unlock();
	    this->markTainted(true);
	  }
	}
      }
     
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
	this->asignee_ = nullptr;
 	this->onValueChangeConnection.disconnect();
	this->buttonClickedConnection.disconnect();
	this->onFocusConnection.disconnect();
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
         lock.unlock();
 	this->pull(); //Fetching initial value
 	lock.lock();
 	this->onValueChangeConnection = this->asignee_->onValueChange.connect(boost::bind(&Setting<F>::pull,this));
	this->buttonClickedConnection = this->button_->clicked().connect(boost::bind(&Setting<F>::push,this));
	this->onFocusConnection = this->field_->changed().connect(boost::bind(&Setting<F>::checkTainted,this));
      }
      
      void detach()
      {
        LUGHOS_LOG_FUNCTION();
	ExclusiveLock lock(mutex);
	LUGHOS_LOG(log::SeverityLevel::informative) << "Detaching UI-Element from value-object \"" << this->asignee_->getName() << "\"." ;
	this->asignee_ = nullptr;
 	this->onValueChangeConnection.disconnect();
	this->buttonClickedConnection.disconnect();
	this->onFocusConnection.disconnect();
      }
      
      void push()
      {
	LUGHOS_LOG_FUNCTION();
	
	if(this->asignee_ != nullptr)
        {
          SharedLock lock(mutex);
	  this->asignee_->setValueFromString(this->field_->text().toUTF8());
	  LUGHOS_LOG(log::SeverityLevel::informative) << "Pushing new value \"" << this->field_->text().toUTF8() << "\" from UI-Element to underlying value-object \"" << this->asignee_->getName() << "\"." ;
	}
	else
	{
	  BOOST_THROW_EXCEPTION( exception() << errorName("push_invalid_ptr") << errorDescription("A push was emitted but the object to which we should push is a null-pointer!") << errorSeverity(severity::MustNot) );
          LUGHOS_LOG(log::SeverityLevel::error) << "Tried to push to value-object but we have a null-pointer!" ;
	}
	this->checkTainted();
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