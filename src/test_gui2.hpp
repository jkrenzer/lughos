#ifndef TEST_GUI2_HPP
#define TEST_GUI2_HPP

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WLabel>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WGroupBox>
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
#include <Wt/WComboBox>
#include <Wt/WDate>
#include <Wt/WDateTime>
#include <Wt/WLocalDateTime>
#include <Wt/WPaintedWidget>
#include <Wt/WItemDelegate>
#include <Wt/WPanel>
#include <Wt/WShadow>
#include <Wt/WStandardItemModel>
#include <Wt/WStackedWidget>
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
#include "RFG.hpp"
#include "FUGNetzteil.hpp"
#include "Relais.hpp"
#include "DeviceUI_bronkhorst.hpp"
#include "DeviceUI_RFG.hpp"
#include "DeviceUI_PSA.hpp"
#include "DeviceUI_FUG.hpp"
#include "DeviceUI_Relais.hpp"
#include "DeviceUI.hpp"
#include "ScatterPlot.hpp"
#include "ScatterPlot_bronkhorst.hpp"

// template <class T, class S> T save_lexical_cast(S& source, T saveDefault)
// {
//   try
//   {
//     return boost::lexical_cast<T>(source);
//   }
//   catch(boost::bad_lexical_cast e)
//   {
//     return saveDefault;
//   }
//   
// }
namespace lughos 
{

  using namespace Wt;
  using namespace std;
  extern boost::shared_ptr<boost::asio::io_service> ioService;
//   extern boost::asio::io_service * ioService;
  extern std::map<std::string, boost::shared_ptr<Device> > deviceMap;

  class ConnectionStatusWidget : public Wt::WPanel
  {
  public:
    
    Wt::WImage* heart1;
    Wt::WImage* heart2;
    Wt::WStackedWidget* heart;
    Wt::WText* dateT;
    Wt::WDateTime* date;
    Wt::WTimer* timer;
    WContainerWidget* container;
    bool state;
    
    ConnectionStatusWidget(WContainerWidget* parent = 0) : WPanel(parent)
    {
      state = false;
      this->heart1 = new Wt::WImage("./resources/heart1.png");
      this->heart2 = new Wt::WImage("./resources/heart2.png");
      this->heart = new Wt::WStackedWidget();
      this->container = new Wt::WContainerWidget();
      this->setTitle("Connection status");
      this->date = new Wt::WDateTime();
      this->timer = new Wt::WTimer(this);
      this->dateT = new Wt::WText("Initializing...");
      this->heart->addWidget(this->heart1);
      this->heart->addWidget(this->heart2);
      this->heart->addStyleClass("inlineDisplay");
      this->heart->setMargin(Wt::WLength(0.5,WLength::FontEm),Wt::Side::Right);
      this->container->addWidget(heart);
      this->container->addWidget(dateT);
      this->timer->setInterval(1000);
      this->timer->timeout().connect(boost::bind(&ConnectionStatusWidget::beat,this));
      this->timer->start();
      this->setCentralWidget(container);
      this->update();
    }
    
virtual ~ConnectionStatusWidget()
{
  this->timer->stop();
}
    
    void update()
    {
      this->heart->setCurrentIndex(this->state);
      *this->date = Wt::WDateTime::currentDateTime();
      this->dateT->setText(this->date->toString());
    }
    
    void beat()
    {
      this->state = !this->state;
      this->update();
    }
    
  };
  
  class ParserWidget : public Wt::WPanel
  {
  protected:
    Wt::WContainerWidget* container;
    Wt::WTextArea* history;
    Wt::WLineEdit* prompt;
    Wt::WLabel* languageL;
    Wt::WComboBox* language;
    Wt::WPushButton* sendB;
  public:
    
