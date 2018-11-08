#include "DigitLedDisplay.hpp"
#include "hwlib.hpp"

int main(){
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    
    auto DIO = hwlib::target::pin_in_out(hwlib::target::pins::d22);
    auto CLK = hwlib::target::pin_in_out(hwlib::target::pins::d24);
    int brightness = 2;
	DigitLedDisplay display(CLK, DIO, brightness);
    
    for(int i=0; i<=10; i++){
        for(int j=0; j<10; j++){
            display.Display(i,j);
            hwlib::wait_ms(500);
        }
    }
}
