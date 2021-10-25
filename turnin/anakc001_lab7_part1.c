/*	Author: Artip Nakchinda
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 7  Exercise 1
 *	Exercise Description: 
 *          - Two buttons
 *          - LCD
 *          - Increment, Decrement, Reset
 *          - Display number on LCD
 *  Video Demo: https://emailucr-my.sharepoint.com/:v:/g/personal/artip_nakchinda_email_ucr_edu/EWi6vaUDQHJFvv8F7slSpskB5qUVk76Z9B_00nkqiZ0c2A?e=poBFBs
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
#include "timer.h"

#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{start, wait, increment, decrement, reset} state;

unsigned char tempA = 0x00;
unsigned char counter = 0;

void Tick(){ 
    tempA = ~PINA;

    switch(state) { // transitions
        case start:
            counter = 0;
            state = wait;
            break;
        case wait:
            if((tempA & 0x03) == 0x00)
                state = wait;
            else if((tempA & 0x03) == 0x01)
                state = increment;
            else if((tempA & 0x03) == 0x02)
                state = decrement;
            else if((tempA & 0x03) == 0x03)
                state = reset;
            break;
        case increment:
            state = wait;
            break;
        case decrement:
            state = wait;
            break;
        case reset:
            state = wait;
            break;
        default:
            break;
    }

    switch(state) { // state actions
        case start:
            break;
        case wait:            
            counter = counter;
            PORTB  = counter;
            LCD_ClearScreen();
            LCD_WriteData(counter + '0');
            break;
        case increment:
            if(counter != 9)
                counter += 1;
            break;
        case decrement:
            if(counter != 0)
                counter -= 1;
            break;
        case reset:
            counter = 0;
            break;
        default:
            break;
    }

}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // inputs
    DDRB = 0xFF; PORTB = 0x00; // LED output debug

    DDRC = 0xFF; PORTC = 0x00; // LCD data lines
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines

    TimerSet(100);
    TimerOn();

    // Initialize LCD display
    LCD_init();
    
    //Start at position 1 on LCD screen, write 'Hello World'
    // LCD_DisplayString(1, "Hello World!");
    // LCD_WriteData(9 + '0');

    
    /* Insert your solution below */
    while (1) {
        Tick();
        // PORTB = counter;    
        while(!TimerFlag) {}
        TimerFlag = 0;
        continue;
    }
    return 1;
}
