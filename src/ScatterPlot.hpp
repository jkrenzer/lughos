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
    Wt::WText* name;
    ScatterPlotWidget()
    {
      this->setStyleClass("ScatterPlotContainer");
      this->name = new Wt::WText();
      this->addWidget(name);
    }
  };
  
    template <class S> class ScatterPlot : public ScatterPlotWidget
  {
  public:
    ScatterPlot<S>()
    {
      this->addWidget(new Wt::WText("No GUI for scatter plots availible!"));
    }
  };
  #endif