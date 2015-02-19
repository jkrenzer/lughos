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
    Wt::WLabel* targetFieldL;
    Wt::WDoubleSpinBox *targetField;
    Wt::WPushButton *sendPB;
    Wt::WPushButton *sendOnB;
    Wt::WPushButton *sendOffB;
    Wt::WLabel* modesL;
    Wt::WCheckBox* rawMode;
    Wt::WPushButton *voltageControl;
    Wt::WPushButton *currentControl;
    Wt::WPushButton *powerControl;
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
	this->targetField->setDisabled(false);
	this->voltageControl->setDisabled(false);
	this->currentControl->setDisabled(false);
	this->powerControl->setDisabled(false);
	this->rawMode->setDisabled(false);
        this->sendUB->clicked().connect(this,&DeviceUI<RFG>::setU);
        this->sendIB->clicked().connect(this,&DeviceUI<RFG>::setI);
	this->sendPB->clicked().connect(this,&DeviceUI<RFG>::setTargetValue);
        this->sendOnB->clicked().connect(this,&DeviceUI<RFG>::switchOn);
	this->sendOffB->clicked().connect(this,&DeviceUI<RFG>::switchOff);
	this->voltageControl->clicked().connect(this,&DeviceUI<RFG>::setTargetVoltage);
	this->currentControl->clicked().connect(this,&DeviceUI<RFG>::setTargetCurrent);
	this->powerControl->clicked().connect(this,&DeviceUI<RFG>::setTargetPower);
	this->rawMode->checked().connect(this,&DeviceUI<RFG>::setRawMode);
	this->rawMode->unChecked().connect(this,&DeviceUI<RFG>::unsetRawMode);
	this->getState();
	this->setTargetVoltage();

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
	this->targetField->setDisabled(true);
	this->voltageControl->setDisabled(true);
	this->currentControl->setDisabled(true);
	this->powerControl->setDisabled(true);
	this->rawMode->setDisabled(true);

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
      
      this->uL = new Wt::WLabel("Set U min and max [V]:");
      this->iL = new Wt::WLabel("Set I [A]:");
      this->iOutL = new Wt::WLabel("I[A]:");
      this->uOutL = new Wt::WLabel("U[V]:");
      this->pOutL = new Wt::WLabel("P[W]:");

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
      //TODO Set Maximums from device mode!
      this->targetFieldL = new Wt::WLabel("Target voltage:");
      this->targetField = new  Wt::WDoubleSpinBox(0);
      this->targetField->setMinimum(0);
      this->targetField->setMaximum(190);
      this->sendIB = new Wt::WPushButton("Send");
      this->sendUB = new Wt::WPushButton("Send");
      this->sendOnB = new Wt::WPushButton("On");
      this->sendOffB = new Wt::WPushButton("Off");
      this->sendPB = new Wt::WPushButton("Send");
      this->stateB = new Wt::WPushButton("Status");
      this->modesL = new Wt::WLabel("Regulation Mode:");
      this->voltageControl = new Wt::WPushButton("Voltage");
      this->currentControl = new Wt::WPushButton("Current");
      this->powerControl = new Wt::WPushButton("Power");
      this->rawMode = new Wt::WCheckBox;

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
      this->addWidget(new Wt::WBreak);
      this->addWidget(targetFieldL);
      this->addWidget(targetField);
      this->addWidget(sendPB); 
      this->addWidget(new Wt::WBreak);
      
      this->addWidget(voltageControl);
      this->addWidget(currentControl);
      this->addWidget(powerControl);
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
    
    void setRawMode()
    {
      this->iField->setMaximum(65535);
      this->iField->setMinimum(0);
      this->iField->setDecimals(0);
      this->uMaxField->setMinimum(0);
      this->uMaxField->setMaximum(65535);
      this->uMaxField->setDecimals(0);
      this->uMinField->setMinimum(0);
      this->uMinField->setMaximum(65535);
      this->uMinField->setDecimals(0);
      this->targetField->setMinimum(0);
      this->targetField->setMaximum(65535);
      this->targetField->setDecimals(0);
      this->getState();
    }
    
    void unsetRawMode()
    {
      this->iField->setMaximum(5);
      this->iField->setMinimum(0);
      this->iField->setDecimals(1);
      this->uMaxField->setMinimum(0);
      this->uMaxField->setMaximum(40);
      this->uMaxField->setDecimals(1);
      this->uMinField->setMinimum(0);
      this->uMinField->setMaximum(40);
      this->uMinField->setDecimals(1);
      this->targetField->setMinimum(0);
      this->targetField->setMaximum(40);
      this->targetField->setDecimals(1);
      this->getState();
    }
    
    void setU()
    {
//       

      float f = uMinField->value(); 
      if(this->rawMode->isChecked())
	responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_voltage_min_raw(f)));
      else
	responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_voltage_min(f)));
      f = uMaxField->value();
      this->stateF->setText("Voltages set: Min: "+uMinField->text().toUTF8()+" Max: "+uMaxField->text().toUTF8());
      if(this->rawMode->isChecked())
	responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_voltage_max_raw(f)));
      else
	responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_voltage_max(f)));
//     
      
    }
    
        void setI()
    {
//       
      float f = iField->value(); 
      this->stateF->setText("Current set:"+iField->text().toUTF8());
      if (this->rawMode->isChecked())
	responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_current_lim_raw(f)));
      else
	responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_current_lim(f)));
//     
      
    }
    
    void setTargetValue()
    {
      float f = targetField->value(); 
      this->stateF->setText("Target set:"+iField->text().toUTF8());
      if (this->rawMode->isChecked())
	responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_target_value_raw(f)));
      else
	responseField->setText(responseField->text().toUTF8()+std::to_string(rfg->set_target_value(f)));
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
    
    void setTargetVoltage()
    {
      this->targetFieldL->setText("Target voltage [V]:");
      this->rfg->use_voltage_controler();
    }
    
    void setTargetCurrent()
    {
      this->targetFieldL->setText("Target current [A]:");
      this->rfg->use_current_controler();
    }
    
    void setTargetPower()
    {
      this->targetFieldL->setText("Target power [W]:");
      this->rfg->use_power_controler();
    }
    
    void getState()
    {
      measuredValue v;
      std::stringstream ss;
      this->rfg->readout(this->rawMode->isChecked());
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
      this->targetField->setValue(this->rfg->getTargetValue());
      this->uOutField->setValue(this->rfg->get_channel(0).getStringValue());
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
  
