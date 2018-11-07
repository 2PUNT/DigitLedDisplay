#ifndef HWLIB_TM1637_HPP
#define HWLIB_TM1637_HPP

//  Ported to hwlib from https://github.com/avishorp/TM1637

static const int SEG_A = 0b00000001;
static const int SEG_B = 0b00000010;
static const int SEG_C = 0b00000100;
static const int SEG_D = 0b00001000;
static const int SEG_E = 0b00010000;
static const int SEG_F = 0b00100000;
static const int SEG_G = 0b01000000;

static const int DEFAULT_BIT_DELAY = 100;

#include "hwlib.hpp"

class TM1637Display {
public:
    //! Initialize a TM1637Display object, setting the clock and
    //! data pins.
    //!
    //! @param pinClk - The number of the digital pin connected to the clock pin of the module
    //! @param pinDIO - The number of the digital pin connected to the DIO pin of the module
    //! @param bitDelay - The delay, in microseconds, between bit transition on the serial
    //!                   bus connected to the display
    TM1637Display(hwlib::target::pin_in_out &m_pinClk, hwlib::target::pin_in_out &m_pinDIO, unsigned int bitDelay = DEFAULT_BIT_DELAY):
        m_pinClk(m_pinClk), m_pinDIO(m_pinDIO)
    {
        m_bitDelay = bitDelay;
        // Set the pin direction and default value.
        // Both pins are set as inputs, allowing the pull-up resistors to pull them up
    //    pinMode(m_pinClk, INPUT);
    //    pinMode(m_pinDIO,INPUT);
        m_pinDIO.direction_set_input();           //CHANGE DIRECTION
        m_pinClk.direction_set_input();
        m_pinDIO.set(0);
        m_pinClk.set(0);
        
    }

    //! Sets the brightness of the display.
    //!
    //! The setting takes effect when a command is given to change the data being
    //! displayed.
    //!
    //! @param brightness A number from 0 (lowes brightness) to 7 (highest brightness)
    //! @param on Turn display on or off
    void setBrightness(uint8_t brightness, bool on = true);

    //! Display arbitrary data on the module
    //!
    //! This function receives raw segment values as input and displays them. The segment data
    //! is given as a byte array, each byte corresponding to a single digit. Within each byte,
    //! bit 0 is segment A, bit 1 is segment B etc.
    //! The function may either set the entire display or any desirable part on its own. The first
    //! digit is given by the @ref pos argument with 0 being the leftmost digit. The @ref length
    //! argument is the number of digits to be set. Other digits are not affected.
    //!
    //! @param segments An array of size @ref length containing the raw segment values
    //! @param length The number of digits to be modified
    //! @param pos The position from which to start the modification (0 - leftmost, 3 - rightmost)
    void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);

    //! Clear the display
    void clear();

    //! Display a decimal number
    //!
    //! Dispaly the given argument as a decimal number.
    //!
    //! @param num The number to be shown
    //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
    //!        blank. NOTE: leading zero is not supported with negative numbers.
    //! @param length The number of digits to set. The user must ensure that the number to be shown
    //!        fits to the number of digits requested (for example, if two digits are to be displayed,
    //!        the number must be between 0 to 99)
    //! @param pos The position of the most significant digit (0 - leftmost, 3 - rightmost)
    void showNumberDec(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

    //! Display a decimal number, with dot control
    //!
    //! Dispaly the given argument as a decimal number. The dots between the digits (or colon)
    //! can be individually controlled.
    //!
    //! @param num The number to be shown
    //! @param dots Dot/Colon enable. The argument is a bitmask, with each bit corresponding to a dot
    //!        between the digits (or colon mark, as implemented by each module). i.e.
    //!        For displays with dots between each digit:
    //!        * 0.000 (0b10000000)
    //!        * 00.00 (0b01000000)
    //!        * 000.0 (0b00100000)
    //!        * 0.0.0.0 (0b11100000)
    //!        For displays with just a colon:
    //!        * 00:00 (0b01000000)
    //!        For displays with dots and colons colon:
    //!        * 0.0:0.0 (0b11100000)
    //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
    //!        blank. NOTE: leading zero is not supported with negative numbers.
    //! @param length The number of digits to set. The user must ensure that the number to be shown
    //!        fits to the number of digits requested (for example, if two digits are to be displayed,
    //!        the number must be between 0 to 99)
    //! @param pos The position of the most significant digit (0 - leftmost, 3 - rightmost)
    void showNumberDecEx(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

    //! Display a hexadecimal number, with dot control
    //!
    //! Dispaly the given argument as a hexadecimal number. The dots between the digits (or colon)
    //! can be individually controlled.
    //!
    //! @param num The number to be shown
    //! @param dots Dot/Colon enable. The argument is a bitmask, with each bit corresponding to a dot
    //!        between the digits (or colon mark, as implemented by each module). i.e.
    //!        For displays with dots between each digit:
    //!        * 0.000 (0b10000000)
    //!        * 00.00 (0b01000000)
    //!        * 000.0 (0b00100000)
    //!        * 0.0.0.0 (0b11100000)
    //!        For displays with just a colon:
    //!        * 00:00 (0b01000000)
    //!        For displays with dots and colons colon:
    //!        * 0.0:0.0 (0b11100000)
    //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
    //!        blank
    //! @param length The number of digits to set. The user must ensure that the number to be shown
    //!        fits to the number of digits requested (for example, if two digits are to be displayed,
    //!        the number must be between 0 to 99)
    //! @param pos The position of the most significant digit (0 - leftmost, 3 - rightmost)
    void showNumberHexEx(uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

    //! Translate a single digit into 7 segment code
    //!
    //! The method accepts a number between 0 - 15 and converts it to the
    //! code required to display the number on a 7 segment display.
    //! Numbers between 10-15 are converted to hexadecimal digits (A-F)
    //!
    //! @param digit A number between 0 to 15
    //! @return A code representing the 7 segment image of the digit (LSB - segment A;
    //!         bit 6 - segment G; bit 7 - always zero)
    uint8_t encodeDigit(uint8_t digit);

protected:
    void bitDelay();
    void start();
    void stop();
    bool writeByte(uint8_t b);
    void showDots(uint8_t dots, uint8_t* digits);
    void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

private:
	hwlib::target::pin_in_out m_pinClk;
	hwlib::target::pin_in_out m_pinDIO;
	uint8_t m_brightness;
	unsigned int m_bitDelay;
};

#endif // HWLIB_TM1637_HPP