#ifndef DIGIT_LED_DISPLAY_HPP
#define DIGIT_LED_DISPLAY_HPP

#include "hwlib.hpp"

class DigitLedDisplay{
private:
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
    const uint8_t digitToSegment[16] = {
//    XGFEDCBA
        0b00111111,    // 0
        0b00000110,    // 1
        0b01011011,    // 2
        0b01001111,    // 3
        0b01100110,    // 4
        0b01101101,    // 5
        0b01111101,    // 6
        0b00000111,    // 7
        0b01111111,    // 8
        0b01101111,    // 9
        0b01110111,    // A
        0b01111100,    // b
        0b00111001,    // C
        0b01011110,    // d
        0b01111001,    // E
        0b01110001     // F
    };
    
    uint8_t m_brightness;
    bool leadingZero;
    bool isDisplayOn;
    bool newDisplayState;
    uint8_t firstDigit;
    uint8_t secondDigit;
    
    
    hwlib::target::pin_in_out &m_pinCLK;
    hwlib::target::pin_in_out &m_pinDIO;
    
    void bitDelay();
    void start();
    void stop();
    bool writeByte(uint8_t b);
    void showDots(uint8_t dots, uint8_t* digits);
    void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
    void showNumberDecEx(int num, uint8_t dots, bool leading_zero,uint8_t length, uint8_t pos);
    void setSegments(const uint8_t segments[], uint8_t length, uint8_t pos);
    uint8_t encodeDigit(uint8_t digit);
    void setBrightness(uint8_t brightness, bool on = true);
    void clear();
    
public:
    ///@fn DigitLedDisplay::DigitLedDisplay(hwlib::target::pin_in_out &CLK, hwlib::target::pin_in_out &DIO, int brightness = 4)
    ///@brief The constructor for the DigitLedDisplay class.
    ///@details This constructor creates a DigitLedDisplay object. 
    ///@param CLK The clock pin of the display.
    ///@param DIO The data I/O pin of the display.
    DigitLedDisplay(hwlib::target::pin_in_out &m_pinCLK, hwlib::target::pin_in_out &m_pinDIO):
        m_pinCLK(m_pinCLK), m_pinDIO(m_pinDIO)
        {
            m_pinDIO.direction_set_input();           //CHANGE DIRECTION
            m_pinCLK.direction_set_input();
            m_pinDIO.set(0);
            m_pinCLK.set(0);
        }

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
    
    ///@fn void DigitLedDisplay::ClearDisplay()
    ///@brief Clears the display
    ///@details This method clears the display.
    void ClearDisplay(int brightness = 4);
    
    ///@fn bool DigitLedDisplay::GetDisplayState()
    ///@brief Returns the state of the display.
    ///@details This method returns @c true if the display is on and returns @c false if the display os off.
    ///@return isDisplayOn
    bool GetDisplayState();
};

#endif // DIGIT_LED_DISPLAY_HPP