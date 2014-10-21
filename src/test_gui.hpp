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
#include "MaxiGauge.hpp"

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
      

//     ofs.close();
     this->name = name;
     coolpak->port_name=comPort;
     bool ConnectionEstablished = false;
     try 
     {
      ConnectionEstablished = coolpak->testconnection();
     }
     catch(...)
     {
      this->addWidget(new Wt::WText(std::string("Port-accessor crashed!\n")));
       ConnectionEstablished = false;
     }
     
     if(ConnectionEstablished)
     {
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
	this->addWidget(new Wt::WText(std::string("Port ") + comPort + std::string(" cannot be opened. Port blocked, disabled or wrong portname.")));
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
  //---------------MaxiGauge
   template <> class DeviceUI<MaxiGauge> : public DeviceUIInterface
  {
  protected:
    boost::shared_ptr<MaxiGauge> maxigauge;
    Wt::WLineEdit* stateF;
    Wt::WLabel* stateL;
    Wt::WPushButton * startB;
    Wt::WPushButton * onB[6];
    Wt::WPushButton * offB[6];
    Wt::WPushButton * stopB;
    Wt::WPushButton * stateB;
    
  public:
    

DeviceUI<MaxiGauge>(std::string  name, std::string comPort) : maxigauge(new MaxiGauge(ioService))
    {
     this->name = name;
     maxigauge->port_name=comPort;
     bool ConnectionEstablished = false;
     try 
     {
      ConnectionEstablished = maxigauge->testconnection();
     }
     catch(...)
     {
       this->addWidget(new Wt::WText(std::string("Port-accessor crashed!\n")));
       ConnectionEstablished = false;
     }
     
     if(ConnectionEstablished)
     {
//         maxigauge->stop();
	this->setWidth(250);
	this->addWidget(new Wt::WText(this->name.c_str()));
	this->stateF = new Wt::WLineEdit("Initializing...");
	this->stateF->setReadOnly(true);
	this->stateL = new Wt::WLabel("Status:");
	this->stateL->setBuddy(stateF);
	this->startB = new Wt::WPushButton("Start all");
	this->stopB = new Wt::WPushButton("Stop all");
	this->stateB = new Wt::WPushButton("Status");
	this->startB->setDisabled(true);
	this->startB->clicked().connect(this,&DeviceUI<MaxiGauge>::startall);
	this->stopB->setDisabled(true);
	this->stopB->clicked().connect(this,&DeviceUI<MaxiGauge>::stopall);
      for(int i=0;i<6;i++)
      {
	this->onB[i]->clicked().connect(boost::bind(&DeviceUI<MaxiGauge>::sensor_on,this, i));
	this->addWidget(onB[i]);
      }
      for(int i=0;i<6;i++)
      {
	this->offB[i]->clicked().connect(boost::bind(&DeviceUI<MaxiGauge>::sensor_off,this, i));
	this->addWidget(offB[i]);
      }
	this->addWidget(stateL);
	this->addWidget(stateF);
// 	this->addWidget(startB);
// 	this->addWidget(stopB);
	
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
    
    void sensor_on(int i)
    {
      this->stateF->setText(std::to_string(maxigauge->sensor_on(i)));
    }
        
     void sensor_off(int i)
    {
      this->stateF->setText(std::to_string(maxigauge->sensor_off(i)));
    }
    
    void getState()
    {
      
//       coolpak->get_data();
      std::string state;
      std::string enabled="Enabled channels: ";
      std::string disabled="disabled channels: ";

      bool communicationEstablished = false;
      for(int i=0;i<6;i++)
      {
	if(!maxigauge->get_status(i).empty())
	{
	 enabled+=std::to_string(i); 
	 enabled+=std::string(" ");
	 communicationEstablished = true;
	}
	else
	{
	 disabled+=std::to_string(i); 
	 disabled+=std::string(" ");
	 communicationEstablished = true;
	}
	
      }
      state=enabled+disabled;
     
      if(communicationEstablished)
      {
	this->stateF->setText(state);
	this->startB->setDisabled(false);
	this->stopB->setDisabled(false);
      }
    }
    
    
    
    void startall()
    {
      
      for(int i=0;i<6;i++)
      {
	maxigauge->sensor_on(i);
      }
      this->getState();
    }
    
    void stopall()
    {
      for(int i=0;i<6;i++)
      {
	maxigauge->sensor_off(i);
      }
      this->getState();
    }
    
    
    
  };
  //-----------------------
  
 
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
      
//           ofs.close();
#ifdef WIN32
      this->addWidget(new DeviceUI<coolpak6000>("Compressor 1" , "COM1"));
      this->addWidget(new DeviceUI<MaxiGauge>("Preasure Monitor 1" , "COM2"));  
#else
      this->addWidget(new DeviceUI<coolpak6000>("Compressor 1" , "/dev/ttyS0"));
      this->addWidget(new DeviceUI<MaxiGauge>("Compressor 1" , "/dev/ttyS1"));

#endif

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
// 		    ofs.close();
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
// 		    ofs.close();
      
      
      Wt::WVBoxLayout *vbox = new Wt::WVBoxLayout();
      container->setLayout(vbox);
      vbox->addWidget(headContainer);
// 		    ofs.close();
      Wt::WTabWidget *tabW = new Wt::WTabWidget(container);
      tabW->addTab(new Wt::WTextArea("This is the contents of the first tab."),
		  "First", Wt::WTabWidget::PreLoading);
      tabW->addTab(new Wt::WTextArea("The contents of the tabs are pre-loaded in"
				    " the browser to ensure swift switching."),
		  "Preload", Wt::WTabWidget::PreLoading);
// 		    ofs.close();
      tabW->addTab(new DeviceView(), "Devices", Wt::WTabWidget::PreLoading)->setStyleClass("thread");

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