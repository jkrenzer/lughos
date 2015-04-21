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
  template <> class DeviceUI<Relais> : public DeviceUITemplate<Relais>
  {
  protected:
    Wt::WRadioButton *rbOn[8];
    Wt::WRadioButton *rbOff[8];
    Wt::WButtonGroup *group[8];
    Wt::WLineEdit* flowF;

    Wt::WTextArea *responseField;
    Wt::WPushButton * changeChannelsB;
    
  public:
    
    DeviceUI< Relais >(boost::shared_ptr<Device> relais) : DeviceUITemplate<Relais>(relais)
    {
            this->init();
    }
    
    void enable()
    {
      for (unsigned i = 0; i < 8; ++i) 
      {
	rbOn[i]->setDisabled(false);
	rbOff[i]->setDisabled(false);
      }
    }
    
    void disable()
    {
      for (unsigned i = 0; i < 8; ++i) 
      {
	rbOn[i]->setDisabled(true);
	rbOff[i]->setDisabled(true);
      }
    }
    
        
    void init()
    {
      
      
      this->changeChannelsB = new Wt::WPushButton("Set");
      this->changeChannelsB->clicked().connect(this,&DeviceUI<Relais>::changeChannels);
    
      
      Wt::WTable *table = new Wt::WTable();
      table->setHeaderCount(1);
      table->setWidth("100%");
      table->elementAt(0, 0)->addWidget(new Wt::WText("#"));
      table->elementAt(0, 1)->addWidget(new Wt::WText("On"));
      table->elementAt(0, 2)->addWidget(new Wt::WText("Off"));
      table->elementAt(0, 3)->addWidget(new Wt::WText(""));
      std::string names[8]={"Valve EV1","Valve EV2","Boost Unit","Ignition Pulse","Not used","Not used","Not used","Not used"};
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
    }
    

    
    void getChannels()
    {
	std::string ss = this->device()->relais.getValueAsString();
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

//       this->stateF->setText(ss);
    }
    
    
    
        void changeChannels()
    {
      measuredValue<double> v;
      std::string ss;
      std::string channelSequence="";
      
	for (unsigned i = 0; i < 8; ++i) 
	{
	  if(rbOn[i]->isChecked())channelSequence+=std::to_string(1);
	  else channelSequence+=std::to_string(0);
	}
	std::bitset<8> newstate(channelSequence);
	this->device()->relais.setValue(newstate);
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
  