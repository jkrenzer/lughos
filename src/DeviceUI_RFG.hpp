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
    Wt::WLineEdit *iField;
    Wt::WPushButton *sendUB;
    Wt::WLineEdit *uMinField;
    Wt::WLineEdit *uMaxField;
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
// 	this->startB->setDisabled(false);
// 	this->stopB->setDisabled(false);
// 	this->startB->clicked().connect(this,&DeviceUI<coolpak6000>::start);
// 	this->stopB->clicked().connect(this,&DeviceUI<coolpak6000>::stop);
        this->sendIB->clicked().connect(this,&DeviceUI<RFG>::setU);
        this->sendIB->clicked().connect(this,&DeviceUI<RFG>::setI);
	this->getState();

      }
      else
      {

	this->stateF->setText("Not connected!");
// 	this->stateF->setText(std::to_string(coolpak->isConnected()));
        this->stateB->setText("Try again");
// 	this->stateB->clicked().connect(this,&DeviceUI<coolpak6000>::checkConnected);
// 	this->startB->setDisabled(true);
// 	this->stopB->setDisabled(true);

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
      
      this->iField =  new  Wt::WLineEdit("");
      this->uMinField =  new  Wt::WLineEdit("");
      this->uMaxField =  new  Wt::WLineEdit("");
      this->sendIB = new Wt::WPushButton("Send");
      this->sendUB = new Wt::WPushButton("Send");
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
      this->addWidget(stateB);
      this->sendIB->setDisabled(true);
      this->iField->setDisabled(true);
      this->sendUB->setDisabled(true);
      this->uMinField->setDisabled(true);
      this->uMaxField->setDisabled(true);
  //      this->responseField =  new Wt::WTextArea("");
  //      this->responseField->setReadOnly(true); 
  //      this->addWidget(responseField);
      this->checkConnected();

    }
    
    void setU()
    {
//       
      
      stringstream sstr; 
      string str = iField->text().toUTF8(); 
      float f; 
      sstr<<str; 
      sstr>>f;

      this->stateF->setText("Flow set:"+iField->text().toUTF8());
//       responseField->setText(responseField->text().toUTF8()+rfg->set_flow(f));
//     
      
    }
    
        void setI()
    {
//       
      
      stringstream sstr; 
      string str = iField->text().toUTF8(); 
      float f; 
      sstr<<str; 
      sstr>>f;

      this->stateF->setText("Flow set:"+iField->text().toUTF8());
//       responseField->setText(responseField->text().toUTF8()+rfg->set_flow(f));
//     
      
    }
    
    void getState()
    {
      
    }
    
    void start()
    {

    }
    
    void stop()
    {

    }
    
    
    
  };

  
  


 #endif 
  