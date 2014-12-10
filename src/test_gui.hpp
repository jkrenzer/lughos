#ifndef TEST_GUI_HPP
#define TEST_GUI_HPP

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
#include "coolpak6000.hpp"
#include "MaxiGauge.hpp"
#include "kithleighSerial.hpp"

namespace lughos 
{

  using namespace Wt;
  using namespace std;
  extern boost::shared_ptr<boost::asio::io_service> ioService;
//   extern boost::asio::io_service * ioService;
  extern std::map<std::string, boost::shared_ptr<Device> > deviceMap;
  
    class ScatterPlotWidget : public Wt::WContainerWidget
  {
  public:
    std::string name;
    ScatterPlotWidget()
    {
      this->setStyleClass("ScatterPlotContainer");
    }
  };
  
    template <class S> class ScatterPlot : public ScatterPlotWidget
  {
  public:
    ScatterPlot<S>()
    {

      this->addWidget(new Wt::WText(this->name.c_str()));
      this->addWidget(new Wt::WText("No GUI for scatter plots availible!"));
    }
  };
  
  //------------------------
      template <> class ScatterPlot<kithleighSerial> : public ScatterPlotWidget
  {
  protected:
    boost::shared_ptr<kithleighSerial> kithleigh;
          Wt::Chart::WCartesianChart *chart;
	  boost::shared_ptr<dbo::Session> session;
	  dbo::backend::Sqlite3 dbBackend;
	  

  public:
    
    ScatterPlot< kithleighSerial >(boost::shared_ptr<Device> kithleigh) : kithleigh(boost::dynamic_pointer_cast<kithleighSerial>(kithleigh)), session(new dbo::Session), dbBackend("test.db")
    {

      this->init();
    }
    
    ~ScatterPlot< kithleighSerial >()
    {
      
    }
    
    
    void init()
    {	
      this->session->setConnection(this->dbBackend);
      this->session->mapClass<measuredDBValue>("measuredValue");
      this->name=kithleigh->getName();
//      this->setWidth(500);
      this->addWidget(new Wt::WText(this->name.c_str()));
           this->name=kithleigh->getName();
      this->chart = new Wt::Chart::WCartesianChart();
      this->chart->setBackground(Wt::WColor(220, 220, 220));
      
      dbo::Transaction transaction(*this->session);
      dbo::collection< dbo::ptr<measuredDBValue> > measuredValues = this->session->find<measuredDBValue>(); //////
      
//       typedef boost::tuple<double, boost::posix_time::ptime> Item;
      typedef boost::tuple<double, Wt::WDateTime> Item;
      dbo::QueryModel<Item> *model = new dbo::QueryModel<Item>();
      
      std::cerr << "We have " << measuredValues.size() << " values in our database:" << std::endl;

//       for (auto i = measuredValues.begin(); i != measuredValues.end(); ++i)
//       std::cerr << " Value: " << (*i)->getvalue() << " " << (*i)->getunit() << " @ " << (*i)->gettimestamp() << std::endl;
//       for (auto i = measuredValues.begin(); i != measuredValues.end(); ++i)
//       std::cout << " Value: " << (*i)->getvalue() << " " << (*i)->getunit() << " @ " << (*i)->gettimestamp() << std::endl;
//   
      model->setQuery(this->session->query<Item>("SELECT value, timestamp FROM measuredValue").where("sensorName = ?").bind("Temperature Monitor 1").limit(100).orderBy("timestamp DESC"));
      model->addColumn("value");
      model->addColumn("timestamp");
      transaction.commit();
	
//       	WTableView *view = new WTableView();
// 	view->resize(800, 400);
// 	view->setModel(model);
// 	view->setAlternatingRowColors(true);
// 	this->addWidget(view);
//       model->setHeaderData(0, Wt::WString("X"));
//       model->setHeaderData(1, Wt::WString("Y = sin(X)"));
//       for (unsigned i = 0; i < 40; ++i) 
// 	{
// 	    double x = (static_cast<double>(i) - 20) / 4;
// 
// 	    model->setData(i, 0, x);
// 	    model->setData(i, 1, std::sin(x));
// 	}

	
      chart->setModel(model);
      this->chart->setXSeriesColumn(1);
      this->chart->setLegendEnabled(true);
      this-> chart->setType(Wt::Chart::ScatterPlot);
      this->chart->axis(Wt::Chart::XAxis).setScale(Wt::Chart::DateTimeScale);
      chart->setPlotAreaPadding(100, Wt::Left | Wt::Top | Wt::Bottom | Wt::Right);
      
//       Add the curves
      Wt::Chart::WDataSeries s(0, Wt::Chart::LineSeries);
      s.setShadow(Wt::WShadow(3, 3, Wt::WColor(0, 0, 0, 127), 3));
      chart->addSeries(s);
      chart->resize(1024, 800);
      chart->setMargin(Wt::WLength::Auto, Wt::Left | Wt::Right);
//       chart->axis(Wt::Chart::XAxis).setMinimum(Wt::WDateTime::currentDateTime().addSecs(-120));
//       chart->axis(Wt::Chart::YAxis).setAutoLimits(Wt::Chart::MinimumValue | Wt::Chart::MaximumValue);
      Wt::WTimer *intervalTimer = new Wt::WTimer(this);
      intervalTimer->setInterval(5000);
      intervalTimer->timeout().connect(boost::bind(&Wt::Dbo::QueryModel<Item>::reload,model)); // Reload model every 3 seconds
      intervalTimer->start();
      this->addWidget(chart);
    }
    
  };
  
  

  
  //------------------------
  
