#ifndef DIGIT_LED_DISPLAY_HPP
#define DIGIT_LED_DISPLAY_HPP

#include "hwlib_TM1637.hpp"
#include "hwlib.hpp"

class DigitLedDisplay{
private:
    int firstDigit;
    int secondDigit;
    hwlib::target::pin_in_out CLK;
    hwlib::target::pin_in_out DIO;
    TM1637Display segDisplay;

public:
    DigitLedDisplay(hwlib::target::pin_in_out &CLK,hwlib::target::pin_in_out &DIO):
        CLK(CLK),DIO(DIO),
        segDisplay(CLK,DIO)
    {segDisplay.setBrightness(4);}
    void Display(int firstDisplay,int secondDisplay);
};

#endif // DIGIT_LED_DISPLAY_HPP