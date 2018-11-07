#include "hwlib_TM1637.hpp"
#include "hwlib.hpp"

//  Ported to hwlib from https://github.com/avishorp/TM1637

static const int TM1637_I2C_COMM1 = 0x40;
static const int TM1637_I2C_COMM2 = 0xC0;
static const int TM1637_I2C_COMM3 = 0x80;

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] = {
 // XGFEDCBA
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

static const uint8_t minusSegments = 0b01000000;

void TM1637Display::setBrightness(uint8_t brightness, bool on){
	m_brightness = (brightness & 0x7) | (on? 0x08 : 0x00);
}

void TM1637Display::setSegments(const uint8_t segments[], uint8_t length, uint8_t pos){
    // Write COMM1
	start();
	writeByte(TM1637_I2C_COMM1);
	stop();

	// Write COMM2 + first digit address
	start();
	writeByte(TM1637_I2C_COMM2 + (pos & 0x03));

	// Write the data bytes
	for (uint8_t k=0; k < length; k++)
	  writeByte(segments[k]);

	stop();

	// Write COMM3 + brightness
	start();
	writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
	stop();
}

void TM1637Display::clear(){
    uint8_t data[] = { 0, 0, 0, 0 };
	setSegments(data);
}

void TM1637Display::showNumberDec(int num, bool leading_zero, uint8_t length, uint8_t pos){
    showNumberDecEx(num, 0, leading_zero, length, pos);
}

void TM1637Display::showNumberDecEx(int num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos){
    showNumberBaseEx(num < 0? -10 : 10, num < 0? -num : num, dots, leading_zero, length, pos);
}

void TM1637Display::showNumberHexEx(uint16_t num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos){
    showNumberBaseEx(16, num, dots, leading_zero, length, pos);
}

void TM1637Display::showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots,
                                     bool leading_zero, uint8_t length, uint8_t pos){
    bool negative = false;
	if (base < 0) {
	    base = -base;
		negative = true;
	}

    uint8_t digits[4];

	if (num == 0 && !leading_zero) {
		// Singular case - take care separately
		for(uint8_t i = 0; i < (length-1); i++)
			digits[i] = 0;
		digits[length-1] = encodeDigit(0);
	}else{
		//uint8_t i = length-1;
		//if (negative) {
		//	// Negative number, show the minus sign
		//    digits[i] = minusSegments;
		//	i--;
		//}
		
		for(int i = length-1; i >= 0; --i){
		    uint8_t digit = num % base;
			if (digit == 0 && num == 0 && leading_zero == false)
			    // Leading zero is blank
				digits[i] = 0;
			else
			    digits[i] = encodeDigit(digit);
				
			if (digit == 0 && num == 0 && negative) {
			    digits[i] = minusSegments;
				negative = false;
			}
			num /= base;
		}

		if(dots != 0){
			showDots(dots, digits);
		}
    }
    setSegments(digits, length, pos);
}

void TM1637Display::bitDelay(){
    hwlib::wait_us(m_bitDelay);
}

void TM1637Display::start(){
    m_pinDIO.direction_set_output();
    bitDelay();
}

void TM1637Display::stop(){
    m_pinDIO.direction_set_output();        // Pullup R On
	bitDelay();
    m_pinClk.direction_set_input();         // Pullup R Off
	bitDelay();
    m_pinDIO.direction_set_input();
	bitDelay();
}

bool TM1637Display::writeByte(uint8_t b){
    uint8_t data = b;

  // 8 Data Bits
    for(uint8_t i = 0; i < 8; i++) {
        // CLK low
        m_pinClk.direction_set_output();
        bitDelay();

        // Set data bit
        if (data & 0x01)
            m_pinDIO.direction_set_input();
        else
            m_pinDIO.direction_set_output();

        bitDelay();

        // CLK high
        m_pinClk.direction_set_input();
        bitDelay();
        data = data >> 1;
    }

    // Wait for acknowledge
    // CLK to zero
    m_pinClk.direction_set_output();
    m_pinDIO.direction_set_input();
    bitDelay();

    // CLK to high
    m_pinClk.direction_set_input();
    bitDelay();
    uint8_t ack = m_pinDIO.get();
    if (ack == 0)
        m_pinDIO.direction_set_output();


    bitDelay();
    m_pinClk.direction_set_output();
    bitDelay();

    return ack;
}

void TM1637Display::showDots(uint8_t dots, uint8_t* digits){
    for(int i = 0; i < 4; ++i){
        digits[i] |= (dots & 0x80);
        dots <<= 1;
    }
}

uint8_t TM1637Display::encodeDigit(uint8_t digit){
	return digitToSegment[digit & 0x0f];
}