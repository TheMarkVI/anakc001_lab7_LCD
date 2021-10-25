/*	Author: Artip Nakchinda
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 7  Exercise 2
 *	Exercise Description: 
 *          - One button
 *          - LCD
 *          - Light game:
 *          - Each time user presses button at the right time,
 *              (at the middle LED), the score increments
 *          - Score decrements if user fails
 *          - Display victory message when reaching 9
 *          - Display number on LCD
 *  Video Demo: https://emailucr-my.sharepoint.com/:v:/g/personal/artip_nakchinda_email_ucr_edu/Edoo5RNL_hBCkhhK3GDpLtkB08S9dJvqgaV9G-7L2Qljlw?e=MmfdCb
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

enum States{start, init, led1, led2, led3, buttonPress, buttonRel, victory} state;

unsigned char tempA = 0x00;
unsigned char score = 0;

void Tick(){ 
    tempA = ~PINA;

    switch(state) { // transitions
        case start:
            score = 5;
            state = init;
            LCD_ClearScreen();
            LCD_WriteData(score + '0');
            break;
        case init:
            if((tempA & 0x01) == 0x01)
                state = init;
            else
                state = led1;
            break;
        case led1:
            if((tempA & 0x01) == 0x01) { 
                state = buttonPress;
                if(score != 0)
                    score -= 1;
            }
            else
                state = led2;
            break;
        case led2:
            if((tempA & 0x01) == 0x01) {
                state = buttonPress;
                score += 1;
            }
            else
                state = led3;
            break;
        case led3:
            if((tempA & 0x01) == 0x01) {
                state = buttonPress;
                if(score != 0)
                    score -= 1;
            }
            else
                state = led1;
            break;
        case buttonPress:
            if((tempA & 0x01) == 0x01)
                state = buttonPress;
            else
                state = buttonRel;
            break;
        case buttonRel:
            if(score == 9)
                state = victory;
            else if ((tempA & 0x01) == 0x01)
                state = init;
            else
                state = buttonRel;
            break;
        case victory:
            if((tempA & 0x01) == 0x01)
                state = start;
            else
                state = victory;
            break;

        default:
            break;
    }

    switch(state) { // state actions
        case start:
            break;
        case init:
            PORTB = 0x00;
            break;
        case led1:
            PORTB = 0x01;
            break;
        case led2:
            PORTB = 0x02;
            break;
        case led3:
            PORTB = 0x04;
            break;
        case buttonPress:
        case buttonRel:
            LCD_ClearScreen();
            LCD_WriteData(score + '0');
            PORTB = PORTB;
            break;
        case victory:
            LCD_ClearScreen();
            LCD_DisplayString(1, "Victory!!!");
            break;
        default:
            break;
    }

}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // inputs
    DDRB = 0xFF; PORTB = 0x00; // LED output

    DDRC = 0xFF; PORTC = 0x00; // LCD data lines
    DDRD = 0xFF; PORTD = 0x00; // LCD control lines

    TimerSet(300);
    TimerOn();

    // Initialize LCD display
    LCD_init();
    
    /* Insert your solution below */
    while (1) {
        Tick();

        while(!TimerFlag) {}
        TimerFlag = 0;
        continue;
    }
    return 1;
}
