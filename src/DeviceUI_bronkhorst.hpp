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
    boost::shared_ptr<bronkhorst> horst;
    Wt::WLineEdit* stateF;
    Wt::WLineEdit* flowF;
    Wt::WLabel* stateL;
    Wt::WLabel* flowL;
    Wt::WLabel* measFlowL;
    Wt::WPushButton *sendB;
    Wt::WLineEdit *flowField;
    Wt::WTextArea *responseField;
//     Wt::WPushButton * startB;
    Wt::WPushButton * stateB;
//     Wt::WPushButton * stopB;
    boost::shared_ptr<dbo::Session> session;
//     dbo::backend::Sqlite3 dbBackend;
    
  public:
    
    DeviceUI< bronkhorst >(boost::shared_ptr<Device> horst) : horst(boost::dynamic_pointer_cast<bronkhorst>(horst))
    {

      this->init();
    }
    
    DeviceUI<bronkhorst>(boost::shared_ptr<bronkhorst> horst) : horst(horst)
    {
      this->init();
    }
    
    void checkConnected()
    {
      if(horst->isConnected())
      {
	this->stateF->setText("Connected!");
        this->stateB->setText("Status");
// 	this->startB->setDisabled(false);
// 	this->stopB->setDisabled(false);
// 	this->startB->clicked().connect(this,&DeviceUI<coolpak6000>::start);
// 	this->stopB->clicked().connect(this,&DeviceUI<coolpak6000>::stop);
	this->sendB->setDisabled(false);
	this->flowField->setDisabled(false);
        this->sendB->clicked().connect(this,&DeviceUI<bronkhorst>::setFlow);
	this->stateB->clicked().connect(this,&DeviceUI<bronkhorst>::getState);
	this->getState();

      }
      else
      {
	this->stateF->setText("Not connected!");
// 	this->stateF->setText(std::to_string(coolpak->isConnected()));
        this->stateB->setText("Try again");
 	this->stateB->clicked().connect(this,&DeviceUI<bronkhorst>::checkConnected);
// 	this->startB->setDisabled(true);
// 	this->stopB->setDisabled(true);

      }
    }
    
    void init()
    {
     this->name=horst->getName();
//      this->setWidth(500);
      this->addWidget(new Wt::WText(this->name.c_str()));
      this->stateF = new Wt::WLineEdit("Initializing...");
      this->stateF->setReadOnly(true);
      this->stateL = new Wt::WLabel("Status:");
      this->flowL = new Wt::WLabel("Set Flow:");
      this->stateL->setBuddy(stateF);
      this->flowField =  new  Wt::WLineEdit("0.0");
       this->sendB = new Wt::WPushButton("Send");
      this->stateB = new Wt::WPushButton("Status");
     this->addWidget(stateL);
     this->addWidget(stateF);
     this->addWidget(flowL);
     this->addWidget(flowField);
     this->addWidget(sendB);
     this->addWidget(stateB);
     this->sendB->setDisabled(true);
     this->flowField->setDisabled(true);
     this->responseField =  new Wt::WTextArea("");
     this->responseField->setReadOnly(true); 
     this->addWidget(responseField);
     this->checkConnected();

    }
    
    void setFlow()
    {
//       
      stringstream sstr; 
      string str = flowField->text().toUTF8(); 
      float f; 
      sstr<<str; 
      sstr>>f;

      this->stateF->setText("Flow set:"+flowField->text().toUTF8());
      responseField->setText(responseField->text().toUTF8()+horst->set_flow(f));
      this->getFlow();
//     
      
    }
    
    void getFlow()
    {
      measuredValue v = this->horst->get_value();
      this->responseField->setText(std::string(v.getStringValue())+std::string(v.getunit()));
      this->flowField->setText(std::string(v.getStringValue()));
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
  