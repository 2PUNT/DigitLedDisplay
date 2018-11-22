#include "DigitLedDisplay.hpp"

static const int TM1637_I2C_COMM1 = 0x40;
static const int TM1637_I2C_COMM2 = 0xC0;
static const int TM1637_I2C_COMM3 = 0x80;

static const uint8_t minusSegments = 0b01000000;

void DigitLedDisplay::bitDelay(){
    hwlib::wait_us(100);
}

void DigitLedDisplay::start(){
    m_pinDIO.direction_set_output();
    bitDelay();
}

void DigitLedDisplay::stop(){
    m_pinDIO.direction_set_output();        // Pullup R On
	bitDelay();
    m_pinCLK.direction_set_input();         // Pullup R Off
	bitDelay();
    m_pinDIO.direction_set_input();
	bitDelay();
}

bool DigitLedDisplay::writeByte(uint8_t b){
    uint8_t data = b;

  // 8 Data Bits
    for(uint8_t i = 0; i < 8; i++){
        // CLK low
        m_pinCLK.direction_set_output();
        bitDelay();

        // Set data bit
        if (data & 0x01){
            m_pinDIO.direction_set_input();
        }else{
            m_pinDIO.direction_set_output();
        }
        bitDelay();

        // CLK high
        m_pinCLK.direction_set_input();
        bitDelay();
        data = data >> 1;
    }

    // Wait for acknowledge
    // CLK to zero
    m_pinCLK.direction_set_output();
    m_pinDIO.direction_set_input();
    bitDelay();

    // CLK to high
    m_pinCLK.direction_set_input();
    bitDelay();
    uint8_t ack = m_pinDIO.get();
    
    if (ack == 0) m_pinDIO.direction_set_output();

    bitDelay();
    m_pinCLK.direction_set_output();
    bitDelay();

    return ack;
}

void DigitLedDisplay::showDots(uint8_t dots, uint8_t* digits){
    for(int i = 0; i < 4; ++i){
        digits[i] |= (dots & 0x80);
        dots <<= 1;
    }
}

void DigitLedDisplay::showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos){
    bool negative = false;
	if (base < 0){
	    base = -base;
		negative = true;
	}

    uint8_t digits[4];

	if (num == 0 && !leading_zero){
		// Singular case - take care separately
		for(uint8_t i = 0; i < (length-1); i++){
			digits[i] = 0;
        }
		digits[length-1] = encodeDigit(0);
	}else{
		for(int i = length-1; i >= 0; --i){
		    uint8_t digit = num % base;
			if (digit == 0 && num == 0 && leading_zero == false){
			    // Leading zero is blank
				digits[i] = 0;
            }else{
			    digits[i] = encodeDigit(digit);
            }
			if (digit == 0 && num == 0 && negative){
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

void DigitLedDisplay::showNumberDecEx(int num, uint8_t dots, bool leading_zero,uint8_t length, uint8_t pos){
    showNumberBaseEx(num < 0? -10 : 10, num < 0? -num : num, dots, leading_zero, length, pos);
}

void DigitLedDisplay::setSegments(const uint8_t segments[], uint8_t length, uint8_t pos){
    // Write COMM1
	start();
	writeByte(TM1637_I2C_COMM1);
	stop();

	// Write COMM2 + first digit address
	start();
	writeByte(TM1637_I2C_COMM2 + (pos & 0x03));

	// Write the data bytes
	for(uint8_t k=0; k < length; k++){
        writeByte(segments[k]);
    }

	stop();

	// Write COMM3 + brightness
	start();
	writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
	stop();
}

uint8_t DigitLedDisplay::encodeDigit(uint8_t digit){
	return digitToSegment[digit & 0x0f];
}

void DigitLedDisplay::clear(){
    uint8_t data[] = { 0, 0, 0, 0 };
	setSegments(data,4,0);
}

void DigitLedDisplay::Display(int firstDigit,int secondDigit,bool leadingZero){
    showNumberDecEx(firstDigit,0x40,leadingZero,2,0);
    showNumberDecEx(secondDigit,0x00,true,2,2);
}

void DigitLedDisplay::setBrightness(uint8_t brightness, bool on){
	m_brightness = (brightness & 0x7) | (on? 0x08 : 0x00);
}

void DigitLedDisplay::ChangeBrightness(int brightness){
    setBrightness(brightness);
    Display(firstDigit,secondDigit,leadingZero);
}

void DigitLedDisplay::TurnDisplayOn(bool newDisplayState){
    if(isDisplayOn && !newDisplayState){
        setBrightness(m_brightness, newDisplayState);
        isDisplayOn = false;
    }else if(!isDisplayOn && newDisplayState){
        setBrightness(m_brightness, newDisplayState);
        isDisplayOn = true;
    }
    Display(firstDigit,secondDigit,leadingZero);
}

void DigitLedDisplay::ClearDisplay(int brightness){
    clear();
    setBrightness(brightness);
}

bool DigitLedDisplay::GetDisplayState(){
    return isDisplayOn;
}