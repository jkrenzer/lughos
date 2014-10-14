#ifndef TEST_GUI_HPP
#define TEST_GUI_HPP

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WTabWidget>
#include <Wt/WMenuItem>
#include <Wt/WEnvironment>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WBootstrapTheme>
#include <Wt/WCssTheme>
#include <Wt/WImage>
#include <Wt/WServer>
#include <Wt/WIOService>
#include <functional>
#include "coolpak6000.hpp"

namespace lughos 
{

  using namespace Wt;
  using namespace std;
  
  extern boost::asio::io_service * ioService;
  
  class DeviceUIInterface : public Wt::WContainerWidget
  {
  public:
    std::string name;
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
  
  template <> class DeviceUI<coolpak6000> : public DeviceUIInterface
  {
  protected:
    boost::shared_ptr<coolpak6000> coolpak;
    Wt::WLineEdit* stateF;
    Wt::WLabel* stateL;
    Wt::WPushButton * startB;
    Wt::WPushButton * stateB;
    Wt::WPushButton * stopB;
    
  public:
    
    DeviceUI<coolpak6000>(std::string  name, std::string comPort) : coolpak(new coolpak6000(ioService))
    {
     this->name = name;
     coolpak->port_name=comPort;
     bool isStarted = false;
     try 
     {
      isStarted = coolpak->start();
     }
     catch(...)
     {
       isStarted = false;
     }
     
     if(isStarted)
     {
        coolpak->stop();
	this->setWidth(250);
	this->addWidget(new Wt::WText(this->name.c_str()));
	this->stateF = new Wt::WLineEdit("Initializing...");
	this->stateF->setReadOnly(true);
	this->stateL = new Wt::WLabel("Status:");
	this->stateL->setBuddy(stateF);
	this->startB = new Wt::WPushButton("Start");
	this->stopB = new Wt::WPushButton("Stop");
	this->stateB = new Wt::WPushButton("Status");
	this->startB->setDisabled(true);
	this->startB->clicked().connect(this,&DeviceUI<coolpak6000>::start);
	this->stopB->setDisabled(true);
	this->stopB->clicked().connect(this,&DeviceUI<coolpak6000>::stop);
	this->stateB->clicked().connect(this,&DeviceUI<coolpak6000>::showData);
	this->addWidget(stateL);
	this->addWidget(stateF);
	this->addWidget(startB);
	this->addWidget(stopB);
	this->addWidget(stateB);
	this->init();
      }
      else
      {
	this->addWidget(new Wt::WText("Wrong or disabled Port!"));
      }
    }
    
    void init()
    {
      this->getState();
    }
    
    void showData()
    {
      this->stateF->setText(coolpak->get_data());
    }
    
    void getState()
    {
      coolpak->get_data();
      std::string state;
      bool communicationEstablished = false;
      switch(coolpak->get_compressor_state())
      {
	case 0:
	  state += std::string("Compressor OFF ");
	  communicationEstablished = true;
	  break;
	case 1:
	  state += std::string("Compressor ON ");
	  communicationEstablished = true;
	  break;
	case 2:
	  state += std::string("Compressor ERROR ");
	  communicationEstablished = true;
	  break;
	default:
	  state += std::string("Compressor ???? ");
	 break;
      }
      state += std::string(",");
      switch(coolpak->get_coolhead1_state())
      {
	case 0:
	  state += std::string("Coldhead1 OFF ");
	  communicationEstablished = true;
	  break;
	case 1:
	  state += std::string("Coldhead1 ON ");
	  communicationEstablished = true;
	  break;
	case 2:
	  state += std::string("Coldhead1 ERROR ");
	  communicationEstablished = true;
	  break;
	default:
	  state += std::string("Coldhead1 ???? ");
	 break;
      } 
      if(communicationEstablished)
      {
	this->stateF->setText(state);
	this->startB->setDisabled(false);
	this->stopB->setDisabled(false);
      }
    }
    
    void start()
    {
      this->stateF->setText("Starting...");
      if(this->coolpak->compressor_on())
	this->stateF->setText("System on");
      else
	this->stateF->setText("Cannot start!");
      this->getState();
    }
    
    void stop()
    {
      this->stateF->setText("Stopping...");
      if(this->coolpak->compressor_off())
	this->stateF->setText("System off");
      else
	this->stateF->setText("Cannot stop!");
      this->getState();
    }
    
    
    
  };
 
  class OverView : public Wt::WContainerWidget
  {
  public:
    
    OverView(WContainerWidget* parent = 0)
    {
      
    }
    
  };
  
  class PressureView : public Wt::WContainerWidget
  {
  public:
    
    PressureView(WContainerWidget* parent = 0)
    {
      
    }
    
  };
  
  class TemperatureView : public Wt::WContainerWidget
  {
  public:
    
    TemperatureView(WContainerWidget* parent = 0)
    {
      
    }
    
  };
  
  class DeviceView : public Wt::WContainerWidget
  {
  public:
    DeviceView(WContainerWidget* parent = 0)
    {
      this->addWidget(new DeviceUI<coolpak6000>("Compressor 1" , "COM1"));
    }
    
  };
  
  
  class mainApplication : public WApplication
  {
  public:
    
    
    
    mainApplication(const WEnvironment &env) : WApplication(env)
    {
      Wt::WBootstrapTheme *bootstrapTheme = new Wt::WBootstrapTheme(this);
      bootstrapTheme->setVersion(Wt::WBootstrapTheme::Version3);
      bootstrapTheme->setResponsive(true);
      this->setTheme(bootstrapTheme);
      // load the default bootstrap3 (sub-)theme
      this->useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
      setTitle("Lughos System Control");
      Wt::WContainerWidget *container = new Wt::WContainerWidget();
      Wt::WContainerWidget *headContainer = new Wt::WContainerWidget();
      Wt::WImage *logo = new WImage("resources/logo.png");
      Wt::WImage *branding = new WImage("resources/branding.png");
      Wt::WText *headText = new WText("<h1>Lughos System Control</h1>");
      logo->setHeight(100);
      branding->setHeight(100);
      Wt::WHBoxLayout *headBox = new Wt::WHBoxLayout();
      headContainer->setLayout(headBox);
      headBox->addWidget(logo);
      headBox->addWidget(headText,1);
      headBox->addWidget(branding);

      
      
      Wt::WVBoxLayout *vbox = new Wt::WVBoxLayout();
      container->setLayout(vbox);
      vbox->addWidget(headContainer);
      Wt::WTabWidget *tabW = new Wt::WTabWidget(container);
      tabW->addTab(new Wt::WTextArea("This is the contents of the first tab."),
		  "First", Wt::WTabWidget::PreLoading);
      tabW->addTab(new Wt::WTextArea("The contents of the tabs are pre-loaded in"
				    " the browser to ensure swift switching."),
		  "Preload", Wt::WTabWidget::PreLoading);
      tabW->addTab(new DeviceView(), "Devices", Wt::WTabWidget::PreLoading)->setStyleClass("trhead");

//       Wt::WMenuItem *tab 
// 	  = tabW->addTab(new Wt::WTextArea("You can close this tab"
// 					  " by clicking on the close icon."),
// 			"Close");
//       tab->setCloseable(true);
      vbox->addWidget(tabW);
      tabW->setStyleClass("tabwidget");
      root()->addWidget(container);
    }
    
    
  };
  
  
} //namespace lughos

#endif