    template <> class ScatterPlot<MaxiGauge> : public ScatterPlotWidget
  {
  protected:
    boost::shared_ptr<MaxiGauge> maxigauge;
    Wt::Chart::WCartesianChart *chart;
    boost::shared_ptr<dbo::Session> session;
    dbo::backend::Sqlite3 dbBackend;

  public:
    
    ScatterPlot< MaxiGauge >(boost::shared_ptr<Device> maxigauge) : maxigauge(boost::dynamic_pointer_cast<MaxiGauge>(maxigauge)), session(new dbo::Session), dbBackend("test.db")
    {

      this->init();
    }
        void init()
    {	
     this->session->setConnection(this->dbBackend);
      this->session->mapClass<measuredDBValue>("measuredValue");
      this->name=maxigauge->getName();
//      this->setWidth(500);
      this->addWidget(new Wt::WText(this->name.c_str()));
      this->chart = new Wt::Chart::WCartesianChart();
      this->chart->setBackground(Wt::WColor(220, 220, 220));
      
      dbo::Transaction transaction(*this->session);
      dbo::collection< dbo::ptr<measuredDBValue> > measuredValues = this->session->find<measuredDBValue>(); //////
      
//       typedef boost::tuple<double, boost::posix_time::ptime> Item;
      typedef boost::tuple<double, Wt::WDateTime> Item;
      dbo::QueryModel<Item> *model = new dbo::QueryModel<Item>();
      
      std::cerr << "We have " << measuredValues.size() << " values in our database:" << std::endl;

//       for (auto i = measuredValues.begin(); i != measuredValues.end(); ++i)
//       std::cerr << " Value: " << (*i)->getvalue() << " " << (*i)->getunit() << " @ " << (*i)->gettimestamp() << std::endl;
//       for (auto i = measuredValues.begin(); i != measuredValues.end(); ++i)
//       std::cout << " Value: " << (*i)->getvalue() << " " << (*i)->getunit() << " @ " << (*i)->gettimestamp() << std::endl;
//   
      model->setQuery(this->session->query<Item>("SELECT value, timestamp FROM measuredValue").where("sensorName = ?").bind("Pressure Monitor 1").limit(100).orderBy("timestamp DESC"));
      model->addColumn("value");
      model->addColumn("timestamp");
      transaction.commit();
	
//       	WTableView *view = new WTableView();
// 	view->resize(800, 400);
// 	view->setModel(model);
// 	view->setAlternatingRowColors(true);
// 	this->addWidget(view);
//       model->setHeaderData(0, Wt::WString("X"));
//       model->setHeaderData(1, Wt::WString("Y = sin(X)"));
//       for (unsigned i = 0; i < 40; ++i) 
// 	{
// 	    double x = (static_cast<double>(i) - 20) / 4;
// 
// 	    model->setData(i, 0, x);
// 	    model->setData(i, 1, std::sin(x));
// 	}

	
      chart->setModel(model);
      this->chart->setXSeriesColumn(1);
      this->chart->setLegendEnabled(true);
      this-> chart->setType(Wt::Chart::ScatterPlot);
      this->chart->axis(Wt::Chart::XAxis).setScale(Wt::Chart::DateTimeScale);
      chart->setPlotAreaPadding(100, Wt::Left | Wt::Top | Wt::Bottom | Wt::Right);
      
//       Add the curves
      Wt::Chart::WDataSeries s(0, Wt::Chart::LineSeries);
      s.setShadow(Wt::WShadow(3, 3, Wt::WColor(0, 0, 0, 127), 3));
      chart->addSeries(s);
      chart->resize(1024, 800);
      chart->setMargin(Wt::WLength::Auto, Wt::Left | Wt::Right);
//       chart->axis(Wt::Chart::XAxis).setMinimum(Wt::WDateTime::currentDateTime().addSecs(-120));
//       chart->axis(Wt::Chart::YAxis).setAutoLimits(Wt::Chart::MinimumValue | Wt::Chart::MaximumValue);
      Wt::WTimer *intervalTimer = new Wt::WTimer(this);
      intervalTimer->setInterval(5000);
      intervalTimer->timeout().connect(boost::bind(&Wt::Dbo::QueryModel<Item>::reload,model)); // Reload model every 3 seconds
      intervalTimer->start();
      this->addWidget(chart);
    }
  };
  
  
    class ScatterPlotView : public Wt::WContainerWidget
  {
  public:
    ScatterPlotView(WContainerWidget* parent = 0)
    {
//       this->addWidget(new ScatterPlot<S>());
      this->addWidget(new ScatterPlot<MaxiGauge>(deviceMap[std::string("Pressure Monitor 1")] ));  
      this->addWidget(new ScatterPlot<kithleighSerial>(deviceMap[std::string("Temperature Monitor 1")] )); 
    }

  };
  
  
  class DeviceUIInterface : public Wt::WContainerWidget
  {
  public:
    std::string name;
    DeviceUIInterface()
    {
      this->setStyleClass("DeviceContainer");
    }
  };
  
    
  template <class D> class DeviceUI : public DeviceUIInterface
  {
  public:
    DeviceUI<D>()
    {

      this->addWidget(new Wt::WText(this->name.c_str()));
      this->addWidget(new Wt::WText("No GUI for device availible!"));
    }
  };
  
  
  
  
  template <> class DeviceUI<coolpak6000> : public DeviceUIInterface
  {
  protected:
    boost::shared_ptr<coolpak6000> coolpak;
    Wt::WLineEdit* stateF;
    Wt::WLabel* stateL;
    Wt::WPushButton * startB;
    Wt::WPushButton * stateB;
    Wt::WPushButton * stopB;
    
  public:
    
    DeviceUI< coolpak6000 >(boost::shared_ptr<Device> coolpak) : coolpak(boost::dynamic_pointer_cast<coolpak6000>(coolpak))
    {

      this->init();
    }
    
    DeviceUI<coolpak6000>(boost::shared_ptr<coolpak6000> coolpak) : coolpak(coolpak)
    {
      this->init();
    }
    
    void checkConnected()
    {
      if(coolpak->isConnected())
      {
	this->stateF->setText("Connected!");
        this->stateB->setText("Status");
	this->startB->setDisabled(false);
	this->stopB->setDisabled(false);
	this->startB->clicked().connect(this,&DeviceUI<coolpak6000>::start);
	this->stopB->clicked().connect(this,&DeviceUI<coolpak6000>::stop);
        this->stateB->clicked().connect(this,&DeviceUI<coolpak6000>::showData);
	this->getState();

      }
      else
      {

	this->stateF->setText("Not connected!");
// 	this->stateF->setText(std::to_string(coolpak->isConnected()));
        this->stateB->setText("Try again");
	this->stateB->clicked().connect(this,&DeviceUI<coolpak6000>::checkConnected);
	this->startB->setDisabled(true);
	this->stopB->setDisabled(true);

      }
    }
    
    void init()
    {
     this->name=coolpak->getName();
//      this->setWidth(500);
      this->addWidget(new Wt::WText(this->name.c_str()));
      this->stateF = new Wt::WLineEdit("Initializing...");
      this->stateF->setReadOnly(true);
      this->stateL = new Wt::WLabel("Status:");
      this->stateL->setBuddy(stateF);
      this->startB = new Wt::WPushButton("Start");
      this->stopB = new Wt::WPushButton("Stop");
      this->stateB = new Wt::WPushButton("Status");
     this->addWidget(stateL);
     this->addWidget(stateF);
     this->addWidget(startB);
     this->addWidget(stopB);
     this->addWidget(stateB);
     this->checkConnected();

    }
    
    void showData()
    {
      this->stateF->setText(coolpak->get_data());
    }
    
    void getState()
    {
      coolpak->get_data();
      std::string state;
      bool communicationEstablished = false;
      switch(coolpak->get_compressor_state())
      {
	case 0:
	  state += std::string("Compressor OFF ");
	  communicationEstablished = true;
	  break;
	case 1:
	  state += std::string("Compressor ON ");
	  communicationEstablished = true;
	  break;
	case 2:
	  state += std::string("Compressor ERROR ");
	  communicationEstablished = true;
	  break;
	default:
	  state += std::string("Compressor ???? ");
	 break;
      }
      state += std::string(",");
      switch(coolpak->get_coolhead1_state())
      {
	case 0:
	  state += std::string("Coldhead1 OFF ");
	  communicationEstablished = true;
	  break;
	case 1:
	  state += std::string("Coldhead1 ON ");
	  communicationEstablished = true;
	  break;
	case 2:
	  state += std::string("Coldhead1 ERROR ");
	  communicationEstablished = true;
	  break;
	default:
	  state += std::string("Coldhead1 ???? ");
	 break;
      } 
      if(communicationEstablished)
      {
	this->stateF->setText(state);
	this->startB->setDisabled(false);
	this->stopB->setDisabled(false);
      }
    }
    
    void start()
    {
      this->stateF->setText("Starting...");
      if(this->coolpak->compressor_on())
	this->stateF->setText("System on");
      else
	this->stateF->setText("Cannot start!");
      this->getState();
    }
    
    void stop()
    {
      this->stateF->setText("Stopping...");
      if(this->coolpak->compressor_off())
	this->stateF->setText("System off");
      else
	this->stateF->setText("Cannot stop!");
      this->getState();
    }
    
    
    
  };

  
  



