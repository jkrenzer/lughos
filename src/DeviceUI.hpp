#ifndef DEVICEUI_HPP
#define DEVICEUI_HPP

#include <boost/signals2/signal.hpp>
#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WPanel>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WTimer>
#include <functional>
#include "StatusLEDWtWidget.hpp"
#include "device.hpp"



namespace lughos
{

  class DeviceUIInterface : public Wt::WPanel
  {
  protected:
        boost::shared_ptr<DeviceImpl> device_;
        boost::shared_ptr<Wt::WTimer> intervalTimer;
  public:
    std::string name;
    Wt::WContainerWidget * container;
    StatusLEDWtWidget * led;
    
    boost::signals2::signal<void ()> reset;
    boost::signals2::signal<void ()> refresh;
    boost::signals2::signal<void ()> disable;
    boost::signals2::signal<void ()> enable;
    
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
    
  DeviceUIInterface (Wt::WContainerWidget * parent = 0):WPanel (parent)
    {
      this->intervalTimer.reset(new Wt::WTimer());
      this->container = new Wt::WContainerWidget ();
      this->led = new StatusLEDWtWidget();
      this->led->setInline(true);
      this->led->setState<Disconnected>();
      this->setStyleClass ("DeviceContainer");
      this->setTitle (Wt::WString::fromUTF8 (this->name.c_str ()));
      this->titleBarWidget()->insertWidget(0,this->led);
      this->setCentralWidget (container);
      Wt::WPopupMenuItem* reconnect = this->led->popupMenu()->addItem("Reset Device");
//       reconnect->triggered().connect(this,&DeviceUIInterface::checkConnected); //TODO Reimplement good device reconnection
      Wt::WPopupMenuItem* state = this->led->popupMenu()->addItem("Refresh State");
      state->triggered().connect(boost::bind(&DeviceUIInterface::refresh,this));
      this->intervalTimer->setInterval(1000);
      this->intervalTimer->timeout().connect(boost::bind(&DeviceUIInterface::refresh,this)); // Reload measurements every second
      this->intervalTimer->start();

    }

    virtual ~ DeviceUIInterface ()
    {
      delete this->container;
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
    }
    
    void device(boost::shared_ptr<Device> device_)
    {
      this->device_ = device_;
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
