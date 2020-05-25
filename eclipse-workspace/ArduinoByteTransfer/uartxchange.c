#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>


#ifndef F_CPU
#warning "F_CPU was not defined, defining it now as 16000000"

#define F_CPU 16000000UL
#endif

#define BAUD 9600UL      // define pre-determined baud rate

/*The lower 4-bit of the UBRR0H (USART Baud Rate Register 0 High is combined with the 8-bit of the UBRR0L-register to
select the baud rate (Bd) of the UART port. The Bd is calculated, as per data sheets of Atmel, using the following formula:

Baud Rate (Bd) = clkSYS or F_CPU/(16(UBBRn + 1); where UBBRn = <UBRR0H, UBBR0L = 12-bit>  */

// Calculations
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // smart rounding
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // real baud rate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // error in parts per mill, 1000 = no error

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Error in baud rate greater than 1%!
#endif


//UART-Initialization from www.mikrocontroller.net
void uart_init(void)
{
    UBRR0H = UBRR_VAL >> 8;
    UBRR0L = UBRR_VAL & 0xFF;

    // asynchronous 8N1: 8 bits data, no parity bit, 1 stop bit
    UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(1<<UCSZ01)|(1<<UCSZ00);
    // enable UART RX and TX
    UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
}

//receive a byte from USB (which chip on Arduino converts to UART)

//see https://forum.arduino.cc/index.php?topic=483133.0 Section 7.5 for reference
uint8_t uart_getc(void)
{
	//check to see if the RX Complete bit of the UCSR0A register is 1
    while (!(UCSR0A & (1<<RXC0)))   // wait/idle until symbol is ready
        ;

    //read data byte from RX register
    return UDR0;                    // return symbol
}

uint8_t uart_putc(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = data;
	//UCSR0A = ((UCSR0A) & ((1 << U2X0) | (1 << MPCM0))) | (1 << TXC0);
	return 0;
}

//test function, a little bit different
uint8_t uart_putchar(char c) {
	UDR0=c;
	//wait for transmission to complete
	loop_until_bit_is_set(UCSR0A, TXC0);
}


void initIO(void)
{
	//set digital pin 3 to output
	DDRD |= (1<<DDD3);

	//set all pins on port B as output (digital pins 8-13)
    DDRB = 0xff;
}

volatile uint8_t data = 10;
#define LED PB5 // LED is on Pin 13 (fifth pin of Port B)

int main(void)
{
  initIO();
  uart_init();
  PORTB |= (1<<LED);
  while (1)
  {
	//transmit test char 0x48 to Android
	uart_putc('H');

	//get the uart byte sent from PC
    uint8_t c = uart_getc();

    //if byte is of even parity, turn off LED (set LED pin to 0)
    if((c & 0x01) == 0)
    	PORTB &= ~(1<<LED);

    //if byte is of odd parity, turn on LED (set LED pin to 1)
    else
    	PORTB |= (1<<LED);
  }

  return 0; // never reached
}

//interrupt handler
ISR(USART_RX_vect) {
	data = UDR0; //read data from UART data register and store it in global data var
}
