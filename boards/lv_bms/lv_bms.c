/*
 *  2021 LV BMS code
 *  Code architecture documented in confluence under LV Battery and BMS
 *  inspiration taken from bms.c, thanks to Corey, Vienna, Hoppe
 * 
 * @author Adi Ramachandran '23
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "can_api.h"
#include "log_uart.h"
#include "ltc6810.h"

// gFlag pre-defines
#define TIMER_BIT           0b00000001
#define UV                  0b00000010
#define OV                  0b00000100
#define UT                  0b00001000
#define soft_OT             0b00010000
#define OT                  0b00100000


#define MAX_MISSED_CYCLES   3
#define NUM_TEMPS           6



volatile __uint16_t gFlag = 0x00; 
volatile __uint16_t missed_cycles = 0x00; 

volatile uint16_t temp_buffer[NUM_TEMPS] = { 0x00 }; 
volatile uint16_t temp_buffer[NUM_TEMPS] = { 0x00 }; 



const uint8_t MUX_ADDR = 0x90;  




/* timer interrupt*/



/* set timer at 100Hz*/
void init_timer(){
    // register magic, 100Hz? 
}

void LTC_get_cell_voltages(){

}

/* sets gFlag with */
void LTC_get_pack_temps(){

}



int main (void){
    


    init_timer(); 



    while (1){
        if bit_is_set(gFlag, timer_bit){


            // data grabs 
            LTC_get_cell_voltages(); 
            LTC_get_pack_temps(); 



            // conditional logic 


        }
    }
}