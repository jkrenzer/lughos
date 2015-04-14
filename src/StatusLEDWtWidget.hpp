#include <iostream>
#include <Wt/WImage>
#include <Wt/WStackedWidget>
#include <Wt/WPopupMenu>
#include <Wt/WPushButton>

namespace lughos
{

  class StatusLEDWtWidget;
  
  class StatusLEDStateInterface
  {
  protected:
    friend class StatusLEDWtWidget;
    
    virtual void set(StatusLEDWtWidget* statusLED) = 0;
  public:
        Wt::WString message;
  };

  class StatusLEDWtWidget : public Wt::WStackedWidget
  {
  public:
    enum Color {Off, Red, Green, Blue, Yellow, Orange};
  protected:
    Wt::WImage* off;
    Wt::WImage* red;
    Wt::WImage* green;
    Wt::WImage* blue;
    Wt::WImage* yellow;
    Wt::WImage* orange;
    Wt::WPopupMenu *popup;
    Wt::WPopupMenuItem *popupState;
    Color color;
    
    boost::shared_ptr<StatusLEDStateInterface> state;
    Wt::WString statusMessage;
    
  public:
    
  StatusLEDWtWidget(WContainerWidget* parent = 0) : WStackedWidget(parent) , statusMessage("State undetermined")
  {
    this->off = new Wt::WImage("./resources/LED_off.png");
    this->red = new Wt::WImage("./resources/LED_red.png");
    this->green = new Wt::WImage("./resources/LED_green.png");
    this->blue = new Wt::WImage("./resources/LED_blue.png");
    this->yellow = new Wt::WImage("./resources/LED_yellow.png");
    this->orange = new Wt::WImage("./resources/LED_orange.png");
    this->popup = new Wt::WPopupMenu();
    
    this->addWidget(off);
    this->addWidget(red);
    this->addWidget(green);
    this->addWidget(blue);
    this->addWidget(yellow);
    this->addWidget(orange);
    
    this->color = Green;
    this->switchOff();
    this->popupState = this->popup->addItem(this->statusMessage);
    this->popup->addSeparator();
    this->clicked().connect(boost::bind(&Wt::WPopupMenu::popup,this->popup,this,Wt::Orientation::Vertical));
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

  void setState(boost::shared_ptr<StatusLEDStateInterface> state)
  {
    this->state = state;
    this->state->set(this);
  }
  
  void setState(StatusLEDStateInterface* state)
  {
    this->state.reset(state);
    this->state->set(this);
  }
  
  template <class T>
  void setState()
  {
    this->setState(new T());
  }

  void unsetState()
  {
    this->state.reset();
    this->switchOff();
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
  
  Wt::WPopupMenu* popupMenu()
  {
    return this->popup;
  }
  
  void setStatusMessage(Wt::WString message)
  {
    this->statusMessage = message;
    this->setToolTip(this->statusMessage,Wt::TextFormat::PlainText);
    this->popupState->setText(message);
  }
    
  };
  
  class StatusLEDState : public StatusLEDStateInterface
  {
  public:
    typedef StatusLEDWtWidget::Color Color;
    Color color;
  protected:
    
    friend class StatusLEDWtWidget;
    
    virtual void set(StatusLEDWtWidget* statusLED) 
    {
      statusLED->setColor(this->color);
      statusLED->setStatusMessage(this->message);
    }
  };
  
} //namespace lughos