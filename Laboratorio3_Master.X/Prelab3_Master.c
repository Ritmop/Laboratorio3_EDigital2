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
#include "LCD.h"

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
uint8_t pot0;
uint8_t pot1;
uint8_t count;
char pot0_s[] = {0,0,0,'\0'};
char pot1_s[] = {0,0,0,'\0'};
char count_s[] = {0,0,0,'\0'};
/*-------------------------------- PROTOTYPES --------------------------------*/
void setup(void);
void requestValues(void);
void outputValues(void);
void separar_digitos8(uint8_t num, char dig8[]);
/*------------------------------- RESET VECTOR -------------------------------*/

/*----------------------------- INTERRUPT VECTOR -----------------------------*/
void __interrupt() isr(void){
}
/*--------------------------- INTERRUPT SUBROUTINES --------------------------*/

/*---------------------------------- TABLES ----------------------------------*/

/*----------------------------------- MAIN -----------------------------------*/
int main(void) {
    setup();
    while(1){
        //Loop
        requestValues();    //Solicitar datos a PIC esclavos
        outputValues();     //Mostrar datos en LCD
    }
}
/*-------------------------------- SUBROUTINES -------------------------------*/
void setup(void){
    ANSEL = 0;
    ANSELH= 0;
    
    TRISD = 0;
    PORTD = 0;
    
    //OSCILLATOR CONFIG
    OSCCONbits.IRCF = 0b111;  //Internal clock frequency 8MHz
    SCS = 1;
    
    //LDC Init
    Lcd_Init();
    
    //SPI    
    TRISC = 0b11010100; //SPI Output/Input pins
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}

void requestValues(void){
    //Get POT 0 from Slave 0
    RC1 = 1;    //Disable slave 1
    RC0 = 0;    //Enable slave 0
    spiWrite('P');  //Request
    __delay_ms(10);
    pot0 = spiRead();  //Read POT0

    //Get POT 1 from Slave 1
    RC0 = 1;    //Disable slave 0
    RC1 = 0;    //Enable slave 1
    spiWrite('P');  //Request
    __delay_ms(10);
    pot1 = spiRead();  //Read POT1

    //Get Counter from Slave 1
    spiWrite('C');  //Request
    __delay_ms(10);
    count = spiRead();  //Read Counter1       
}

void outputValues(void){
    //Preparar datos
    separar_digitos8(pot0,pot0_s);
    separar_digitos8(pot1,pot1_s);
    separar_digitos8(count,count_s);
    //Mostrar en LCD
    //Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("P0: ");
    Lcd_Write_String(pot0_s);
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("P1: ");
    Lcd_Write_String(pot1_s);
    Lcd_Set_Cursor(1,11);
    Lcd_Write_String("C: ");
    Lcd_Write_String(count_s);
    
}

void separar_digitos8(uint8_t num, char dig8[]){
    uint8_t div1,div2,div3,centenas,decenas,unidades;
    div1 = num / 10;
    unidades = num % 10;
    div2 = div1 / 10;
    decenas = div1 % 10;    
    centenas = div2 % 10;
    
    dig8[2] = unidades + 0x30;
    dig8[1] = decenas  + 0x30;
    dig8[0] = centenas + 0x30;
}