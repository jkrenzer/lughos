#include <iostream>
#include <Wt/WImage>
#include <Wt/WStackedWidget>

class StatusLEDWtWidget : Wt::WStackedWidget
{
public:
  enum Color {Off, Red, Green, Blue, Yellow, Orange};
protected:
  Wt::WImage off;
  Wt::WImage red;
  Wt::WImage green;
  Wt::WImage blue;
  Wt::WImage yellow;
  Wt::WImage orange;
  Color color;
public:
  
StatusLEDWtWidget(WContainerWidget* parent = 0) : WStackedWidget(parent)
{
  this->off = new Wt::WImage("./resources/LED_off.png");
  this->red = new Wt::WImage("./resources/LED_red.png");
  this->green = new Wt::WImage("./resources/LED_green.png");
  this->blue = new Wt::WImage("./resources/LED_blue.png");
  this->yellow = new Wt::WImage("./resources/LED_yellow.png");
  this->orange = new Wt::WImage("./resources/LED_orange.png");
  
  this->addWidget(off);
  this->addWidget(red);
  this->addWidget(green);
  this->addWidget(blue);
  this->addWidget(yellow);
  this->addWidget(orange);
  
  this->color = Green;
  this->switchOff();
}

virtual ~StatusLEDWtWidget()
{
  delete this->off;
  delete this->red;
  delete this->green;
  delete this->blue;
  delete this->yellow;
  delete this->orange;
  
}

void switchOff()
{
  this->setCurrentIndex(Off);
}

void switchOn()
{
  this->setCurrentIndex(color);
}

void setColor(Color color)
{
  this->color = color;
  this->setCurrentIndex(color);  
}
  
};