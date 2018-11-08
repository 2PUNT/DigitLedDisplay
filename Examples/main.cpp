#include "DigitLedDisplay.hpp"
#include "hwlib.hpp"

void testDisplay(DigitLedDisplay &display){
    //Display numbers, without leading zero
    for(int i=20; i<=0; i--){
        display.Display(i,0,false);
        hwlib::wait_ms(500);
    }
    hwlib::wait_ms(1000);
    
    //Display numbers, with leading zero
    for(int i=0; i<=10; i++){
        for(int j=0; j<10; j++){
            display.Display(i,j);
            hwlib::wait_ms(500);
        }
    }
    hwlib::wait_ms(1000);
}

void testBrightness(DigitLedDisplay &display){
    //Change brightness
    display.Display(12,34);
    for(int i=7; i>=0; i--){
        display.ChangeBrightness(i);
    }
    display.ChangeBrightness(4);
    hwlib::wait_ms(1000);
}

void testDisplayOnOff(DigitLedDisplay &display){
    //Turn display on and off, get display state
    for(int i=0; i<4; i++){
        display.TurnDisplayOn(true);
        hwlib::cout << hwlib::boolalpha << display.GetDisplayState() << hwlib::endl;
        hwlib::wait_ms(500);
        display.TurnDisplayOn(false);
        hwlib::cout << hwlib::boolalpha << display.GetDisplayState() << hwlib::endl;
        hwlib::wait_ms(500);
    }
    display.TurnDisplayOn(true);
    hwlib::wait_ms(1000);
}

int main(){
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    
    auto DIO = hwlib::target::pin_in_out(hwlib::target::pins::d22);
    auto CLK = hwlib::target::pin_in_out(hwlib::target::pins::d24);
	DigitLedDisplay display(CLK, DIO);
    
    testDisplay(display);
    testBrightness(display);
    testDisplayOnOff(display);
    hwlib::cout << "Done" << hwlib::endl;
    
}
