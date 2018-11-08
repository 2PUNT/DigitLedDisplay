#include "DigitLedDisplay.hpp"

void DigitLedDisplay::Display(int firstDigit,int secondDigit,bool leadingZero){
    segDisplay.showNumberDecEx(firstDigit,0x40,leadingZero,2,0);
    segDisplay.showNumberDecEx(secondDigit,0x40,true,2,2);
}

void DigitLedDisplay::ChangeBrightness(int brightness){
    segDisplay.setBrightness(brightness);
}

void DigitLedDisplay::TurnDisplayOn(bool newDisplayState){
    if(isDisplayOn && !newDisplayState){
        segDisplay.setBrightness(brightness, newDisplayState);
        isDisplayOn = false;
    }else if(!isDisplayOn && newDisplayState){
        segDisplay.setBrightness(brightness, newDisplayState);
        isDisplayOn = true;
    }
}

bool DigitLedDisplay::GetDisplayState(){
    return isDisplayOn;
}