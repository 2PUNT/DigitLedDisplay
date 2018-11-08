#ifndef DIGIT_LED_DISPLAY_HPP
#define DIGIT_LED_DISPLAY_HPP

#include "hwlib_TM1637.hpp"
#include "hwlib.hpp"

///@class DigitLedDisplay
///@brief A class for the 4x 7-segment display boundary object.
class DigitLedDisplay{
private:
    int firstDigit;
    int secondDigit;
    int brightness;
    bool leadingZero;
    bool isDisplayOn = false;
    hwlib::target::pin_in_out CLK;
    hwlib::target::pin_in_out DIO;
    TM1637Display segDisplay;

public:
    ///@fn DigitLedDisplay::DigitLedDisplay(hwlib::target::pin_in_out &CLK, hwlib::target::pin_in_out &DIO, int brightness = 4)
    ///@brief The constructor for the DigitLedDisplay class.
    ///@details This constructor creates a DigitLedDisplay object. 
    ///@param CLK The clock pin of the display.
    ///@param DIO The data I/O pin of the display.
    DigitLedDisplay(hwlib::target::pin_in_out &CLK, hwlib::target::pin_in_out &DIO):
        CLK(CLK),DIO(DIO),
        segDisplay(CLK,DIO)
    {}
    ///@fn void DigitLedDisplay::Display(int firstDigit, int secondDigit)
    ///@brief The display method.
    ///@details This method displays the given numbers on the display.
    ///@param firstDigit Controls the two leftmost digits of the display.
    ///@param secondDigit Controls the two rightmost digits of the display.
    ///@param leadingZero If @c firstDigit is less than 10, a zero wil be placed at the leftmost position
    /// if leadingZero equals @c true. Default value is @c true.
    void Display(int firstDigit, int secondDigit, bool leadingZero = true);
    
    ///@fn void DigitLedDisplay::ChangeBrightness(int brightness)
    ///@brief Change brightness method.
    ///@details This method changes the brightness of the display.
    ///@param brightness A number from 0 (lowest brightness) to 7 (highest brightness).
    void ChangeBrightness(int brightness);
    
    ///@fn void DigitLedDisplay::TurnDisplayOn(bool newDisplayState)
    ///@brief Turns the display on.
    ///@details This methods turns the display on or off.
    ///@param newDisplayState Turns the display on (True) or off (False).
    void TurnDisplayOn(bool newDisplayState);
    
    ///@fn bool DigitLedDisplay::GetDisplayState()
    ///@brief Returns the state of the display.
    ///@details This method returns @c true if the display is on and returns @c false if the display os off.
    ///@return isDisplayOn
    bool GetDisplayState();
};

#endif // DIGIT_LED_DISPLAY_HPP