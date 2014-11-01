#ifndef SCATTERPLOT_HPP
#define SCATTERPLOT_HPP

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <functional>


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
  #endif