  class OverView : public Wt::WContainerWidget
  {
  public:
    
    OverView(WContainerWidget* parent = 0)
    {
      
    }
    
  };
  
  class PressureView : public Wt::WContainerWidget
  {
  public:
    
    PressureView(WContainerWidget* parent = 0)
    {

    }
    
  };
  
  class TemperatureView : public Wt::WContainerWidget
  {
  public:
    
    TemperatureView(WContainerWidget* parent = 0)
    {
      
    }
    
  };
  
  
  template <> class DeviceUI<MaxiGauge> : public DeviceUIInterface
  {
   protected:
    boost::shared_ptr<MaxiGauge> maxigauge;
    Wt::WLineEdit* stateF;
    Wt::WLabel* stateL;
    Wt::WLabel* sensorOnL;
    Wt::WLabel* sensorOffL;
    Wt::WPushButton * startB;
    Wt::WPushButton * onB[6];
    Wt::WPushButton * offB[6];
    Wt::WPushButton * stopB;
    Wt::WPushButton * stateB;
    
  public:
    
    DeviceUI< MaxiGauge >(boost::shared_ptr<Device> maxigauge) : maxigauge(boost::dynamic_pointer_cast<MaxiGauge>(maxigauge))
    {
    std::cout<< maxigauge.get()<<std::endl;

      this->init();
    }
    
    DeviceUI<MaxiGauge>(boost::shared_ptr<MaxiGauge> maxigauge) : maxigauge(maxigauge)
    {
      this->init();
    }
    
    
    void checkConnected()
    {
      if(maxigauge->isConnected())
      {
	this->stateF->setText("Connected!");
        this->stateB->setText("Status");
	this->startB->setDisabled(false);
	this->stopB->setDisabled(false);
	this->startB->clicked().connect(this,&DeviceUI<MaxiGauge>::startall);
	this->stopB->clicked().connect(this,&DeviceUI<MaxiGauge>::stopall);
        this->stateB->clicked().connect(this,&DeviceUI<MaxiGauge>::showData);



	this->getState();
      }
      else
      {
	for(int i=0;i<6;i++)
	{
	  this->onB[i]->setDisabled(true);
	  this->offB[i]->setDisabled(true);
	}

	this->stateF->setText("Not connected!");
// 	this->stateF->setText(std::to_string(maxigauge->isConnected()));
        this->stateB->setText("Try again");
	this->stateB->clicked().connect(this,&DeviceUI<MaxiGauge>::checkConnected);
	this->startB->setDisabled(true);
	this->stopB->setDisabled(true);

      }
    }
    
    void init()
    {
     this->name=maxigauge->getName();
//      this->setWidth(500);
      this->addWidget(new Wt::WText(this->name.c_str()));
      this->stateF = new Wt::WLineEdit("Initializing...");
      this->stateF->setReadOnly(true);
      this->stateL = new Wt::WLabel("Status:");
      this->stateL->setBuddy(stateF);
      this->sensorOnL = new Wt::WLabel("Sensor on: ");
      this->sensorOffL = new Wt::WLabel("Sensor off:");
      this->startB = new Wt::WPushButton("Start");
      this->stopB = new Wt::WPushButton("Stop");
      this->stateB = new Wt::WPushButton("Status");
     this->addWidget(stateL);
     this->addWidget(stateF);
     this->addWidget(startB);
     this->addWidget(stopB);
     this->addWidget(stateB);
     
     Wt::WContainerWidget* onButtonContainer= new Wt::WContainerWidget;
     this->addWidget(onButtonContainer);
      Wt::WContainerWidget* offButtonContainer= new Wt::WContainerWidget;
     this->addWidget(offButtonContainer);
     onButtonContainer->addWidget(sensorOnL);
         for(int i=0;i<6;i++)
      {
	onB[i]=new Wt::WPushButton(std::to_string(i+1));
	this->onB[i]->clicked().connect(boost::bind(&DeviceUI<MaxiGauge>::sensor_on,this, i));
	onButtonContainer->addWidget(onB[i]);
      }
     offButtonContainer->addWidget(sensorOffL);
      for(int i=0;i<6;i++)
      {
	offB[i]=new Wt::WPushButton(std::to_string(i+1));
	this->offB[i]->clicked().connect(boost::bind(&DeviceUI<MaxiGauge>::sensor_off,this, i));
	this->addWidget(offB[i]);
	offButtonContainer->addWidget(offB[i]);
      }
     this->checkConnected();
    }
    
    void showData()
    {
      this->getState();
//       this->stateF->setText("");
    }
    
    void getState()
    {
      std::string state;
      std::string enabled="Enabled channels: ";
      std::string disabled="disabled channels: ";

      bool communicationEstablished = false;
      for(int i=0;i<6;i++)
      {
	
	if(!maxigauge->get_status(i).empty())
	{
	 enabled+=std::to_string(i); 
	
	 communicationEstablished = true;
	}
	else
	{
	 disabled+=std::to_string(i); 
	 disabled+=std::string(" ");
	 communicationEstablished = true;
	}
	
      }
      state=enabled+disabled;
     
      if(communicationEstablished)
      {
	this->stateF->setText(state);
	this->startB->setDisabled(false);
	this->stopB->setDisabled(false);
      }
    }
    
    void startall()
    {
      
      for(int i=0;i<6;i++)
      {
	maxigauge->sensor_on(i);
	onB[i]->setDisabled(true);
	offB[i]->setDisabled(false);
      }

      
      this->getState();
    }
    
    void stopall()
    {
      for(int i=0;i<6;i++)
      {
	maxigauge->sensor_off(i);
	onB[i]->setDisabled(false);
	offB[i]->setDisabled(true);
      }
      this->getState();
    }
    
        void sensor_on(int i)
    {
      if(maxigauge->sensor_on(i))
      {
	this->stateF->setText("Sensor "+ std::to_string(i)+" enabled");
	onB[i]->setDisabled(true);
	offB[i]->setDisabled(false);
      }
    }
        
     void sensor_off(int i)
    {
      if(maxigauge->sensor_off(i))
      {
	this->stateF->setText("Sensor "+ std::to_string(i)+" disabled");
	onB[i]->setDisabled(false);
	offB[i]->setDisabled(true);
      }
    }
    
    
  };
  
