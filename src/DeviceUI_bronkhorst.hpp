 #ifndef DEVICEUI_BRONKHORST_HPP
#define DEVICEUI_BRONKHORST_HPP

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
#include <Wt/WDoubleSpinBox>
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
#include <Wt/WToolBar>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/QueryModel>
#include <Wt/Dbo/WtSqlTraits>
#include <functional>
#include "measuredValue.hpp"
#include "measuredDBValue.hpp"
#include "jobQueue.hpp"
#include "bronkhorst.hpp"
#include "DeviceUI.hpp"
#include "basicUIElements.hpp"


namespace lughos

{

 template <> class DeviceUI<bronkhorst> : public DeviceUITemplate<bronkhorst>
  {
  protected:
    Wt::WLabel* capacityLabel;
    Wt::WLabel* flowLabel;
    Wt::WLabel* setpointLabel;
    ui::Measurement<Wt::WLineEdit>* flow;
    ui::Measurement<Wt::WLineEdit>* capacity;
    ui::Setting<Wt::WDoubleSpinBox>* setpoint;

    boost::shared_ptr<Wt::Dbo::Session> session;
//     dbo::backend::Sqlite3 dbBackend;
    
    
    
  public:
    
    DeviceUI< bronkhorst >(boost::shared_ptr<Device> device_) : DeviceUITemplate< bronkhorst >(device_)
    {
     this->setTitle(Wt::WString::fromUTF8(this->name.c_str()));
     this->capacityLabel = new Wt::WLabel("Capacity:");
     this->capacity = new ui::Measurement<Wt::WLineEdit>();
     this->flowLabel = new Wt::WLabel("Set Flow:");
     this->setpoint =  new  ui::Setting<Wt::WDoubleSpinBox>();
     this->setpointLabel = new Wt::WLabel("Measured Flow:");
     this->flow = new ui::Measurement<Wt::WLineEdit>();
     this->capacity->attach(device()->capacity);
     this->setpoint->attach(device()->setpoint);
     this->flow->attach(device()->flow);
     this->addWidget(capacityLabel);
     this->addWidget(capacity);
     this->addWidget(setpointLabel);
     this->addWidget(flow);
     this->addWidget(flowLabel);
     this->addWidget(setpoint);
    }
    
    void disable()
    {
      this->setpoint->setDisabled(true);
    }
    
    void enable()
    {
      this->setpoint->setDisabled(false);
    }
    
//     DeviceUI<bronkhorst>(boost::shared_ptr<bronkhorst> horst) : horst(horst)
//     {
//       this->init();
//     }
    
//     void checkConnected()
//     {
//       if(device()->isConnected())
//       {
//         this->setpoint->field()->setMaximum(this->devic->getMaxCapacity());
// 	this->setpoint->field()->setMinimum(0);
// 	this->setpoint->setDisabled(false);
//         this->setpoint->button()->clicked().connect(this,&DeviceUI<bronkhorst>::setFlow);
//         this->led->setState<Connected>();
//         this->refresh();
//       }
//       else
//       {
//         this->led->setState<Disconnected>();
//         this->setpoint->setDisabled(true);
//       }
//     }

//     void setFlow()
//     {
// //       
//       stringstream sstr; 
//       string str = setpoint->field()->text().toUTF8(); 
//       float f = lughos::save_lexical_cast<float>(str,0.0);
//       this->getSetpoint();
//       this->getFlow();
//     
      
//     }
    
//     void setFlow(boost::shared_ptr<bronkhorst> horst)
//     {
//       stringstream sstr; 
//       string str = setpoint->field()->text().toUTF8(); 
//       float f; 
//       sstr<<str; 
//       sstr>>f;
// 
//       this->getSetpoint();
//       this->getFlow();
//     }
    
//     void getSetpoint()
//     {
//       measuredValue<double> v = device()->get_setpoint();
//       this->setpoint->field()->setText(std::string(v.getValueAsString()));
//     }
    
//     void getFlow()
//     {
//       measuredValue<double> v = device()->get_flow();
//       this->flowMeasurementField1->setText(std::string(v.getValueAsString()));
//     }
/*    
    void start()
    {

    }
    
    void stop()
    {

    }*/
    
    
    
  };
} //namespace lughos
 #endif 
  