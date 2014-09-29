#ifndef TEST_GUI_HPP
#define TEST_GUI_HPP

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WTabWidget>
#include <Wt/WMenuItem>
#include <Wt/WEnvironment>
#include <Wt/WVBoxLayout>
#include <Wt/WBootstrapTheme>
#include <Wt/WCssTheme>
#include <functional>

namespace lughos 
{

  using namespace Wt;
  using namespace std;
  
  class mainApplication : public WApplication
  {
  public:
    
    mainApplication(const WEnvironment &env) : WApplication(env)
    {
      Wt::WBootstrapTheme *bootstrapTheme = new Wt::WBootstrapTheme(this);
      bootstrapTheme->setVersion(Wt::WBootstrapTheme::Version3);
      bootstrapTheme->setResponsive(true);
      this->setTheme(bootstrapTheme);

      // load the default bootstrap3 (sub-)theme
      this->useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
      setTitle("Lughos System Control");
      Wt::WContainerWidget *container = new Wt::WContainerWidget();
      Wt::WContainerWidget *headContainer = new Wt::WContainerWidget();
      headContainer->addWidget(new WText("LUGHOS System Control"));
      
      
      Wt::WVBoxLayout *vbox = new Wt::WVBoxLayout();
      container->setLayout(vbox);
      vbox->addWidget(headContainer);
      Wt::WTabWidget *tabW = new Wt::WTabWidget(container);
      tabW->addTab(new Wt::WTextArea("This is the contents of the first tab."),
		  "First", Wt::WTabWidget::PreLoading);
      tabW->addTab(new Wt::WTextArea("The contents of the tabs are pre-loaded in"
				    " the browser to ensure swift switching."),
		  "Preload", Wt::WTabWidget::PreLoading);
      tabW->addTab(new Wt::WTextArea("You could change any other style attribute of the"
				    " tab widget by modifying the style class."
				    " The style class 'trhead' is applied to this tab."),
		  "Style", Wt::WTabWidget::PreLoading)->setStyleClass("trhead");

      Wt::WMenuItem *tab 
	  = tabW->addTab(new Wt::WTextArea("You can close this tab"
					  " by clicking on the close icon."),
			"Close");
      tab->setCloseable(true);
      vbox->addWidget(tabW);
      tabW->setStyleClass("tabwidget");
      root()->addWidget(container);
    }
    
    
  };
  
} //namespace lughos

#endif