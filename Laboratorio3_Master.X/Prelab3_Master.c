/* 
 * File:   Prelab3_Slave.c
 * Device: PIC16F887
 * Author: Judah Pérez - 21536
 *Compiler: XC8 (v2.41)
 * 
 * Program: Ejemplo librerías
 * Hardware:
 * 
 * Created: Aug 1, 2023
 * Last updated:
 */

/*--------------------------------- LIBRARIES --------------------------------*/
#include <xc.h>
#include "SPI.h"

/*---------------------------- CONFIGURATION BITS ----------------------------*/
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

/*----------------------- GLOBAL VARIABLES & CONSTANTS -----------------------*/
#define _XTAL_FREQ 8000000
/*-------------------------------- PROTOTYPES --------------------------------*/
void setup(void);
/*------------------------------- RESET VECTOR -------------------------------*/

/*----------------------------- INTERRUPT VECTOR -----------------------------*/
void __interrupt() isr(void){
    if(SSPIF){
        SSPIF = 0;
    }
}

/*--------------------------- INTERRUPT SUBROUTINES --------------------------*/

/*---------------------------------- TABLES ----------------------------------*/

/*----------------------------------- MAIN -----------------------------------*/
int main(void) {
    setup();
    while(1){
        //Loop
        __delay_ms(1);
        //Get POT 0 from Slave 0
        RC1 = 1;    //Disable slave 1
        RC0 = 0;    //Enable slave 0
        spiWrite('a');  //Request
        PORTA = spiRead();  //Read
        __delay_ms(1);
        //Get POT 1 from Slave 1
        RC0 = 1;    //Disable slave 0
        RC1 = 0;    //Enable slave 1
        spiWrite('a');  //Request
        PORTB = spiRead();  //Read
    }
}
/*-------------------------------- SUBROUTINES -------------------------------*/
void setup(void){
    ANSEL = 0;
    ANSELH= 0;
    
    TRISA = 0;
    PORTA = 0;
    
    TRISB = 0;
    PORTB = 0;
    
    //OSCILLATOR CONFIG
    OSCCONbits.IRCF = 0b111;  //Internal clock frequency 8MHz
    SCS = 1;
    
    //SPI    
    TRISC = 0b00010000; //Slave select pins
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}