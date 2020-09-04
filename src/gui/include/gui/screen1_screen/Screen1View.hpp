#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>

class Screen1View : public Screen1ViewBase {
public:
    Screen1View();
    virtual ~Screen1View() { }
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

protected:
    int tickCounter;
    int analogHours;
    int analogMinutes;
    int analogSeconds;
};

#endif // SCREEN1VIEW_HPP
