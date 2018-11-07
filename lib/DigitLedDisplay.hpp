#ifndef DIGIT_LED_DISPLAY_HPP
#define DIGIT_LED_DISPLAY_HPP

#include "hwlib_TM1637.hpp"
#include "hwlib.hpp"

///@class DigitLedDisplay
///@brief A class for the 4x 7-segment display.
class DigitLedDisplay{
private:
    int firstDigit;
    int secondDigit;
    int brightness;
    hwlib::target::pin_in_out CLK;
    hwlib::target::pin_in_out DIO;
    TM1637Display segDisplay;

public:
    ///@fn DigitLedDisplay::DigitLedDisplay(hwlib::target::pin_in_out &CLK, hwlib::target::pin_in_out &DIO, int brightness = 4)
    ///@brief The constructor for the DigitLedDisplay class.
    ///@details This constructor creates a DigitLedDisplay object. 
    ///@param CLK The clock pin of the display.
    ///@param DIO The data I/O pin of the display.
    ///@param brightness Sets the brightness of the display. Default value 4. 
    DigitLedDisplay(hwlib::target::pin_in_out &CLK, hwlib::target::pin_in_out &DIO, int brightness = 4):
        brightness(brightness),CLK(CLK),DIO(DIO),
        segDisplay(CLK,DIO)
    {
        segDisplay.setBrightness(brightness);
    }
    ///@fn void DigitLedDisplay::Display(int firstDigit, int secondDigit)
    ///@brief The display function.
    ///@details This function displays the given numbers on the display.
    ///@param firstDigit Controls the two leftmost digits of the display.
    ///@param secondDigit Controls the two rightmost digits of the display.
    void Display(int firstDigit, int secondDigit);
};

#endif // DIGIT_LED_DISPLAY_HPP