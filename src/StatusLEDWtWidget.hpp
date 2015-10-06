#include <iostream>
#include <Wt/WImage>
#include <Wt/WStackedWidget>
#include <Wt/WPopupMenu>
#include <Wt/WPushButton>
#include <Wt/WApplication>
#include "threadSafety.hpp"

namespace lughos
{

  class StatusLEDWtWidget;
  
  class StatusLEDStateInterface
  {
  protected:
    Mutex mutex;
    friend class StatusLEDWtWidget;
    
    virtual void set(boost::shared_ptr<StatusLEDWtWidget> statusLED) = 0;
  public:
        Wt::WString message;
  };

  class StatusLEDWtWidget : public Wt::WStackedWidget, public boost::enable_shared_from_this<StatusLEDWtWidget>
  {
  public:
    enum Color {Off, Red, Green, Blue, Yellow, Orange};
 
  protected:
    Mutex mutex;
    
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
    ExclusiveLock lock(mutex);
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
    this->popupState = this->popup->addItem(this->statusMessage);
    this->popup->addSeparator();
    this->clicked().connect(boost::bind(&Wt::WPopupMenu::popup,this->popup,this,Wt::Orientation::Vertical));
    lock.unlock();
    this->switchOff();
  }

  virtual ~StatusLEDWtWidget()
  {
    ExclusiveLock lock(mutex);
    delete this->off;
    delete this->red;
    delete this->green;
    delete this->blue;
    delete this->yellow;
    delete this->orange;
    delete this->popup;
//     delete this->popupState;
  }

  void setState(boost::shared_ptr<StatusLEDStateInterface> state)
  {
    ExclusiveLock lock(mutex);
    this->state = state;
    lock.unlock();
    this->state->set(shared_from_this());
  }
  
  template <class T>
  void setState()
  {
    this->setState(boost::shared_ptr<StatusLEDStateInterface>(new T()));
  }

  void unsetState()
  {
    ExclusiveLock lock(mutex);
    this->state.reset();
    lock.unlock();
    this->switchOff();
  }

  void switchOff()
  {
    ExclusiveLock lock(mutex);
    this->setCurrentIndex(Off);
  }

  void switchOn()
  {
    ExclusiveLock lock(mutex);
    this->setCurrentIndex(color);
  }

  void setColor(Color color)
  {
    ExclusiveLock lock(mutex);
    this->color = color;
    if(color < this->count())
    {
//       Wt::WApplication::UpdateLock lock(Wt::WApplication::instance());
      this->setCurrentIndex(color);
//       Wt::WApplication::instance()->triggerUpdate();
    }
  }
  
  Wt::WPopupMenu* popupMenu()
  {
    SharedLock lock(mutex);
    return this->popup;
  }
  
  void setStatusMessage(Wt::WString message)
  {
    ExclusiveLock lock(mutex);
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
    
    virtual void set(boost::shared_ptr<StatusLEDWtWidget> statusLED) 
    {
      ExclusiveLock lock(this->mutex);
      if(statusLED)
      {
        statusLED->setColor(this->color);
        statusLED->setStatusMessage(this->message);
      }
    }
  };
  
} //namespace lughos