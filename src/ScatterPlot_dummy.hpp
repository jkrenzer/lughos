/********************************************************************************
 *   This file is part of LUGHOS.						*
 *										*
 *										*
 *  LUGHOS is free software: you can redistribute it and/or modify		*
 *  it under the terms of the GNU General Public License as published by	*
 *  the Free Software Foundation, either version 3 of the License, or		*
 *  (at your option) any later version.						*
 *										*
 *  LUGHOS is distributed in the hope that it will be useful,			*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of		*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*
 *  GNU General Public License for more details.				*
 *										*
 *  We sincerily invite everybody to participate in LUGHO'S development. 	*
 *  Together we can achieve more.						*
 *										*
 *  (c) 2014, 2015 Jörn Mankiewicz (joern.mankiewicz@gmx.de), 			*
 *                 Irina Brodski (irina@brodski.de)				*
 *										*
 ********************************************************************************/

#ifndef SCATTERPLOT_DUMMY_HPP
#define SCATTERPLOT_DUMMY_HPP

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
#include "ScatterPlot.hpp"


namespace lughos
{

  class ScatterPlotDummy : public ScatterPlotWidget
  {
  protected:
   
    Wt::WContainerWidget *container;
    Wt::WStandardItemModel *model;

  public:
    
     Wt::Chart::WCartesianChart *chart;
    
    ScatterPlotDummy()
    {
      this->init();
    }
    void init()
    {	
//       this->setWidth(Wt::WLength(60,Wt::WLength::Percentage));
      this->chart = new Wt::Chart::WCartesianChart();
      this->chart->setBackground(Wt::WColor(230, 230, 230));
      this->container = new Wt::WContainerWidget();
      this->model = new Wt::WStandardItemModel(40, 6, container);
      model->setHeaderData(0, Wt::WString("Time"));
      model->setHeaderData(1, Wt::WString("V1"));
      model->setHeaderData(2, Wt::WString("V2"));
      model->setHeaderData(3, Wt::WString("V3"));
      model->setHeaderData(4, Wt::WString("V4"));
      model->setHeaderData(5, Wt::WString("V5"));
      
      Wt::WDateTime dateTime = Wt::WDateTime::fromString("02/11/14");
      
      for (unsigned i = 0; i < 40; ++i) 
      {
	model->setData(i, 0, dateTime.addSecs(i*60));
	model->setData(i, 1, 1);
	model->setData(i, 2, 2);
	model->setData(i, 3, 3);
	model->setData(i, 4, 4);
	model->setData(i, 5, 5);
      }

	
      chart->setModel(this->model);
      this->chart->setXSeriesColumn(0);
      this->chart->setLegendEnabled(true);
      this-> chart->setType(Wt::Chart::ScatterPlot);
      this->chart->axis(Wt::Chart::XAxis).setScale(Wt::Chart::DateTimeScale);
//       chart->axis(Wt::Chart::XAxis).setLocation(Wt::Chart::ZeroValue);
//       chart->axis(Wt::Chart::YAxis).setLocation(Wt::Chart::ZeroValue);
//       chart->setPlotAreaPadding(80, Wt::Left);
      chart->setPlotAreaPadding(40, Wt::Left | Wt::Right);
      
//       Add the curves
      Wt::Chart::WDataSeries s1(1, Wt::Chart::LineSeries);
      s1.setShadow(Wt::WShadow(3, 3, Wt::WColor(0, 0, 0, 127), 3));
      Wt::Chart::WDataSeries s2(2, Wt::Chart::LineSeries);
      s2.setShadow(Wt::WShadow(3, 3, Wt::WColor(0, 0, 0, 127), 3));
      Wt::Chart::WDataSeries s3(3, Wt::Chart::LineSeries);
      s3.setShadow(Wt::WShadow(3, 3, Wt::WColor(0, 0, 0, 127), 3));
      Wt::Chart::WDataSeries s4(4, Wt::Chart::LineSeries);
      s4.setShadow(Wt::WShadow(3, 3, Wt::WColor(0, 0, 0, 127), 3));
      Wt::Chart::WDataSeries s5(5, Wt::Chart::LineSeries);
      s5.setShadow(Wt::WShadow(3, 3, Wt::WColor(0, 0, 0, 127), 3));
      chart->addSeries(s1);
      chart->addSeries(s2);
      chart->addSeries(s3);
      chart->addSeries(s4);
      chart->addSeries(s5);
      chart->resize(1024, 800);
//       chart->setMargin(Wt::WLength::Auto, Wt::Left | Wt::Right);
//       chart->axis(Wt::Chart::XAxis).setMinimum(Wt::WDateTime::currentDateTime().addSecs(-120));
//       chart->axis(Wt::Chart::YAxis).setAutoLimits(Wt::Chart::MinimumValue | Wt::Chart::MaximumValue);
      this->addWidget(chart);
    }
  };
} //namespace lughos
#endif