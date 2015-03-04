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

using namespace lughos;

  
 

    template <> class DeviceUI<FUGNetzteil> : public DeviceUIInterface
  {
  protected:
    boost::shared_ptr<FUGNetzteil> fug;
    Wt::WLineEdit* stateF;
    Wt::WLineEdit* flowF;
    Wt::WLabel* stateL;
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
    Wt::WTextArea *responseField;
//     Wt::WPushButton * startB;
    std::string LastError;
    Wt::WPushButton * stateB;
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
	this->stateF->setText(fug->getIDN());
        this->stateB->setText("Status");
	this->stateB->clicked().connect(this,&DeviceUI<FUGNetzteil>::getState);
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
	this->stateF->setText("Not connected!");
        this->stateB->setText("Try again");
	this->stateB->clicked().connect(this,&DeviceUI<FUGNetzteil>::checkConnected);
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
      this->stateF = new Wt::WLineEdit("Initializing...");
      this->stateF->setReadOnly(true);
    
      this->stateL = new Wt::WLabel("Status:");
      this->stateL->setBuddy(stateF);
  
      this->addWidget(stateL);
      this->addWidget(stateF); 
      
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

      this->sendIB = new Wt::WPushButton("Send");
      this->sendUB = new Wt::WPushButton("Send");
      this->onB = new Wt::WPushButton("On");
      this->offB = new Wt::WPushButton("Off");
      this->stateB = new Wt::WPushButton("Status");
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
      this->addWidget(stateB);
      this->addWidget(resetOCB);
//       this->sendIB->setDisabled(true);
//       this->iField->setDisabled(true);
//       this->sendUB->setDisabled(true);
//       this->uMinField->setDisabled(true);
//       this->uMaxField->setDisabled(true);
      this->responseField =  new Wt::WTextArea("");
      this->responseField->setReadOnly(true); 
      this->addWidget(responseField);
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
//       responseField->setText(responseField->text().toUTF8()+std::to_string(fug->set_voltage_min(str)));
      
      std::string str = uMaxField->text().toUTF8();
      sstr<<str; 
      sstr>>f;
      this->stateF->setText("Voltages set: "+uMaxField->text().toUTF8());
      responseField->setText(responseField->text().toUTF8()+std::to_string(fug->setU(f)));
      
      std::string Error;
      Error = fug->getLastError();
      if(Error!=this->LastError)
      {
	LastError=Error;
      this->stateF->setText("Error: "+LastError);
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

      this->stateF->setText("Current set:"+iField->text().toUTF8());
      responseField->setText(responseField->text().toUTF8()+std::to_string(fug->setI(f)));
//     
      std::string Error;
      Error = fug->getLastError();
      if(Error!=this->LastError)
      {
	LastError=Error;
      this->stateF->setText("Error: "+LastError);
      }
      
      this->getState();
    }
    
        void getU()
    {
   
      std::string str = std::to_string(fug->getSetpointU());
      this->uMaxField->setText(str);
      str = std::to_string(fug->getU());
      this->uOutField->setText(str);
      responseField->setText(responseField->text().toUTF8()+str);
//     
      std::string Error;
      Error = fug->getLastError();
      if(Error!=this->LastError)
      {
	LastError=Error;
      this->stateF->setText("Error: "+LastError);
      }  
    }
    
        void getI()
    {
      string str = std::to_string(fug->getSetpointI());
      this->iField->setText(str);
      str = std::to_string(fug->getI());
      this->iOutField->setText(str);

//       this->stateF->setText("Current set:"+iField->text().toUTF8());
      responseField->setText(responseField->text().toUTF8()+str);
//     
      std::string Error;
      Error = fug->getLastError();
      if(Error!=this->LastError)
      {
	LastError=Error;
      this->stateF->setText("Error: "+LastError);
      } 
    }
    
     void getRemoteState()
     {
       bool analogue = this->fug->getAnalogueRemote();
       bool digital = this->fug->getDigitalRemote();
       bool local = !analogue && !digital;
       if (digital)
	 this->stateF->setText("Remote on");
       else if(analogue)
	 this->stateF->setText("Analogue on");
       else
	 this->stateF->setText("Local control");
     }
     
     void getOC()
     {
       if(this->fug->getOvercurrent())
	 this->stateF->setText("Overcurrent!!");
     }
     
     void resetOC()
     {
       this->fug->resetOvercurrent();
       this->getState();
     }
    
    void getMeasure()
    {
      measuredValue v;
      std::stringstream ss;
      for (int i; i<8;i++)
      {
	v = this->fug->getMeasure();
	ss << "Channel " << i << ": " << v.getStringValue() << v.getunit() << std::endl;
      }
      this->stateF->setText(ss.str());
      std::string Error;
      Error = fug->getLastError();
      if(Error!=this->LastError)
      {
	LastError=Error;
      this->stateF->setText("Error: "+LastError);
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

  
  


 #endif 
  