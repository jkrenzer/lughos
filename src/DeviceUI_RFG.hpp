 #ifndef DEVICEUI_RFG_HPP
#define DEVICEUI_RFG_HPP

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
#include "RFG.hpp"
#include "DeviceUI.hpp"

using namespace lughos;

  
 

    template <> class DeviceUI<RFG> : public DeviceUIInterface
  {
  protected:
    boost::shared_ptr<RFG> rfg;
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
    Wt::WDoubleSpinBox *pMaxField;
    Wt::WPushButton *sendPB;
    Wt::WPushButton *sendOnB;
    Wt::WPushButton *sendOffB;
    Wt::WTextArea *responseField;
//     Wt::WPushButton * startB;
    Wt::WPushButton * stateB;
//     Wt::WPushButton * stopB;
    
  public:
    
    DeviceUI< RFG >(boost::shared_ptr<Device> rfg) : rfg(boost::dynamic_pointer_cast<RFG>(rfg))
    {

      this->init();
    }
    
    DeviceUI<RFG>(boost::shared_ptr<RFG> rfg) : rfg(rfg)
    {
      this->init();
    }
    
    void checkConnected()
    {
      if(rfg->isConnected())
      {
	this->stateF->setText("Connected!");
        this->stateB->setText("Status");
	this->stateB->clicked().connect(this,&DeviceUI<RFG>::getState);
	this->sendIB->setDisabled(false);
	this->iField->setDisabled(false);
	this->sendUB->setDisabled(false);
	this->sendPB->setDisabled(false);
	this->sendOnB->setDisabled(false);
	this->sendOffB->setDisabled(false);
	this->uMinField->setDisabled(false);
	this->uMaxField->setDisabled(false);
	this->pMaxField->setDisabled(false);
        this->sendUB->clicked().connect(this,&DeviceUI<RFG>::setU);
        this->sendIB->clicked().connect(this,&DeviceUI<RFG>::setI);
	this->sendPB->clicked().connect(this,&DeviceUI<RFG>::setP);
        this->sendOnB->clicked().connect(this,&DeviceUI<RFG>::switchOn);
	this->sendOffB->clicked().connect(this,&DeviceUI<RFG>::switchOff);
	this->getState();

      }
      else
      {

	this->stateF->setText("Not connected!");
        this->stateB->setText("Try again");
	this->stateB->clicked().connect(this,&DeviceUI<RFG>::checkConnected);
	this->sendIB->setDisabled(true);
	this->iField->setDisabled(true);
	this->sendUB->setDisabled(true);
	this->sendPB->setDisabled(true);
	this->sendOnB->setDisabled(true);
	this->sendOffB->setDisabled(true);
	this->uMinField->setDisabled(true);
	this->uMaxField->setDisabled(true);
	this->pMaxField->setDisabled(true);

      }
    }
    
    void init()
    {
     this->name=rfg->getName();
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
      this->iField->setMaximum(5);
      this->iField->setMinimum(0);
      this->uMinField =  new  Wt::WDoubleSpinBox(0);
      this->uMinField->setMinimum(0);
      this->uMinField->setMaximum(40);
      this->uMaxField =  new  Wt::WDoubleSpinBox(0);
      this->uMaxField->setMinimum(0);
      this->uMaxField->setMaximum(40);
      this->pMaxField = new  Wt::WDoubleSpinBox(0);
      this->pMaxField->setMinimum(0);
      this->pMaxField->setMaximum(190);
      this->sendIB = new Wt::WPushButton("Send");
      this->sendUB = new Wt::WPushButton("Send");
      this->sendOnB = new Wt::WPushButton("On");
      this->sendOffB = new Wt::WPushButton("Off");
      this->sendPB = new Wt::WPushButton("Send");
      this->stateB = new Wt::WPushButton("Status");

      this->addWidget(iOutL);      
      this->addWidget(iOutField);
      this->addWidget(iL);
      this->addWidget(iField);
      this->addWidget(sendIB);
      this->addWidget(new Wt::WBreak); 
      this->addWidget(uOutL);    
      this->addWidget(uOutField);
      this->addWidget(uL);
      this->addWidget(uMinField);
      this->addWidget(uMaxField);
      this->addWidget(sendUB); 
      this->addWidget(new Wt::WBreak);
      this->addWidget(pOutL);    
      this->addWidget(pOutField);
      this->addWidget(pMaxField);
      this->addWidget(sendPB); 
      this->addWidget(new Wt::WBreak);
      this->addWidget(sendOnB);
      this->addWidget(sendOffB);
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

      float f = uMinField->value(); 
      responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_voltage_min(f)));
      f = uMaxField->value();
      this->stateF->setText("Voltages set: Min: "+uMinField->text().toUTF8()+" Max: "+uMaxField->text().toUTF8());
      responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_voltage_max(f)));
//     
      
    }
    
        void setI()
    {
//       
      float f = iField->value(); 
      this->stateF->setText("Current set:"+iField->text().toUTF8());
      responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_current_lim(f)));
//     
      
    }
    
    void setP()
    {
      float f = pMaxField->value(); 
      this->stateF->setText("Power set:"+iField->text().toUTF8());
      responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_power_lim(f)));
    }
    
    void switchOn()
    {
      rfg->switch_on();
      this->stateF->setText("Switched to ON.");
    }
    
    void switchOff()
    {
      rfg->switch_off();
      this->stateF->setText("Switched to OFF.");
    }
    
    void getState()
    {
      measuredValue v;
      std::stringstream ss;
      this->rfg->readout();
      if (this->rfg->isConnected())
	
      for (int i; i<8;i++)
      {
	v = this->rfg->get_channel(i);
	ss << "Channel " << i << ": " << v.getStringValue() << v.getunit() << std::endl;
      }
      this->stateF->setText(ss.str());
      this->uMaxField->setValue(this->rfg->getLimitMaxVoltage());
      this->uMinField->setValue(this->rfg->getLimitMinVoltage());
      this->iField->setValue(this->rfg->getLimitMaxCurrent());
      this->pMaxField->setValue(this->rfg->getPower());
    }
    
//     void start()
//     {
// 
//     }
//     
//     void stop()
//     {
// 
//     }
    
    
    
  };

  
  


 #endif 
  