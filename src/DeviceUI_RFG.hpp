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
#include "basicUIElements.hpp"

namespace lughos
{ 
  template <> class DeviceUI<RFG> : public DeviceUITemplate<RFG>
  {
  protected:
    Wt::WLineEdit* flowF;
    Wt::WLabel* uL;
    Wt::WLabel* iL;
    Wt::WLabel* measFlowL;
    Wt::WLabel* iOutL;
    Wt::WLabel* uOutL;
    Wt::WLabel* pOutL;
//     Wt::WLineEdit *iOutField;
//     Wt::WLineEdit *uOutField;
//     Wt::WLineEdit *pOutField;
    ui::Measurement<Wt::WLineEdit>* current;
    ui::Measurement<Wt::WLineEdit>* voltage;
    ui::Measurement<Wt::WLineEdit>* power;
//     Wt::WDoubleSpinBox *iField;
    ui::Setting<Wt::WDoubleSpinBox>* currentMax;
//     Wt::WPushButton *sendUB;
//     Wt::WDoubleSpinBox *uMinField;
//     Wt::WDoubleSpinBox *uMaxField;
    ui::Setting<Wt::WDoubleSpinBox>* voltageMin;
    ui::Setting<Wt::WDoubleSpinBox>* voltageMax;
    Wt::WLabel* targetFieldL;
//     Wt::WDoubleSpinBox *targetField;
    ui::Setting<Wt::WDoubleSpinBox>* target;
//     Wt::WPushButton *sendPB;
    Wt::WPushButton *sendOnB;
    Wt::WPushButton *sendOffB;
    Wt::WLabel* modesL;
    Wt::WPushButton *voltageControl;
    Wt::WPushButton *currentControl;
    Wt::WPushButton *powerControl;
    
  public:
    
    DeviceUI< RFG >(boost::shared_ptr<Device> rfg) : DeviceUITemplate< RFG >(rfg)
    {
this->name=device()->getName();
//      this->setWidth(500);

      this->setTitle(Wt::WString::fromUTF8(this->name.c_str()));
      
      this->uL = new Wt::WLabel("Set U min and max [V]:");
      this->iL = new Wt::WLabel("Set I [A]:");
      this->iOutL = new Wt::WLabel("I[A]:");
      this->uOutL = new Wt::WLabel("U[V]:");
      this->pOutL = new Wt::WLabel("P[W]:");

      this->current = new ui::Measurement<Wt::WLineEdit>();
      this->voltage = new ui::Measurement<Wt::WLineEdit>();
      this->power = new ui::Measurement<Wt::WLineEdit>();
      
      this->currentMax = new ui::Setting<Wt::WDoubleSpinBox>();
      this->voltageMax = new ui::Setting<Wt::WDoubleSpinBox>();
      this->voltageMin = new ui::Setting<Wt::WDoubleSpinBox>();
      this->target = new ui::Setting<Wt::WDoubleSpinBox>();
      
      this->currentMax->field()->setMaximum(5);
      this->currentMax->field()->setMinimum(0);
      this->voltageMax->field()->setMinimum(0);
      this->voltageMax->field()->setMaximum(40);
      this->voltageMin->field()->setMinimum(0);
      this->voltageMin->field()->setMaximum(40);
      //TODO Set Maximums from device mode!
      this->targetFieldL = new Wt::WLabel("Target voltage:");
      this->target->field()->setMinimum(0);
      this->target->field()->setMaximum(190);
      
      this->voltage->attach(this->device()->voltage);
      this->current->attach(this->device()->current);
      this->target->attach(this->device()->target);
      this->power->attach(this->device()->power);
      this->currentMax->attach(this->device()->currentLimitMax);
      this->voltageMax->attach(this->device()->voltageLimitMax);
      this->voltageMin->attach(this->device()->voltageLimitMin);
      this->device()->controller.onValueChange.connect(boost::bind(&DeviceUI< RFG >::determinTarget,this));

            
      this->sendOnB = new Wt::WPushButton("On");
      this->sendOffB = new Wt::WPushButton("Off");
      
      this->modesL = new Wt::WLabel("Regulation Mode:");
      this->voltageControl = new Wt::WPushButton("Voltage");
      this->currentControl = new Wt::WPushButton("Current");
      this->powerControl = new Wt::WPushButton("Power");
      
      
        this->sendOnB->clicked().connect(boost::bind(&DeviceUI< RFG >::setOutput,this,true));
	this->sendOffB->clicked().connect(boost::bind(&DeviceUI< RFG >::setOutput,this,false));
	this->voltageControl->clicked().connect(boost::bind(&DeviceUI< RFG >::setController,this,RFG::Controller::Voltage));
	this->currentControl->clicked().connect(boost::bind(&DeviceUI< RFG >::setController,this,RFG::Controller::Current));
	this->powerControl->clicked().connect(boost::bind(&DeviceUI< RFG >::setController,this,RFG::Controller::Power));
      
      this->addWidget(iOutL);      
      this->addWidget(current);
      this->addWidget(iL);
      this->addWidget(currentMax);
      this->addWidget(new Wt::WBreak); 
      this->addWidget(uOutL);    
      this->addWidget(voltage);
      this->addWidget(uL);
      this->addWidget(voltageMax);
      this->addWidget(voltageMin);
      this->addWidget(new Wt::WBreak);
      this->addWidget(pOutL);    
      this->addWidget(power);
      this->addWidget(new Wt::WBreak);
      this->addWidget(targetFieldL);
      this->addWidget(target);
      this->addWidget(new Wt::WBreak);
      
      this->addWidget(voltageControl);
      this->addWidget(currentControl);
      this->addWidget(powerControl);
      this->addWidget(new Wt::WBreak);
      this->addWidget(sendOnB);
      this->addWidget(sendOffB);
      
      this->device()->onConnect.connect(boost::bind(&DeviceUI< RFG >::enable_,this));
      this->device()->onDisconnect.connect(boost::bind(&DeviceUI< RFG >::disable_,this));
      
    }
    
    void disable_()
    {
      this->currentMax->setDisabled(true);
	this->voltageMax->setDisabled(true);
	this->voltageMin->setDisabled(true);
	this->target->setDisabled(true);
	this->sendOnB->setDisabled(true);
	this->sendOffB->setDisabled(true);

	this->voltageControl->setDisabled(true);
	this->currentControl->setDisabled(true);
	this->powerControl->setDisabled(true);
    }
    
    void enable_()
    {
	this->currentMax->setDisabled(false);
	this->voltageMax->setDisabled(false);
	this->voltageMin->setDisabled(false);
	this->target->setDisabled(false);
	this->sendOnB->setDisabled(false);
	this->sendOffB->setDisabled(false);

	this->voltageControl->setDisabled(false);
	this->currentControl->setDisabled(false);
	this->powerControl->setDisabled(false);
	this->determinTarget();
    }
    
    void setController(RFG::Controller controller)
    {
      this->device()->controller.setValue(controller);
    }
    
    void setOutput(bool onOff)
    {
      this->device()->output.setValue(onOff);
    }
    
    void determinTarget()
    {
      switch (device()->controller)
      {
	default:
	case RFG::Controller::Voltage: this->targetFieldL->setText("Target voltage [V]:"); break;
	case RFG::Controller::Current: this->targetFieldL->setText("Target current [A]:"); break;
	case RFG::Controller::Power: this->targetFieldL->setText("Target power [W]:"); break;
      }
    }


//     }
    
    
    
  };
}
 #endif 
  