    ParserWidget()
    {
      this->setTitle("Command prompt");
      this->container = new Wt::WContainerWidget();
      this->history = new Wt::WTextArea();
      this->history->setDisabled(true);
      this->sendB = new Wt::WPushButton("Send");
      this->prompt = new Wt::WLineEdit();
      this->prompt->setPlaceholderText("Enter command...");
      this->languageL = new Wt::WLabel();
      this->language = new Wt::WComboBox();
      this->language->addItem("C-Script");
      this->languageL->setBuddy(language);
      
      this->container->addWidget(history);
      this->container->addWidget(languageL);
      this->container->addWidget(language);
      this->container->addWidget(new Wt::WBreak);
      this->container->addWidget(prompt);
      this->container->addWidget(sendB);
      this->setCentralWidget(this->container);
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
  

  class DeviceView : public Wt::WContainerWidget
  {
  public:
    DeviceView(WContainerWidget* parent = 0)
    {
      this->addStyleClass("DeviceView");
      this->addWidget(new DeviceUI<PSAPowersupply>(deviceMap[std::string("PSA 1")] ));
      this->addWidget(new DeviceUI<PSAPowersupply>(deviceMap[std::string("PSA 2")] ));
      this->addWidget(new DeviceUI<PSAPowersupply>(deviceMap[std::string("PSA 3")] ));
      this->addWidget(new DeviceUI<Relais>(deviceMap[std::string("Relais 1")] ));
      this->addWidget(new DeviceUI<bronkhorst>(deviceMap[std::string("Flow Controll 1")]));
      this->addWidget(new DeviceUI<bronkhorst>(deviceMap[std::string("Flow Controll 2")]));
      this->addWidget(new DeviceUI<RFG>(deviceMap[std::string("RFG 1")] ));   
      this->addWidget(new DeviceUI<FUGNetzteil>(deviceMap[std::string("FUGNetzteil 1")]));
      this->addWidget(new DeviceUI<FUGNetzteil>(deviceMap[std::string("FUGNetzteil 2")]));
    }

  };
  class ScatterPlotView : public Wt::WContainerWidget
  {
  public:
    ScatterPlotView(WContainerWidget* parent = 0)
    {
//       this->addWidget(new ScatterPlot<S>());
      this->addWidget(new ScatterPlot<bronkhorst>(deviceMap[std::string("Flow Controll 1")] )); 
      
//       this->addWidget(new ScatterPlot<RFG>(deviceMap[std::string("E")] )); 
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
      this->useStyleSheet("/usr/share/Wt/resources/themes/bootstrap/3/bootstrap-theme.min.css");
      setTitle("Lughos System Control");
// 		    ofs.close();
      Wt::WContainerWidget *container = new Wt::WContainerWidget();
      Wt::WContainerWidget *headContainer = new Wt::WContainerWidget();
      Wt::WImage *logo = new WImage("./resources/logo.png");
      Wt::WImage *branding = new WImage("./resources/branding.png");
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
      Wt::WContainerWidget *bodyContainer = new Wt::WContainerWidget();
      Wt::WHBoxLayout *hbox = new Wt::WHBoxLayout();
      bodyContainer->setLayout(hbox);
      
      vbox->addWidget(headContainer);
      vbox->addWidget(bodyContainer);
      
      Wt::WGroupBox *leftPanel = new Wt::WGroupBox("System Widgets");
      leftPanel->addWidget(new Wt::WBreak);
      leftPanel->addWidget(new ConnectionStatusWidget());
      leftPanel->addWidget(new ParserWidget());
      leftPanel->setWidth(Wt::WLength(15,WLength::Percentage));
      hbox->addWidget(leftPanel);
// 		    ofs.close();
      Wt::WTabWidget *tabW = new Wt::WTabWidget(container);
      tabW->addTab(new DeviceView(), "Devices", Wt::WTabWidget::PreLoading)->setStyleClass("thread");
      tabW->addTab(new ScatterPlotView(), "Plots", Wt::WTabWidget::PreLoading)->setStyleClass("thread");
//       Wt::WMenuItem *tab 
// 	  = tabW->addTab(new Wt::WTextArea("You can close this tab"
// 					  " by clicking on the close icon."),
// 			"Close");
//       tab->setCloseable(true);
      
      tabW->setStyleClass("tabwidget");
      hbox->addWidget(tabW);
      root()->addWidget(container);


    }
    
  
  };
  

  
} //namespace lughos

#endif