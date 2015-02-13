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
#include "bronkhorst.hpp"
#include "DeviceUI.hpp"


using namespace lughos;

 template <> class DeviceUI<bronkhorst> : public DeviceUIInterface
  {
  protected:

    boost::shared_ptr<bronkhorst> horst1;
    boost::shared_ptr<bronkhorst> horst2;
    Wt::WLineEdit* stateF;
    Wt::WLineEdit* flowF1;
    Wt::WLabel* stateL1;
    Wt::WLabel* flowL1;
    Wt::WLabel* measFlowL1;
    Wt::WPushButton *sendB1;
    Wt::WLineEdit *flowField1;
    Wt::WTextArea *responseField;
    bool numberOfHorsts=1;
    Wt::WPushButton * stateB1;
//     Wt::WPushButton * stopB;
    boost::shared_ptr<dbo::Session> session;
//     dbo::backend::Sqlite3 dbBackend;
    
    
    
  public:
    
    DeviceUI< bronkhorst >(boost::shared_ptr<Device> horst1) : horst1(boost::dynamic_pointer_cast<bronkhorst>(horst1))
    {
      numberOfHorsts=1;
      this->init();
    }
    
    DeviceUI<bronkhorst>(boost::shared_ptr<bronkhorst> horst1) : horst1(horst1)
    {
      numberOfHorsts=1;
      this->init();
    }
    
//     DeviceUI< bronkhorst >(boost::shared_ptr<Device> horst1, boost::shared_ptr<Device> horst2) : horst1(boost::dynamic_pointer_cast<bronkhorst>(horst1)), horst2(boost::dynamic_pointer_cast<bronkhorst>(horst2))
//     {
//       numberOfHorsts=2;
//       this->init1();
//     }
//     
//     DeviceUI<bronkhorst>(boost::shared_ptr<Device> horst1, boost::shared_ptr<Device> horst2) : horst1(horst1), horst2(horst2)
//     {
//       numberOfHorsts=2;
//       this->init1();
//     }
//     
    void checkConnected()
    {
      if(horst1->isConnected())
      {
	this->stateF1->setText("Connected!");
        this->stateB1->setText("Status");
// 	this->startB->setDisabled(false);
// 	this->stopB->setDisabled(false);
// 	this->startB->clicked().connect(this,&DeviceUI<coolpak6000>::start);
// 	this->stopB->clicked().connect(this,&DeviceUI<coolpak6000>::stop);
	this->sendB1->setDisabled(false);
	this->flowField1->setDisabled(false);
        this->sendB1->clicked().connect(this,&DeviceUI<bronkhorst>::setFlow);
	this->stateB1->clicked().connect(this,&DeviceUI<bronkhorst>::getState);
	this->getState();

      }
      else
      {
	this->stateF->setText("Not connected!");
// 	this->stateF->setText(std::to_string(coolpak->isConnected()));
        this->stateB1->setText("Try again");
 	this->stateB1->clicked().connect(this,&DeviceUI<bronkhorst>::checkConnected);
// 	this->startB->setDisabled(true);
// 	this->stopB->setDisabled(true);

      }
    }
    
    void init()
    {
      if(numberOfHorsts==1)  this->init1();
      if(numberOfHorsts==2)  this->init2();
    }
    
       void init1()
    {
     this->name=horst1->getName();
//      this->setWidth(500);
      this->addWidget(new Wt::WText(this->name.c_str()));
      this->stateF = new Wt::WLineEdit("Initializing...");
      this->stateF->setReadOnly(true);
      this->stateL1 = new Wt::WLabel("Status:");
      this->flowL1 = new Wt::WLabel("Set Flow:");
      this->stateL1->setBuddy(stateF);
      this->flowField1 =  new  Wt::WLineEdit("0.0");
       this->sendB1 = new Wt::WPushButton("Send");
      this->stateB1 = new Wt::WPushButton("Status");
     this->addWidget(stateL1);
     this->addWidget(stateF);
     this->addWidget(flowL1);
     this->addWidget(flowField1);
     this->addWidget(sendB1);
     this->addWidget(stateB1);
     this->sendB1->setDisabled(true);
     this->flowField1->setDisabled(true);
     this->responseField =  new Wt::WTextArea("");
     this->responseField->setReadOnly(true); 
     this->addWidget(responseField);
     this->checkConnected();

    }
    
 void init2()
    {
     this->name=horst1->getName();
//      this->setWidth(500);
      this->addWidget(new Wt::WText(this->name.c_str()));
      this->stateF = new Wt::WLineEdit("Initializing...");
      this->stateF->setReadOnly(true);
      this->stateL1 = new Wt::WLabel("Status:");
      this->flowL1 = new Wt::WLabel("Set Flow:");
      this->stateL1->setBuddy(stateF);
      this->flowField1 =  new  Wt::WLineEdit("0.0");
       this->sendB1 = new Wt::WPushButton("Send");
      this->stateB1 = new Wt::WPushButton("Status");
     this->addWidget(stateL1);
     this->addWidget(stateF);
     this->addWidget(flowL1);
     this->addWidget(flowField1);
     this->addWidget(sendB1);
     this->addWidget(stateB1);
     this->sendB1->setDisabled(true);
     this->flowField1->setDisabled(true);
     this->responseField =  new Wt::WTextArea("");
     this->responseField->setReadOnly(true); 
     this->addWidget(responseField);
     this->checkConnected();

    }
    
    void setFlow()
    {
//       
      stringstream sstr; 
      string str = flowField1->text().toUTF8(); 
      float f; 
      sstr<<str; 
      sstr>>f;

      this->stateF->setText("Flow set:"+flowField1->text().toUTF8());
      responseField->setText(responseField->text().toUTF8()+horst1->set_flow(f));
      this->getFlow();
//     
      
    }
    
    void getFlow()
    {
      measuredValue v = this->horst1->get_setpoint();
      this->responseField->setText(std::string(v.getStringValue())+std::string(v.getunit()));
      this->flowField1->setText(std::string(v.getStringValue()));
    }
    
    void getState()
    {
      this->getFlow();
    }
    
    void start()
    {

    }
    
    void stop()
    {

    }
    
    
    
  };

 #endif 
  