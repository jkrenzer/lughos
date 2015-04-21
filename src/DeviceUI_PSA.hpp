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
#include "basicUIElements.hpp"

namespace lughos
{

 template <> class DeviceUI<PSAPowersupply> : public DeviceUITemplate<PSAPowersupply>
  {
  protected:

    ui::Measurement<::Wt::WLineEdit>* voltageF;
    ui::Measurement<::Wt::WLineEdit>* currentF;
    ui::Measurement<::Wt::WLineEdit>* temperaturF;
    Wt::WLabel* voltageL;
    Wt::WLabel* currentL;
    Wt::WLabel* temperatureL;
    Wt::WPushButton *onB;
    Wt::WPushButton *offB;
       
    
  public:
    
    DeviceUI< PSAPowersupply >(boost::shared_ptr<Device> powersupply) : DeviceUITemplate< PSAPowersupply >(powersupply)
    {
            std::cout << "PSA-powersupply init running..." << std::endl;
     this->setTitle(Wt::WString::fromUTF8(this->name.c_str()));
     this->voltageF = new ui::Measurement<::Wt::WLineEdit>;
     this->voltageL = new Wt::WLabel("Voltage");
     this->voltageL->setBuddy(voltageF->field());
     this->currentF =  new  ui::Measurement<::Wt::WLineEdit>;
     this->currentL = new Wt::WLabel("Current");
     this->currentL->setBuddy(currentF->field());
     this->temperaturF =  new ui::Measurement<::Wt::WLineEdit>;
     this->temperatureL = new Wt::WLabel("Temperature");
     this->temperatureL->setBuddy(temperaturF->field());
     this->onB = new Wt::WPushButton("On");
     this->offB = new Wt::WPushButton("Off");
     this->addWidget(voltageL);
     this->addWidget(voltageF);
     this->addWidget(currentL);
     this->addWidget(currentF);
     this->addWidget(temperatureL);
     this->addWidget(temperaturF);
     this->addWidget(onB);
     this->addWidget(offB);
     this->onB->setDisabled(true);
     this->offB->setDisabled(true);
     this->onB->clicked().connect(boost::bind(&exposedMeasurement<bool>::setValue,&(device()->state),true));
     this->onB->clicked().connect(boost::bind(&exposedMeasurement<bool>::setValue,&(device()->state),false));
     this->voltageF->attach(device()->voltage);
     this->currentF->attach(device()->current);
     this->temperaturF->attach(device()->temperature);
     this->device()->onConnect.connect(boost::bind(&DeviceUI< PSAPowersupply >::enable_,this));
     this->device()->onDisconnect.connect(boost::bind(&DeviceUI< PSAPowersupply >::disable_,this));
    }

    void enable_()
    {
      this->onB->setDisabled(false);
	this->offB->setDisabled(false);
    }
    
    void disable_()
    {
              this->onB->setDisabled(true);
        this->offB->setDisabled(true);
    }

  };
}
 #endif 
  