  //-----------------------------------
    template <> class DeviceUI<kithleighSerial> : public DeviceUIInterface
  {
  protected:
  Wt::WTextArea *dialogField;
  Wt::WTextArea *responseField;
  boost::shared_ptr<kithleighSerial> keithley;
  Wt::WLineEdit* stateF;
  Wt::WLabel* stateL;
  Wt::WPushButton * dialogB;
  Wt::WPushButton * stateB;
//     Wt::WPushButton * stopB;
    
  public:
    
    DeviceUI< kithleighSerial >(boost::shared_ptr<Device> keithley) : keithley(boost::dynamic_pointer_cast<kithleighSerial>(keithley))
    {
      
      this->init();
    }
    
    DeviceUI<kithleighSerial>(boost::shared_ptr<kithleighSerial> keithley) : keithley(keithley)
    {
      this->init();
    }
    
    void checkConnected()
    {
      if(keithley->isConnected())
      {
	this->stateF->setText("Connected!");
        this->stateB->setText("Status");
	this->dialogField->setDisabled(false);
	
// 	this->stopB->setDisabled(false);
	this->dialogB->setDisabled(false);
// 	this->startB->clicked().connect(this,&DeviceUI<kithleighSerial>::start);
// 	this->stopB->clicked().connect(this,&DeviceUI<kithleighSerial>::stop);
        this->stateB->clicked().connect(this,&DeviceUI<kithleighSerial>::showData);
	this->dialogB->clicked().connect(this,&DeviceUI<kithleighSerial>::startDialog);


// 	Wt::WText *out = new Wt::WText("<p></p>", dialogField);

	this->getState();
	

      }
      else
      {

	this->stateF->setText("Not connected!");
// 	this->stateF->setText(std::to_string(coolpak->isConnected()));
        this->stateB->setText("Try again");
	this->stateB->clicked().connect(this,&DeviceUI<kithleighSerial>::checkConnected);
// 	this->startB->setDisabled(true);
// 	this->stopB->setDisabled(true);
// 	this->dialogB->setDisabled(true);

      }
    }
    
    void init()
    {
     this->name=keithley->getName();
//      this->setWidth(500);
      this->addWidget(new Wt::WText(this->name.c_str()));
      this->stateF = new Wt::WLineEdit("Initializing...");
      this->stateF->setReadOnly(true);
      this->stateL = new Wt::WLabel("Status:");
      this->stateL->setBuddy(stateF);
      this->dialogField =  new Wt::WTextArea();
      this->responseField =  new Wt::WTextArea();
//       this->stopB = new Wt::WPushButton("Stop");
      this->stateB = new Wt::WPushButton("Status");
       this->dialogB = new Wt::WPushButton("Send");
      this->responseField->setReadOnly(true); 
     this->addWidget(stateL);
     this->addWidget(stateF);
//      this->addWidget(startB);
     this->addWidget(dialogField);
     this->addWidget(responseField);
     this->addWidget(stateB);
     
//      	Wt::WTextArea *ta = new Wt::WTextArea(dialogField);
// 	ta->setColumns(80);
// 	ta->setRows(5);
// 	ta->setText("");
     this->addWidget(dialogB);
     this->checkConnected();
    }
    
    void showData()
    {
      
          fstream f;
	  f.open("~/projects/test.dat", ios::out);
	  f << keithley->inputOutput("*IDN?") << endl;
	   f.close();
      
      this->stateF->setText(keithley->inputOutput("*IDN?"));
    }
    
    void getState()
    {
      std::string state;
      state=keithley->inputOutput("*IDN?");
      std::cout<<"state "<<state<<std::endl;
      bool communicationEstablished = false;

      if(communicationEstablished)
      {
	this->stateF->setText(state);
// 	this->startB->setDisabled(false);
// 	this->stopB->setDisabled(false);
      }
    }
    
    void startDialog()
    {
      std::string sendText = dialogField->text().toUTF8();
	std::istringstream iss(sendText);
	std::string token;
	
	while(getline(iss, token, '\n'))
	{
	  
// 	  std::cout<<keithley->inputOutput(token)<<std::endl;
	  if (token.find_last_of("?")) responseField->setText(responseField->text().toUTF8()+keithley->inputOutput(token));   
	  else keithley->input(token);  
	  std::cout << token << std::endl;
	}
        responseField->setText(responseField->text().toUTF8()+std::string("\n--------------------------\n")); 
// 	dialogField->setText("");
    }
    
    
    void start()
    {
      this->stateF->setText("Starting...");
      this->getState();
    }
    
    void stop()
    {
      this->stateF->setText("Stopping...");
      this->getState();
    }
    
  };

