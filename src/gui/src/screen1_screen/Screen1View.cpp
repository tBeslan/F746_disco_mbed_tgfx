#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
    analogHours = analogClock1.getCurrentHour();
    analogMinutes = analogClock1.getCurrentMinute();
    analogSeconds = analogClock1.getCurrentSecond();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent()
{
    tickCounter++;

    if (tickCounter % 60 == 0) {
        if (++analogSeconds >= 60) {
            analogSeconds = 0;
            if (++analogMinutes >= 60) {
                analogMinutes = 0;
                if (++analogHours >= 24) {
                    analogHours = 0;
                }
            }
        }

        // Update the clocks
        analogClock1.setTime24Hour(analogHours, analogMinutes, analogSeconds);
    }
}
