/*
 * File:   tank-controller.c
 * Author: sock.sqt@gmail.com
 *
 * Created on August 21, 2017, 9:42 PM
 */
#include <xc.h>

#pragma config CP = OFF, WDTE = OFF, PWRTE = ON, FOSC = XT
#define _XTAL_FREQ 4000000

#define interruptor PORTAbits.RA0

const unsigned char matrixLed[] = {
  // port A
  0x02,
  0x06,
  0x0e,
  // port B
  0x01,
  0x03,
  0x07,
  0x0f,
  0x1f,
  0x3f
};

// 5 cycles
const char FULL_CYCLES = 45; // 9 * N = Cycles

void printToPorts(int);

void main(void) {
  TRISA = 0b00000001;
  TRISB = 0x00;
  int counter = 0;
  unsigned char isLoading = 0;
  unsigned int loadingCounter = 0;
  unsigned int totalCycles = 0;

  while(1) {
    if (interruptor == 1) { // loading
      isLoading = 1;
      counter = counter == 9 ? 0 : counter;
      
      printToPorts(counter);
      __delay_ms(500);

      counter++;
      loadingCounter++;
    } else { // discharge
      if (isLoading == 1) { // Turn on all LEDS
        PORTA = 0xff;
        PORTB = 0xff;
        isLoading = 0;
        totalCycles = loadingCounter;
      }
      
      if (loadingCounter > 0) { // uncharge if and only if it was charged
        int indexToShow = (loadingCounter * 9) / totalCycles;
        printToPorts(indexToShow - 1);
        __delay_ms(500);
        loadingCounter--;
      }
    }
  }
}

void printToPorts(int index) {
  if (index < 0) {
    PORTA = 0x00;
    PORTB = 0x00;
  } else if (index < 3) {
    PORTA = matrixLed[index];
    PORTB = 0x00;
  } else if (index < 9) {
    PORTA = 0x0e;
    PORTB = matrixLed[index];
  }
}
