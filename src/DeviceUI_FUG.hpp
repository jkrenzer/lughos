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
#include "basicUIElements.hpp"

namespace lughos 
{
  template <> class DeviceUI<FUGNetzteil> : public DeviceUITemplate<FUGNetzteil>
  {
  protected:
    Wt::WLabel* uL;
    Wt::WLabel* iL;
    Wt::WLabel* iOutL;
    Wt::WLabel* uOutL;
    ui::Measurement<Wt::WLineEdit> *current;
    ui::Measurement<::Wt::WLineEdit> *voltage;
    ui::Setting<Wt::WDoubleSpinBox> *currentSetpoint;
    ui::Setting<Wt::WDoubleSpinBox> *voltageSetpoint;
    Wt::WPushButton * onB;
    Wt::WPushButton * offB;
    Wt::WPushButton * resetOCB;

    
  public:
    
    DeviceUI< FUGNetzteil >(boost::shared_ptr<Device> fug) : DeviceUITemplate< FUGNetzteil >(fug)
    {
      this->uL = new Wt::WLabel("Set voltage [V]:");
      this->iL = new Wt::WLabel("Set current [A]:");
      this->iOutL = new Wt::WLabel("I[A]:");
      this->uOutL = new Wt::WLabel("U[V]:");
      this->voltage = new ui::Measurement<Wt::WLineEdit>();
      this->current = new ui::Measurement<Wt::WLineEdit>();
      this->voltageSetpoint = new ui::Setting<Wt::WDoubleSpinBox>();
      this->currentSetpoint = new ui::Setting<Wt::WDoubleSpinBox>();
      this->current->attach(this->device()->current);
      this->voltage->attach(this->device()->voltage);
      this->currentSetpoint->attach(this->device()->targetCurrent);
      this->voltageSetpoint->attach(this->device()->targetVoltage);
      this->uL->setBuddy(this->voltageSetpoint->field());
      this->iL->setBuddy(this->currentSetpoint->field());
      this->uOutL->setBuddy(this->voltage->field());
      this->iOutL->setBuddy(this->current->field());
      

      this->onB = new Wt::WPushButton("On");
      this->offB = new Wt::WPushButton("Off");
      this->resetOCB = new Wt::WPushButton("Reset OC");
      this->onB->clicked().connect(boost::bind(&exposedMeasurement<bool>::setValue,&(device()->outputState),true));
      this->offB->clicked().connect(boost::bind(&exposedMeasurement<bool>::setValue,&(device()->outputState),false));
      this->resetOCB->clicked().connect(boost::bind(&exposedMeasurement<bool>::setValue,&(device()->overcurrent),false));

      this->addWidget(iOutL);      
      this->addWidget(current);
      this->addWidget(iL);
      this->addWidget(currentSetpoint);
      this->addWidget(new Wt::WBreak); 
      this->addWidget(uOutL);    
      this->addWidget(voltage);
      this->addWidget(uL);
      this->addWidget(voltageSetpoint);
      this->addWidget(new Wt::WBreak);
      this->addWidget(onB);
      this->addWidget(offB);
      this->addWidget(resetOCB);
    }
    
    void enable()
    {
	this->onB->setDisabled(false);
	this->offB->setDisabled(false);
	this->resetOCB->setDisabled(false);
	this->voltageSetpoint->setDisabled(false);
	this->currentSetpoint->setDisabled(false);
    }
    
    void disable()
    {
	this->voltageSetpoint->setDisabled(false);
	this->currentSetpoint->setDisabled(true);
	this->onB->setDisabled(true);
	this->offB->setDisabled(true);
	this->resetOCB->setDisabled(true);
    }
    
//     void setU()
//     {
// //       
// 
//       stringstream sstr; 
// //       string str = uMinField->text().toUTF8(); 
//       float f; 
// //       sstr<<str; 
// //       sstr>>f;
//       
//       std::string str = uMaxField->text().toUTF8();
//       sstr<<str; 
//       sstr>>f;
//       fug->setU(f);
//       
//       std::string Error;
//       Error = fug->getLastError();
//       if(Error!=this->LastError)
//       {
// 	LastError=Error;
//       }
//       this->refresh();
//       
//     }
//     
//         void setI()
//     {
// //       
//       
//       stringstream sstr; 
//       string str = iField->text().toUTF8(); 
//       double f; 
//       sstr<<str; 
//       sstr>>f;
// 
//       fug->setI(f);
// //     
//       std::string Error;
//       Error = fug->getLastError();
//       if(Error!=this->LastError)
//       {
// 	LastError=Error;
//       }
//       
//       this->refresh();
//     }
//     
//         void getU()
//     {
//    
// 
//       string str = std::to_string(fug->getU());
//       this->uOutField->setText(str);
// //     
//       std::string Error;
//       Error = fug->getLastError();
//       if(Error!=this->LastError)
//       {
// 	LastError=Error;
//       }  
//     }
//     
//     void getSetpointU()
//     {
//       std::string str = std::to_string(fug->getSetpointU());
//       this->uMaxField->setText(str);
//       std::string Error;
//       Error = fug->getLastError();
//       if(Error!=this->LastError)
//       {
//         LastError=Error;
//       }
//     }
//     
//         void getI()
//     {
//       
//       string str = std::to_string(fug->getI());
//       this->iOutField->setText(str);
// 
// //       this->stateF->setText("Current set:"+iField->text().toUTF8());
// //     
//       std::string Error;
//       Error = fug->getLastError();
//       if(Error!=this->LastError)
//       {
// 	LastError=Error;
//       } 
//     }
//     
//     void getSetpointI()
//     {
//       string str = std::to_string(fug->getSetpointI());
//       this->iField->setText(str);
//       std::string Error;
//       Error = fug->getLastError();
//       if(Error!=this->LastError)
//       {
//         LastError=Error;
//       } 
//     }
//     
//      void getRemoteState()
//      {
//        bool analogue = this->fug->getAnalogueRemote();
//        bool digital = this->fug->getDigitalRemote();
//        bool local = !analogue && !digital;
//        if (digital)
// 	 return;
//        else if(analogue)
//        {
// 	 this->led->setColor(lughos::StatusLEDWtWidget::Red);
//          this->led->setToolTip(Wt::WString("Analogue control mode. Cannot command device!"));
//          this->led->setStatusMessage(Wt::WString("Analogue control mode. Cannot command device!"));
//        }
//        else
//        {
// 	 this->led->setColor(lughos::StatusLEDWtWidget::Red);
//          this->led->setToolTip(Wt::WString("Local control mode. Cannot command device!"));
//          this->led->setStatusMessage(Wt::WString("Local control mode. Cannot command device!"));
//        }
//      }
//      
//      void getOC()
//      {
//        if(this->fug->getOvercurrent())
//        {
//          std::cout << std::endl << std::endl << "############>>>>>>>>>>>> Overcurrent!" << std::endl << std::endl;
//          this->led->setColor(lughos::StatusLEDWtWidget::Orange);
//          this->led->setToolTip(Wt::WString("Overcurrent detected!"));
//          this->led->setStatusMessage(Wt::WString("Overcurrent detected!"));
//        }
//      }
//      
//      void resetOC()
//      {
//        this->fug->resetOvercurrent();
//        this->refresh();
//      }
//     
//     void getMeasure()
//     {
//       measuredValue<double> v;
//       std::stringstream ss;
//       for (int i; i<8;i++)
//       {
// 	v = this->fug->getMeasure();
// 	ss << "Channel " << i << ": " << v.getValueAsString() << v.getUnit() << std::endl;
//       }
//       std::string Error;
//       Error = fug->getLastError();
//       if(Error!=this->LastError)
//       {
// 	LastError=Error;
//       }
//     }
//     
//     void switchOn()
//     {
//       this->fug->switchVoltage(1);
//     }
//     
//     void switchOff()
//     {
//       this->fug->switchVoltage(0);
//     }
//     
//     void getMeasurements()
//     {
//       this->getU();
//       this->getI();
//     }
//     
//     void getSettings()
//     {
//       this->getSetpointU();
//       this->getSetpointI();
//     }
//     
//     void getStatusInformation()
//     {
//       this->led->setState<Connected>();
//       this->getOC();
//       this->getRemoteState();
//     }
    
  };
} //namespace
 #endif 
  