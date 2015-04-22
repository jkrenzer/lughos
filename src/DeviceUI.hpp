#ifndef DEVICEUI_HPP
#define DEVICEUI_HPP

#include <boost/signals2.hpp>
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WPanel>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WTimer>
#include <functional>
#include "StatusLEDWtWidget.hpp"
#include "device.hpp"
#include "basicUIElements.hpp"



namespace lughos
{

  class DeviceUIInterface : public Wt::WPanel
  {
  protected:
        boost::shared_ptr<DeviceImpl> device_;
        boost::shared_ptr<Wt::WTimer> intervalTimer;
        
        boost::signals2::signal<void ()> refreshSignal;
        boost::signals2::signal<void (bool)> setDisabledSignal;
        
        
  public:
    std::string name;
    boost::shared_ptr<Wt::WContainerWidget> container;
    boost::shared_ptr<StatusLEDWtWidget> led;
        
    class Disconnected : public StatusLEDState
    {
    public:
      Disconnected(std::string message = std::string("Disconnected"))
      {
        this->message = message;
        this->color = Color::Off;
      }
    };
    
    class Connected : public StatusLEDState
    {
    public:
      Connected(std::string message = std::string("Connected"))
      {
        this->message = message;
        this->color = Color::Green;
      }
    };
    
    class Waiting : public StatusLEDState
    {
    public:
      Waiting(std::string message = std::string("Notice"))
      {
        this->message = message;
        this->color = Color::Yellow;
      }
    };
    
    class Warning : public StatusLEDState
    {
    public:
      Warning(std::string message = std::string("Warning!"))
      {
        this->message = message;
        this->color = Color::Red;
      }
    };
    
    class Error : public StatusLEDState
    {
    public:
      Error(std::string message = std::string("Error"))
      {
        this->message = message;
        this->color = Color::Red;
      }
    };
    
    typedef boost::shared_ptr<StatusLEDState> StatePointer;

    void addWidget (Wt::WWidget * widget)
    {
      this->container->addWidget (widget);
    }
    
        template <class T>
    void addWidget(ui::Measurement<T>* widget)
    {
      this->container->addWidget(widget);
      this->refreshSignal.connect(boost::bind(&ui::Measurement<T>::pull,widget));
      this->setDisabledSignal.connect(boost::bind(&ui::Measurement<T>::setDisabled,widget,_1));
    }
    
    virtual void reset()
    {

    }
    
  DeviceUIInterface (Wt::WContainerWidget * parent = 0):WPanel (parent)
    {
      this->intervalTimer.reset(new Wt::WTimer());
      this->container.reset(new Wt::WContainerWidget());
      this->led.reset(new StatusLEDWtWidget());
      this->led->setInline(true);
      this->led->setState<Disconnected>();
      this->setStyleClass ("DeviceContainer");
      this->setTitle (Wt::WString::fromUTF8 ("Loading..."));
      this->titleBarWidget()->insertWidget(0,this->led.get());
      this->setCentralWidget (container.get());
      Wt::WPopupMenuItem* reconnect = this->led->popupMenu()->addItem("Reset Device");
      reconnect->triggered().connect(boost::bind(&DeviceUIInterface::reset,this)); //TODO Reimplement good device reconnection
      Wt::WPopupMenuItem* state = this->led->popupMenu()->addItem("Refresh State");
      state->triggered().connect(boost::bind(&DeviceUIInterface::refreshSignal,this));
      this->intervalTimer->setInterval(1000);
      this->intervalTimer->timeout().connect(boost::bind(&DeviceUIInterface::refreshSignal,this));
    }

    virtual ~ DeviceUIInterface ()
    {
      this->intervalTimer->stop();
    }

  };

template <class D> class DeviceUITemplate : public DeviceUIInterface
{
  public:
    boost::shared_ptr<D> device()
    {
      return boost::dynamic_pointer_cast<D>(this->device_);
    }
    
    DeviceUITemplate<D>(boost::shared_ptr<Device> device_)
    {
      this->device(device_);
      this->name = device_->getName();
      this->setTitle (Wt::WString::fromUTF8 (this->name.c_str ()));
      this->device_->onConnect.connect(boost::bind(&StatusLEDWtWidget::setState<Connected>,this->led));
//       this->device_->onConnect.connect(boost::bind(&DeviceUIInterface::setDisabledSignal::,this,false));
      this->device_->onDisconnect.connect(boost::bind(&StatusLEDWtWidget::setState<Disconnected>,this->led));
//       this->device_->onDisconnect.connect(boost::bind(&DeviceUIInterface::setDisabledSignal,this,true));
      this->device_->onError.connect(boost::bind(&StatusLEDWtWidget::setState<Error>,this->led));
      this->device_->emitConnectionSignals();
      this->intervalTimer->start();
    }
    
    ~DeviceUITemplate<D>()
    {
      this->device_->onConnect.disconnect(boost::bind(&StatusLEDWtWidget::setState<Connected>,this->led));
      this->device_->onDisconnect.disconnect(boost::bind(&StatusLEDWtWidget::setState<Disconnected>,this->led));
      this->device_->onError.disconnect(boost::bind(&StatusLEDWtWidget::setState<Error>,this->led));
    }
    
    void device(boost::shared_ptr<Device> device_)
    {
      this->device_ = device_;
      this->device_->emitConnectionSignals();
    }
};

template < class D > class DeviceUI : public DeviceUITemplate<D>
  {
  public:
    DeviceUI < D > ()
    {
      this->addWidget (new Wt::WText ("No GUI for device availible!"));
    }
  };
}				//namespace  
#endif
