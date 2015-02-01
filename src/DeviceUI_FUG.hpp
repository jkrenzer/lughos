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
    Wt::WPushButton *getIB;
    Wt::WLabel* iOutL;
    Wt::WLabel* uOutL;
    Wt::WLabel* pOutL;
    Wt::WLineEdit *iOutField;
    Wt::WLineEdit *uOutField;
    Wt::WLineEdit *pOutField;
    Wt::WDoubleSpinBox *iField;
    Wt::WPushButton *sendUB;
    Wt::WPushButton *getUB;
    Wt::WDoubleSpinBox *uMinField;
    Wt::WDoubleSpinBox *uMaxField;
    Wt::WTextArea *responseField;
//     Wt::WPushButton * startB;
    std::string LastError;
    Wt::WPushButton * stateB;
//     Wt::WPushButton * stopB;
    
  public:
    
    DeviceUI< FUGNetzteil >(boost::shared_ptr<Device> fug) : fug(boost::dynamic_pointer_cast<FUGNetzteil>(fug))
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
// 	measuredValue getMeasure(bool force=false);	
// 	int setI(double I );
// 	int setU(double I );
// 	double getI();
// 	double getU();
// 	std::string getLastError();
// 	std::string getIDN();
	
	this->stateF->setText("Connected!");
        this->stateB->setText("Status");
	this->stateB->clicked().connect(this,&DeviceUI<FUGNetzteil>::getMeasure);
	this->sendIB->setDisabled(false);
	this->getIB->setDisabled(false);
	this->iField->setDisabled(false);
	this->sendUB->setDisabled(false);
	this->getUB->setDisabled(false);
	this->uMinField->setDisabled(false);
	this->uMaxField->setDisabled(false);
        this->sendUB->clicked().connect(this,&DeviceUI<FUGNetzteil>::setU);
        this->sendIB->clicked().connect(this,&DeviceUI<FUGNetzteil>::setI);
	this->getUB->clicked().connect(this,&DeviceUI<FUGNetzteil>::getU);
        this->getIB->clicked().connect(this,&DeviceUI<FUGNetzteil>::getI);
	this->getState();

      }
      else
      {

	this->stateF->setText("Not connected!");
        this->stateB->setText("Try again");
	this->sendIB->setDisabled(true);
	this->iField->setDisabled(true);
	this->sendUB->setDisabled(true);
	this->getUB->setDisabled(true);
	this->getIB->setDisabled(true);
	this->uMinField->setDisabled(true);
	this->uMaxField->setDisabled(true);

      }
    }
    
    void init()
    {
     this->name=fug->getName();
//      this->setWidth(500);
      this->addWidget(new Wt::WText(this->name.c_str()));
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
      this->sendIB = new Wt::WPushButton("Get");
      this->sendUB = new Wt::WPushButton("Get");
      this->stateB = new Wt::WPushButton("Status");

      this->addWidget(iOutL);      
      this->addWidget(iOutField);
      this->addWidget(iL);
      this->addWidget(iField);
      this->addWidget(sendIB);
      this->addWidget(getIB);
      this->addWidget(new Wt::WBreak); 
      this->addWidget(uOutL);    
      this->addWidget(uOutField);
      this->addWidget(uL);
//       this->addWidget(uMinField);
      this->addWidget(uMaxField);
      this->addWidget(sendUB); 
      this->addWidget(getUB);
      this->addWidget(new Wt::WBreak);
      this->addWidget(pOutL);    
      this->addWidget(pOutField);
      this->addWidget(stateB);
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
   
      std::string str = std::to_string(fug->getU());
      this->uMaxField->setText(str);
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
//       
      
      stringstream sstr; 
      string str = std::to_string(fug->getI()); 

//       this->stateF->setText("Current set:"+iField->text().toUTF8());
      responseField->setText(responseField->text().toUTF8()+str);
      this->iOutField->setText(str);
//     
      std::string Error;
      Error = fug->getLastError();
      if(Error!=this->LastError)
      {
	LastError=Error;
      this->stateF->setText("Error: "+LastError);
      } 
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
    
   
    void getState()
    {
      this->getU();
      this->getI();
    }
    
  };

  
  


 #endif 
  