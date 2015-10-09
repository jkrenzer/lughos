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
    
    void refresh()
    {
      this->current->refresh();
      this->voltage->refresh();
      this->currentSetpoint->refresh();
      this->voltageSetpoint->refresh();
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
  };
} //namespace
 #endif 
  