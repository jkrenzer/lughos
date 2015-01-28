#ifndef DEVICEUI_HPP
#define DEVICEUI_HPP

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <functional>

 class DeviceUIInterface : public Wt::WContainerWidget
  {
  public:
    std::string name;
    DeviceUIInterface()
    {
      setStyleClass("DeviceContainer");
    }
  };
  
    
  template <class D> class DeviceUI : public DeviceUIInterface
  {
  public:
    DeviceUI<D>()
    {

      this->addWidget(new Wt::WText(this->name.c_str()));
      this->addWidget(new Wt::WText("No GUI for device availible!"));
    }
  };
  
  #endif  