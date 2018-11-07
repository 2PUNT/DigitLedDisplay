#include "DigitLedDisplay.hpp"

void DigitLedDisplay::Display(int firstDigit,int secondDigit){
    segDisplay.showNumberDecEx(firstDigit,0x40,true,2,0);
    segDisplay.showNumberDecEx(secondDigit,0x40,true,2,2);
}