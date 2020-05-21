#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define PB5 5
#define LED PB5 // LED is on Pin 13 or Pin 5 of Port B

void initializeIO(void)
{
	DDRD = DDRD | 0b11111100;
	PORTD = 0b11111100;

	//OR the data direction register for port B with 2^5 (32)
	DDRB |= (1<<LED); //setting pin 13 to output
}


int test_fxn(void)
{
	//call setup
  initializeIO();

  while (1)
  {
	PORTB |= (1<<LED); // set LED pin to ON (1)
	//delay
	_delay_ms(500);
	PORTB &= ~(1<<LED); // clear; set LED pin to OFF (0)
	//delay
	_delay_ms(500);
  }

  return 0; // never reached
}

//set up PWM
void pwm_init(void){
	TCCR2A = (1<<COM2B1)|(1<<COM2B0)|(1<<WGM21)|(1<<WGM20)|(0<<CS22)|(0<<CS21)|(1<<CS20);
	TCCR2B |= (0<<CS22)|(0<<CS21)|(1<<CS20);
	OCR2B=128;
}

//OCR2B = ~c;