  //------------------------------------
  
  
  
  class DeviceView : public Wt::WContainerWidget
  {
  public:
    DeviceView(WContainerWidget* parent = 0)
    {
      this->addWidget(new DeviceUI<coolpak6000>(deviceMap[std::string("Compressor 1")] ));
      this->addWidget(new DeviceUI<coolpak6000>(deviceMap[std::string("Compressor 2")] ));
      this->addWidget(new DeviceUI<MaxiGauge>(deviceMap[std::string("Pressure Monitor 1")] ));  
      this->addWidget(new DeviceUI<kithleighSerial>(deviceMap[std::string("Temperature Monitor 1")] )); 
    }

  };
  
  
  class mainApplication : public WApplication
  {
  public:
    
    
    
    mainApplication(const WEnvironment &env) : WApplication(env)
    {
      


      this->useStyleSheet("resources/lughos.css");
      Wt::WBootstrapTheme *bootstrapTheme = new Wt::WBootstrapTheme(this);
      bootstrapTheme->setVersion(Wt::WBootstrapTheme::Version3);
      bootstrapTheme->setResponsive(true);
      this->setTheme(bootstrapTheme);
      // load the default bootstrap3 (sub-)theme
      this->useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
      setTitle("Lughos System Control");
// 		    ofs.close();
      Wt::WContainerWidget *container = new Wt::WContainerWidget();
      Wt::WContainerWidget *headContainer = new Wt::WContainerWidget();
      Wt::WImage *logo = new WImage("resources/logo.png");
      Wt::WImage *branding = new WImage("resources/branding.png");
      Wt::WText *headText = new WText("<h1>Lughos System Control</h1>");
      logo->setHeight(100);
      branding->setHeight(100);
      Wt::WHBoxLayout *headBox = new Wt::WHBoxLayout();
      headContainer->setLayout(headBox);
      headBox->addWidget(logo);
      headBox->addWidget(headText,1);
      headBox->addWidget(branding);
// 		    ofs.close();
      
      
      Wt::WVBoxLayout *vbox = new Wt::WVBoxLayout();
      container->setLayout(vbox);
      vbox->addWidget(headContainer);
// 		    ofs.close();
      Wt::WTabWidget *tabW = new Wt::WTabWidget(container);
      tabW->addTab(new Wt::WTextArea("This is the contents of the first tab."),
		  "First", Wt::WTabWidget::PreLoading);
      tabW->addTab(new Wt::WTextArea("The contents of the tabs are pre-loaded in"
				    " the browser to ensure swift switching."),
		  "Preload", Wt::WTabWidget::PreLoading);
// 		    ofs.close();
      tabW->addTab(new DeviceView(), "Devices", Wt::WTabWidget::PreLoading)->setStyleClass("thread");
      tabW->addTab(new ScatterPlotView(), "ScatterPlots", Wt::WTabWidget::PreLoading)->setStyleClass("thread");
//       Wt::WMenuItem *tab 
// 	  = tabW->addTab(new Wt::WTextArea("You can close this tab"
// 					  " by clicking on the close icon."),
// 			"Close");
//       tab->setCloseable(true);
      vbox->addWidget(tabW);
      tabW->setStyleClass("tabwidget");
      root()->addWidget(container);


    }
    
  
  };
  
  
} //namespace lughos

#endif