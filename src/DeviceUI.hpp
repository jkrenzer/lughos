#ifndef DEVICEUI_HPP
#define DEVICEUI_HPP

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WPanel>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <functional>

namespace lughos
{

  class DeviceUIInterface:public Wt::WPanel
  {
  public:
    std::string name;
    Wt::WContainerWidget * container;

    void addWidget (Wt::WWidget * widget)
    {
      this->container->addWidget (widget);
    }

  DeviceUIInterface (Wt::WContainerWidget * parent = 0):WPanel (parent)
    {
      this->container = new Wt::WContainerWidget ();
      this->setStyleClass ("DeviceContainer");
      this->setTitle (Wt::WString::fromUTF8 (this->name.c_str ()));
      this->setCentralWidget (container);
    }

    virtual ~ DeviceUIInterface ()
    {
      delete this->container;
    }

  };


template < class D > class DeviceUI:public DeviceUIInterface
  {
  public:
    DeviceUI < D > ()
    {
      this->addWidget (new Wt::WText ("No GUI for device availible!"));
    }
  };
}				//namespace  
#endif
