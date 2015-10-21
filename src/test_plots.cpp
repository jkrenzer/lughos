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

#include <iostream>
#include <string>
#include <exception>

#include "Wt/Dbo/backend/Sqlite3"
#include "Wt/Dbo/Exception"

#include <Wt/WServer>
#include <Wt/WGlobal>
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WTableView>
#include <Wt/Dbo/Dbo>
#include <Wt/WText>
#include <Wt/WDate>
#include <Wt/WStandardItemModel>
#include <Wt/WStandardItem>
#include <Wt/WStringListModel>
#include <Wt/Chart/WCartesianChart>
#include <Wt/Dbo/Query>
#include <Wt/Dbo/QueryModel>
#include <Wt/Dbo/QueryModel_impl.h>

using namespace Wt;
using namespace std;
using namespace Wt::Dbo;

WApplication *createApplication(const WEnvironment& env);

class Test {
	public:
		boost::posix_time::ptime  col1;
		double col2;
		
	public:
	  
		Test() : col1(boost::posix_time::time_from_string("1981-08-20 08:05:00")), col2(8) { }
		Test( boost::posix_time::ptime acol1) : col1(acol1), col2(0) { }
		Test( boost::posix_time::ptime acol1, double acol2) : col1(acol1), col2(acol2) { }

		template<class Action>
		void persist(Action& a) {
			field(a, col1, "column1");
			field(a, col2, "column2");
		}
};

class myAppl : public WApplication {
	public:
		myAppl(const WEnvironment& env);

	private:
		backend::Sqlite3 sqlite3;
		Session session;
};

myAppl::myAppl(const Wt::WEnvironment& env) : WApplication(env), sqlite3("test.db") {

	session.setConnection(sqlite3);
	session.mapClass<Test>("test");
	
	try {
		Transaction tt(session);
		session.createTables();
		tt.commit();
	} catch (Exception &e) {
		cerr << "Exception: " << e.what() << endl;
	}
	Transaction transaction(session);
	Test *test1 = new Test(boost::posix_time::time_from_string("1981-08-20 08:05:00"),1.1);
	Test *test2 = new Test(boost::posix_time::time_from_string("1982-08-20 09:05:00"),2.2);
	Test *test3 = new Test(boost::posix_time::time_from_string("1984-08-20 10:05:00"),3.3);
	Test *test4 = new Test(boost::posix_time::time_from_string("1987-08-20 11:05:00"),4.4);
	ptr<Test> test1Ptr = session.add(test1);
	ptr<Test> test2Ptr = session.add(test2);
	ptr<Test> test3Ptr = session.add(test3);
	ptr<Test> test4Ptr = session.add(test4);
	transaction.commit();

	Transaction transaction3(session);
	typedef boost::tuple<boost::posix_time::ptime, double> Item;
	QueryModel< Item > *model = new QueryModel< Item > ();
	Query< Item > dboquery = session.query< Item >("select * from test").limit(10);
	model->setQuery(dboquery);
// 	model->addAllFieldsAsColumns();
	model->addColumn("column1");
	model->addColumn("column2");
	
	WTableView *view = new WTableView();
	view->resize(800, 400);
	view->setModel(model);
	view->setAlternatingRowColors(true);
	transaction3.commit();
// 	
// 	
	root()->addWidget(new WText("<h1>Table1: Limit does not work through model->setQuery:</h1>"));
	root()->addWidget(view);
// 
// 	for (int row = 0; row < model->rowCount(); ++row) 
// 	{
// 	  Wt::WString s = Wt::asString(model->data(row, 1));
// // 	  boost::posix_time::ptime date = boost::posix_time::time_from_string(s.toUTF8());
// 	 root()->addWidget(new WText(s)); 
// // 	  model->setData(row, 0, WDateTime (date));
// 	}
// 	Wt::Chart::WCartesianChart *chart = new Wt::Chart::WCartesianChart();
// 	chart->setBackground(Wt::WColor(220, 220, 220));
// 	chart->setModel(model);
// 	chart->setXSeriesColumn(0);
// 	chart->setLegendEnabled(true);
// 	chart->setType(Wt::Chart::ScatterPlot);
// 	chart->axis(Wt::Chart::XAxis).setScale(Wt::Chart::DateScale);
// 
// 	chart->setPlotAreaPadding(40, Wt::Left | Wt::Top | Wt::Bottom);
// 	chart->setPlotAreaPadding(120, Wt::Right);	
// 	Wt::Chart::WDataSeries s(1, Wt::Chart::LineSeries);
// 	s.setShadow(Wt::WShadow(3, 3, Wt::WColor(0, 0, 0, 127), 3));
// 	chart->addSeries(s);
// 	chart->resize(800, 400);
// 	chart->setMargin(Wt::WLength::Auto, Wt::Left | Wt::Right);
// 	root()->addWidget(chart);
	
}

WApplication *createApplication(const WEnvironment& env) { return new myAppl(env); }

int main(int argc, char **argv) { return WRun(argc, argv, &createApplication); }