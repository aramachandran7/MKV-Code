// objective - flash test, blink LED1, PD7, pin 15


#include <avr/io.h>
#include "can_api.h"
#include <util/delay.h>

// defines


int main(void){
    // init LED 
    //
    DDRD |= _BV(DDD7); // toggle mode to output 
    PORTD |= _BV(PD7); // set to logic HIGH


    while(1){
        PORTD ^= _BV(PD7); 
        _delay_ms(500); 
    }
}
