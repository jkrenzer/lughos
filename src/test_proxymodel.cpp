#include <Wt/Chart/WCartesianChart>
#include <Wt/Chart/WDataSeries>
#include <Wt/WAbstractItemModel>
#include <Wt/WAbstractItemView>
#include <boost/shared_ptr.hpp>
#include <measuredDBValue.hpp>

int main()
{
    Wt::Chart::WCartesianChart *chart;
    boost::shared_ptr<Wt::Dbo::Session> session;
    Wt::Dbo::backend::Sqlite3 dbBackend("test.db");
    this->session->setConnection(this->dbBackend);
    this->session->mapClass<measuredDBValue<double> >("measuredValue");
    return 0;
}
