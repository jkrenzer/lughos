 #ifndef DEVICEUI_FUGNetzteil_HPP
#define DEVICEUI_FUGNetzteil_HPP

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
#include <Wt/Chart/WCartesianChart>
#include <Wt/Chart/WDataSeries>
#include <Wt/WAbstractItemModel>
#include <Wt/WAbstractItemView>
#include <Wt/WDate>
#include <Wt/WDateTime>
#include <Wt/WLocalDateTime>
#include <Wt/WPaintedWidget>
#include <Wt/WItemDelegate>
#include <Wt/WShadow>
#include <Wt/WStandardItemModel>
#include <Wt/WSpinBox>
#include <Wt/WDoubleSpinBox>
#include <Wt/WTableView>
#include <Wt/WTimer>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/QueryModel>
#include <Wt/Dbo/WtSqlTraits>
#include <functional>
#include "measuredValue.hpp"
#include "measuredDBValue.hpp"
#include "jobQueue.hpp"
#include "FUGNetzteil.hpp"
#include "DeviceUI.hpp"

namespace lughos 
{
  template <> class DeviceUI<FUGNetzteil> : public DeviceUIInterface
  {
  protected:
    boost::shared_ptr<FUGNetzteil> fug;
    Wt::WLineEdit* flowF;
    Wt::WLabel* uL;
    Wt::WLabel* iL;
    Wt::WLabel* measFlowL;
    Wt::WPushButton *sendIB;
    Wt::WLabel* iOutL;
    Wt::WLabel* uOutL;
    Wt::WLabel* pOutL;
    Wt::WLineEdit *iOutField;
    Wt::WLineEdit *uOutField;
    Wt::WLineEdit *pOutField;
    Wt::WDoubleSpinBox *iField;
    Wt::WPushButton *sendUB;
    Wt::WDoubleSpinBox *uMinField;
    Wt::WDoubleSpinBox *uMaxField;
//     Wt::WPushButton * startB;
    std::string LastError;
    Wt::WPushButton * onB;
    Wt::WPushButton * offB;
    Wt::WPushButton * resetOCB;
    boost::shared_ptr<Wt::WTimer> intervalTimer;
//     Wt::WPushButton * stopB;
    
  public:
    
    DeviceUI< FUGNetzteil >(boost::shared_ptr<Device> fug) : fug(boost::dynamic_pointer_cast<FUGNetzteil>(fug)), intervalTimer(new Wt::WTimer)
    {

      this->init();
    }
    
    DeviceUI<FUGNetzteil>(boost::shared_ptr<FUGNetzteil> fug) : fug(fug)
    {
      this->init();
    }
    
    void checkConnected()
    {
      if(fug->isConnected())
      {
	this->led->setState<Connected>();
	this->sendIB->setDisabled(false);
	this->iField->setDisabled(false);
	this->sendUB->setDisabled(false);
	this->uMinField->setDisabled(false);
	this->uMaxField->setDisabled(false);
        this->sendUB->clicked().connect(this,&DeviceUI<FUGNetzteil>::setU);
        this->sendIB->clicked().connect(this,&DeviceUI<FUGNetzteil>::setI);
	this->onB->clicked().connect(this,&DeviceUI< FUGNetzteil >::switchOn);
	this->offB->clicked().connect(this,&DeviceUI< FUGNetzteil >::switchOff);
	this->resetOCB->clicked().connect(this,&DeviceUI< FUGNetzteil >::resetOC);
	this->getState();
// 	intervalTimer->setInterval(2000);
// 	intervalTimer->timeout().connect(this,&DeviceUI< FUGNetzteil >::getState); // Reload state every 2 seconds
// 	intervalTimer->start();
      }
      else
      {
	intervalTimer->stop();
	this->led->setState<Disconnected>();
	this->sendIB->setDisabled(true);
	this->iField->setDisabled(true);
	this->sendUB->setDisabled(true);
	this->onB->setDisabled(true);
	this->offB->setDisabled(true);
	this->resetOCB->setDisabled(true);
	this->uMinField->setDisabled(true);
	this->uMaxField->setDisabled(true);

      }
    }
    
    void init()
    {
     this->name=fug->getName();
//      this->setWidth(500);
      this->setTitle(Wt::WString::fromUTF8(this->name.c_str()));
      
      this->uL = new Wt::WLabel("Set U min and max:");
      this->iL = new Wt::WLabel("Set I:");
      this->iOutL = new Wt::WLabel("I[A]:");
      this->uOutL = new Wt::WLabel("U[V]:");
      this->pOutL = new Wt::WLabel("P[Watt]:");

      this->iOutField =  new  Wt::WLineEdit("");
      this->iOutField->setReadOnly(true);      
      this->uOutField =  new  Wt::WLineEdit("");
      this->uOutField->setReadOnly(true);  
      this->pOutField =  new  Wt::WLineEdit("");
      this->pOutField->setReadOnly(true);  
      
      this->iField =  new  Wt::WDoubleSpinBox(0);

      this->uMinField =  new  Wt::WDoubleSpinBox(0);

      this->uMaxField =  new  Wt::WDoubleSpinBox(0);

      this->sendIB = new Wt::WPushButton("Set");
      this->sendUB = new Wt::WPushButton("Set");
      this->onB = new Wt::WPushButton("On");
      this->offB = new Wt::WPushButton("Off");
      this->resetOCB = new Wt::WPushButton("Reset OC");

      this->addWidget(iOutL);      
      this->addWidget(iOutField);
      this->addWidget(iL);
      this->addWidget(iField);
      this->addWidget(sendIB);
      this->addWidget(new Wt::WBreak); 
      this->addWidget(uOutL);    
      this->addWidget(uOutField);
      this->addWidget(uL);
//       this->addWidget(uMinField);
      this->addWidget(uMaxField);
      this->addWidget(sendUB); 
      this->addWidget(new Wt::WBreak);
      this->addWidget(pOutL);    
      this->addWidget(pOutField);
      this->addWidget(onB);
      this->addWidget(offB);
      this->addWidget(resetOCB);
      this->checkConnected();

    }
    
    void setU()
    {
//       

      stringstream sstr; 
//       string str = uMinField->text().toUTF8(); 
      float f; 
//       sstr<<str; 
//       sstr>>f;
      
      std::string str = uMaxField->text().toUTF8();
      sstr<<str; 
      sstr>>f;
      fug->setU(f);
      
      std::string Error;
      Error = fug->getLastError();
      if(Error!=this->LastError)
      {
	LastError=Error;
      }
      this->getState();
      
    }
    
        void setI()
    {
//       
      
      stringstream sstr; 
      string str = iField->text().toUTF8(); 
      double f; 
      sstr<<str; 
      sstr>>f;

      fug->setI(f);
//     
      std::string Error;
      Error = fug->getLastError();
      if(Error!=this->LastError)
      {
	LastError=Error;
      }
      
      this->getState();
    }
    
        void getU()
    {
   
      std::string str = std::to_string(fug->getSetpointU());
      this->uMaxField->setText(str);
      str = std::to_string(fug->getU());
      this->uOutField->setText(str);
//     
      std::string Error;
      Error = fug->getLastError();
      if(Error!=this->LastError)
      {
	LastError=Error;
      }  
    }
    
        void getI()
    {
      string str = std::to_string(fug->getSetpointI());
      this->iField->setText(str);
      str = std::to_string(fug->getI());
      this->iOutField->setText(str);

//       this->stateF->setText("Current set:"+iField->text().toUTF8());
//     
      std::string Error;
      Error = fug->getLastError();
      if(Error!=this->LastError)
      {
	LastError=Error;
      } 
    }
    
     void getRemoteState()
     {
       bool analogue = this->fug->getAnalogueRemote();
       bool digital = this->fug->getDigitalRemote();
       bool local = !analogue && !digital;
       if (digital)
	 this->led->setState<Connected>();
       else if(analogue)
       {
	 this->led->setColor(lughos::StatusLEDWtWidget::Red);
         this->led->setToolTip(Wt::WString("Analogue control mode. Cannot command device!"));
       }
       else
       {
	 this->led->setColor(lughos::StatusLEDWtWidget::Red);
         this->led->setToolTip(Wt::WString("Local control mode. Cannot command device!"));
       }
     }
     
     void getOC()
     {
       if(this->fug->getOvercurrent())
       {
         
       }
     }
     
     void resetOC()
     {
       this->fug->resetOvercurrent();
       this->getState();
     }
    
    void getMeasure()
    {
      measuredValue<double> v;
      std::stringstream ss;
      for (int i; i<8;i++)
      {
	v = this->fug->getMeasure();
	ss << "Channel " << i << ": " << v.getValueAsString() << v.getUnit() << std::endl;
      }
      std::string Error;
      Error = fug->getLastError();
      if(Error!=this->LastError)
      {
	LastError=Error;
      }
    }
    
    void switchOn()
    {
      this->fug->switchVoltage(1);
    }
    
    void switchOff()
    {
      this->fug->switchVoltage(0);
    }
   
    void getState()
    {
      this->getU();
      this->getI();
      this->getRemoteState();
      this->getOC();
    }
    
  };
} //namespace
 #endif 
  