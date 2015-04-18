 #ifndef DEVICEUI_PSA_HPP
#define DEVICEUI_PSA_HPP

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
#include "PSAPowersupply.hpp"
#include "DeviceUI.hpp"

namespace lughos
{

 template <> class DeviceUI<PSAPowersupply> : public DeviceUIInterface
  {
  protected:

    boost::shared_ptr<PSAPowersupply> powersupply;
    Wt::WLineEdit* voltageF;
    Wt::WLineEdit* currentF;
    Wt::WLineEdit* powerF;
    Wt::WLineEdit* temperaturF;
    Wt::WLabel* voltageL;
    Wt::WLabel* currentL;
    Wt::WLabel* powerL;
    Wt::WLabel* temperatureL;
    Wt::WPushButton *onB;
    Wt::WPushButton *offB;
    boost::shared_ptr<Wt::Dbo::Session> session;
    
    
    
  public:
    
    DeviceUI< PSAPowersupply >(boost::shared_ptr<Device> powersupply) : powersupply(boost::dynamic_pointer_cast<PSAPowersupply>(powersupply))
    {
      this->init();
    }
    
    DeviceUI<PSAPowersupply>(boost::shared_ptr<PSAPowersupply> powersupply) : powersupply(powersupply)
    {
      this->init();
    }
    
    void checkConnected()
    {
      if(powersupply->isConnected())
      {
        this->led->setState<Connected>();
	this->onB->setDisabled(false);
	this->offB->setDisabled(false);
        this->onB->clicked().connect(this,&DeviceUI<PSAPowersupply>::switchOn);
	this->offB->clicked().connect(this,&DeviceUI<PSAPowersupply>::switchOff);
	this->refresh();

      }
      else
      {
        this->led->setState<Disconnected>();
        this->onB->setDisabled(true);
        this->offB->setDisabled(true);
      }
    }
  
      void init()
    {
      std::cout << "PSA-powersupply init running..." << std::endl;
     this->name=powersupply->getName();
//      this->setWidth(500);
     this->setTitle(Wt::WString::fromUTF8(this->name.c_str()));
     this->voltageF = new Wt::WLineEdit("0.0");
     this->voltageF->setReadOnly(true);
     this->voltageL = new Wt::WLabel("Voltage");
     this->voltageL->setBuddy(voltageF);
     this->currentF =  new  Wt::WLineEdit("0.0");
     this->currentF->setReadOnly(true);
     this->currentL = new Wt::WLabel("Current");
     this->currentL->setBuddy(currentF);
     this->powerF =  new  Wt::WLineEdit("0.0");
     this->powerF->setReadOnly(true);
     this->powerL = new Wt::WLabel("Power");
     this->powerL->setBuddy(powerF);
     this->temperaturF =  new  Wt::WLineEdit("0.0");
     this->temperaturF->setReadOnly(true);
     this->temperatureL = new Wt::WLabel("Temperature");
     this->temperatureL->setBuddy(temperaturF);
     this->onB = new Wt::WPushButton("On");
     this->offB = new Wt::WPushButton("Off");
     this->addWidget(voltageL);
     this->addWidget(voltageF);
     this->addWidget(currentL);
     this->addWidget(currentF);
     this->addWidget(powerL);
     this->addWidget(powerF);
     this->addWidget(temperatureL);
     this->addWidget(temperaturF);
     this->addWidget(onB);
     this->addWidget(offB);
     this->onB->setDisabled(true);
     this->offB->setDisabled(true);
     this->checkConnected();

    }
    
     
    void getMeasurements()
    {
      measuredValue<double> voltage, current, power;
      voltage = this->powersupply->get_voltage();
      current = this->powersupply->get_current();
      power = voltage * current;
      power.setUnit("W");
      this->voltageF->setText(Wt::WString::fromUTF8(voltage.getString()));
      this->currentF->setText(Wt::WString::fromUTF8(current.getString()));
      this->temperaturF->setText(Wt::WString::fromUTF8(this->powersupply->get_temperature().getString()));
      
      this->powerF->setText(Wt::WString::fromUTF8(power.getString()));

    }
    
    void switchOn()
    {
      this->powersupply->on();
    }
    
    void switchOff()
    {
      this->powersupply->off();
    }
    
    void start()
    {

    }
    
    void stop()
    {

    }
    
    
    
  };
}
 #endif 
  