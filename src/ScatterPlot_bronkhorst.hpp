#ifndef SCATTERPLOT_BRONKHORST_HPP
#define SCATTERPLOT_BRONKHORST_HPP

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
#include "device.hpp"
#include "measuredValue.hpp"
#include "measuredDBValue.hpp"
#include "jobQueue.hpp"
#include "bronkhorst.hpp"
#include "ScatterPlot.hpp"


using namespace lughos;

  template <> class ScatterPlot<bronkhorst> : public ScatterPlotWidget
  {
  protected:
    boost::shared_ptr<bronkhorst> horst;
    Wt::Chart::WCartesianChart *chart;
    boost::shared_ptr<Wt::Dbo::Session> session;
    Wt::Dbo::backend::Sqlite3 dbBackend;

  public:
    
    ScatterPlot< bronkhorst >(boost::shared_ptr<Device> horst) : horst(boost::dynamic_pointer_cast<bronkhorst>(horst)), session(new Wt::Dbo::Session), dbBackend("test.db")
    {

      this->init();
    }
        void init()
    {	
     this->session->setConnection(this->dbBackend);
      this->session->mapClass<measuredDBValue<double> >("measuredValue");
      this->name=horst->getName();
//      this->setWidth(500);
      this->addWidget(new Wt::WText(this->name.c_str()));
      this->chart = new Wt::Chart::WCartesianChart();
      this->chart->setBackground(Wt::WColor(220, 220, 220));
      
      Wt::Dbo::Transaction transaction(*this->session);
      Wt::Dbo::collection< Wt::Dbo::ptr<measuredDBValue<double> > > measuredValues = this->session->find<measuredDBValue<double> >(); //////
      
//       typedef boost::tuple<double, boost::posix_time::ptime> Item;
      typedef boost::tuple<double, Wt::WDateTime> Item;
      Wt::Dbo::QueryModel<Item> *model = new Wt::Dbo::QueryModel<Item>();
      
      std::cerr << "We have " << measuredValues.size() << " values in our database:" << std::endl;

//       for (auto i = measuredValues.begin(); i != measuredValues.end(); ++i)
//       std::cerr << " Value: " << (*i)->getvalue() << " " << (*i)->getunit() << " @ " << (*i)->getTimeStamp() << std::endl;
//       for (auto i = measuredValues.begin(); i != measuredValues.end(); ++i)
//       std::cout << " Value: " << (*i)->getvalue() << " " << (*i)->getunit() << " @ " << (*i)->getTimeStamp() << std::endl;
//   
      model->setQuery(this->session->query<Item>("SELECT value, timestamp FROM measuredValue").where("sensorName = ?").bind("Flow Controll 1").limit(100).orderBy("timestamp DESC"));
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
      boost::shared_ptr<Wt::WTimer> intervalTimer(new Wt::WTimer(this));
      intervalTimer->setInterval(5000);
      intervalTimer->timeout().connect(boost::bind(&Wt::Dbo::QueryModel<Item>::reload,model)); // Reload model every 3 seconds
      intervalTimer->start();
      this->addWidget(chart);
    }
  };
  
#endif