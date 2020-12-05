#include <xc.h>
#include <pic18f4520.h>
#include "adc.h"
#include "bits.h"
#include "config.h"
#include "io.h"
#include "keypad.h"
#include "lcd.h"

#define RIGHT_BUTTON 0 
#define LEFT_BUTTON  3

#define CELSIUS_LED 5 
#define FAHR_LED  6
#define KELVIN_LED 7 

void celsius(void);
void fahr(void);
void kelvin(void);
void itoa(unsigned int val, char* str);
void readButton(void);
void atraso_ms(unsigned int valor);

int menuNum = 1;
char str[6];

char grau = 223; //Simbolo grau
unsigned char tecla = 0;

void main() {
    kpInit();
    adcInit();
    lcdInit();
    while (HIGH) {
        kpDebounce();
        readButton();
        
        switch (menuNum) {
            case 1:
                bitSet(PORTB, CELSIUS_LED);
                bitClr(PORTB, FAHR_LED);
                bitClr(PORTB, KELVIN_LED);                
                celsius();
                break;
            case 2:
                bitClr(PORTB, CELSIUS_LED);
                bitSet(PORTB, FAHR_LED);
                bitClr(PORTB, KELVIN_LED);
                fahr();
                break;
            case 3:
                bitClr(PORTB, CELSIUS_LED);
                bitClr(PORTB, FAHR_LED);
                bitSet(PORTB, KELVIN_LED);
                kelvin();
                break;
        }
    }
}

void celsius(void) {
    int tmp;
    lcdSetCursor(0, 0);
    lcdPrint("     Celsius   >");
    
    tmp = (adcRead(0)* 0.0048828125 * 100); //Leio o potenciômetro
    itoa(tmp, str);
    
    lcdSetCursor(1, 5);
    lcdData(str[2]);
    lcdData(str[3]);
    lcdData(str[4]);
    lcdData(',');
    lcdData(str[5]);
    lcdPrint(" ");
    lcdData(grau);
    lcdPrint("C  ");
}

void fahr(void) {
    int tmp, tmpFahr;
    lcdSetCursor(0, 0);
    lcdPrint("<  Fahrenheit  >");
    
    tmp = (adcRead(0)* 0.0048828125 * 100); //Leio o potenciômetro
    tmpFahr = (tmp*9/5) + 32;
    itoa(tmpFahr, str);
    
    lcdSetCursor(1, 5);
    lcdData(str[2]);
    lcdData(str[3]);
    lcdData(str[4]);
    lcdData(',');
    lcdData(str[5]);
    lcdPrint(" ");
    lcdData(grau);
    lcdPrint("F  ");
}

void kelvin(void) {
    int tmp, tmpKel;
    lcdSetCursor(0, 0);
    lcdPrint("<    Kelvin     ");
    
    tmp = (adcRead(0)* 0.0048828125 * 100); //Leio o potenciômetro
    tmpKel = tmp + 273;
    itoa(tmpKel, str);
    
    lcdSetCursor(1, 5);
    lcdData(str[2]);
    lcdData(str[3]);
    lcdData(str[4]);
    lcdData(',');
    lcdData(str[5]);
    lcdPrint(" ");
    lcdPrint("K   ");
}

void itoa(unsigned int val, char* str) {
    str[0] = (val / 10000) + 0x30;
    str[1] = ((val % 10000) / 1000) + 0x30;
    str[2] = ((val % 1000) / 100) + 0x30;
    str[3] = ((val % 100) / 10) + 0x30;
    str[4] = (val % 10) + 0x30;
    str[5] = 0 + 0x30;
}

void readButton(void) {
    if (kpRead() != tecla) {
        tecla = kpRead();

        if (bitTst(tecla, RIGHT_BUTTON)) { //Asterisco
            if (menuNum <= 3) menuNum++;
        } else if (bitTst(tecla, LEFT_BUTTON)) { //Tecla 1
            if (menuNum > 0) menuNum--;
        }
    }
}

void atraso_ms(unsigned int valor) {
    unsigned int i;
    volatile unsigned char j, k;

    for (i = 0; i < valor; i++) {

        //Dois for's que gastam aprox 1ms
        for (j = 0; j < 41; j++) {
            for (k = 0; k < 3; k++);
        }
    }
}

