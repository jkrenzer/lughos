#ifndef DEVICEUI_RELAIS_HPP
#define DEVICEUI_RELAIS_HPP

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
#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/WString>
#include <Wt/WTable>
#include <Wt/WTableCell>
#include "measuredValue.hpp"
#include "measuredDBValue.hpp"
#include "jobQueue.hpp"
#include "Relais.hpp"
#include "DeviceUI.hpp"

namespace lughos
{
  template <> class DeviceUI<Relais> : public DeviceUIInterface
  {
  protected:
    boost::shared_ptr<Relais> relais;
    Wt::WRadioButton *rbOn[8];
    Wt::WRadioButton *rbOff[8];
    Wt::WButtonGroup *group[8];
    Wt::WLineEdit* stateF;
    Wt::WLineEdit* flowF;
    Wt::WLabel* stateL;

    Wt::WTextArea *responseField;
    Wt::WPushButton * changeChannelsB;
    Wt::WPushButton * stateB;
    Wt::WPushButton * channelStateB;
    
  public:
    
    DeviceUI< Relais >(boost::shared_ptr<Device> relais) : relais(boost::dynamic_pointer_cast<Relais>(relais))
    {
            this->init();
    }
    
    DeviceUI<Relais>(boost::shared_ptr<Relais> relais) : relais(relais)
    {
      this->init();
    }
    
    void checkConnected()
    {
      if(relais->isConnected())
      {
	this->stateF->setText("Connected!");
        this->stateB->setText("Status");
	
        this->changeChannelsB->clicked().connect(this,&DeviceUI<Relais>::changeChannels);
        this->channelStateB->clicked().connect(this,&DeviceUI<Relais>::getState);
	this->getState();

      }
      else
      {

	this->stateF->setText("Not connected!");
// 	this->stateF->setText(std::to_string(coolpak->isConnected()));
        this->stateB->setText("Try again");
	this->stateB->clicked().connect(this,&DeviceUI<Relais>::checkConnected);
	std::cout<<"not f*cking connected!"<<std::endl;
	this->changeChannelsB->setDisabled(true);
	this->channelStateB->setDisabled(true);
	
	    for (unsigned i = 0; i < 8; ++i) 
	    {
	      rbOn[i]->setDisabled(true);
	      rbOff[i]->setDisabled(true);
	    }
// 	this->sendUB->setDisabled(true);
// 	this->uMinField->setDisabled(true);
// 	this->uMaxField->setDisabled(true);

      }
    }
    
    void init()
    {
      
     this->name=relais->getName();
//      this->setWidth(500);
      this->setTitle(Wt::WString::fromUTF8(this->name.c_str()));
      this->stateF = new Wt::WLineEdit("Initializing...");
      this->stateF->setReadOnly(true);
      this->changeChannelsB = new Wt::WPushButton("Send");
      this->channelStateB = new Wt::WPushButton("Get states");
      this->stateL = new Wt::WLabel("Status:");
      this->stateL->setBuddy(stateF);
      
      this->addWidget(stateL);
      this->addWidget(stateF); 


      this->stateB = new Wt::WPushButton("Status");
      this->addWidget(stateB);
     
      
      Wt::WTable *table = new Wt::WTable();
      table->setHeaderCount(1);
      table->setWidth("100%");
      table->elementAt(0, 0)->addWidget(new Wt::WText("#"));
      table->elementAt(0, 1)->addWidget(new Wt::WText("On"));
      table->elementAt(0, 2)->addWidget(new Wt::WText("Off"));
      table->elementAt(0, 3)->addWidget(new Wt::WText(""));
      std::string names[8]={"I am your father!","","","","","","",""};
//        
    for (unsigned i = 0; i < 8; ++i) {
    group[i] = new Wt::WButtonGroup();
    int row = i + 1;

      new Wt::WText(std::to_string(i+1), table->elementAt(row, 0));
      rbOn[i]=new Wt::WRadioButton("", table->elementAt(row, 1));
//       button=*rbOn[i];
      rbOff[i]=new Wt::WRadioButton("", table->elementAt(row, 2));
       new Wt::WText(names[i], table->elementAt(row, 3));
//       rbOn[i]->setInline(false);
//       rbOff[i]->setInline(false);
      group[i]->addButton(rbOn[i]);
      group[i]->addButton(rbOff[i]); 
      group[i]->setSelectedButtonIndex(1);
      }
//       
      this->addWidget(table);
      this->addWidget(changeChannelsB);
      this->addWidget(channelStateB);


      this->checkConnected();

    }
    

    
    void getState()
    {
	std::string ss = this->relais->read_channels();
	std::bitset<8> relais(ss);
      

	for (int i = 0; i < 8; i++)
	{
	  if (relais[i])
	  {
	    this->rbOn[i]->setChecked();
	  }
	  else 
	  {
	    this->rbOff[i]->setChecked();
	  }
	}

      this->stateF->setText(ss);
    }
    
        void changeChannels()
    {
      measuredValue v;
      std::string ss;
      std::string channelSequence="";
      
	for (unsigned i = 0; i < 8; ++i) 
	{
	  if(rbOn[i]->isChecked())channelSequence+=std::to_string(1);
	  else channelSequence+=std::to_string(0);
	}
	ss = this->relais->write_channels(channelSequence);
// 	ss << "Channel " << i << ": " << v.getStringValue() << v.getunit() << std::endl;

	this->getState();
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

} //namespace
  


 #endif 
  