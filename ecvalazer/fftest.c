/*------------------------------------------------*/
/* FFTEST : A test program for FFT module         */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "uart_logger.h"		/* Defs for using Software UART module (Debugging via AVRSP-COM) */
#include "ffft.h"		/* Defs for using Fixed-point FFT module */
#include <avr/delay.h>
#include <stdio.h>
#include "common_makros.h"

#define	SYSCLK		8000000


#include <avr/interrupt.h>   
//======================================================================================
//        Global variables                               */
//======================================================================================

char pool[16];	/* Console input buffer */

int16_t capture[FFT_N];			/* Wave captureing buffer */
complex_t bfly_buff[FFT_N];		/* FFT buffer */
uint16_t spektrum[FFT_N/2];		/* Spectrum output buffer */



//======================================================================================
//         Capture waveform                               */
//======================================================================================
void capture_wave (int16_t *buffer, uint16_t count)
{
	ADMUX = _BV(REFS0)|_BV(ADLAR)|_BV(MUX2)|_BV(MUX1)|_BV(MUX0);	// channel
	ADMUX = 0b01000111;


	do {
//		ADCSRA = _BV(ADEN)|_BV(ADSC)|_BV(ADATE)|_BV(ADIF)|_BV(ADPS2)|_BV(ADPS1);
//		ADCSRA = _BV(ADEN)|_BV(ADSC)|_BV(ADATE)|_BV(ADIF)|_BV(ADPS2);
		ADCSRA = _BV(ADEN)|_BV(ADSC)|_BV(ADATE)|_BV(ADIF)|_BV(ADPS2)|_BV(ADPS0);
		while(bit_is_clear(ADCSRA, ADIF));
		*buffer++ = ADC - 512;

	} while(--count);

	ADCSRA = 0;
}

//=====================================================================================

void ledOff()
{
	PORTC = 0;
	PORTD &= ~(0b11111100);

}

//=====================================================================================

void setChannel(char n)
{
	//PORTB &= 0b11100000;
	//PORTA &= 0b11100000;
	PORTB = 0;
	PORTA = 0;


	switch (n){
		case 0:  UPBIT(PORTB, 4); break;
		case 1:  UPBIT(PORTB, 3); break;
		case 2:  UPBIT(PORTB, 2); break;
		case 3:  UPBIT(PORTB, 1); break;
		case 4:  UPBIT(PORTB, 0); break;
		case 5:  UPBIT(PORTA, 0); break;
		case 6:  UPBIT(PORTA, 1); break;
		case 7:  UPBIT(PORTA, 2); break;
		case 8:  UPBIT(PORTA, 3); break;
		case 9:  UPBIT(PORTA, 4); break;
		default: break;

	}

}

//=====================================================================================

void ledOn(char s)
{

	//PORTC = 0b10000000;
	//return;
	

	if (s == 0) return;

	if (s <= 8){
		PORTC = (0xff << (8 - s));
		return;
	} 


	PORTC = 0xff;
	PORTD |= (0b11111100 << (6 - (s - 8)));

	return;

}

//======================================================================================

void inIRQTimer(){
	#define CHK 10
	char channels[CHK] = {1,2,3,5,7,10,16,29,57,62};

	uint16_t s;
	static uint16_t n = CHK;

	if (n++ >= CHK)
		n = 0;


	//loggerWrite("\n\r",2);
	//loggerWrite("\n\r",2);

	s = spektrum[channels[n]] ;
	//loggerWrite("\n\r",2);
	//s /= FFT_N;
		
	if(s >= 1){
		if (s > 2){
			if (s > 4){
				if (s > 7){
					if (s > 10){
						if (s > 14){
							if ( s > 20){
								if (s > 30){
									if (s > 55){
										if ( s > 70){
											if (s > 100){
												if (s > 135){
													if (s > 155){
														if ( s > 180){
															s = 14;
														} else s = 13;
													} else s = 12;
												} else s = 11;
											} else s = 10;
										} else s = 9;
									} else s = 8;
								} else s = 7;
							} else s = 6;
						} else s = 5;
					} else s = 4;
				} else s = 3;
			} else s = 2;
		} else s = 1;
	} else s = 0;

					
	//for (m = 0; m < s; m++) loggerWrite("**",1);

	ledOff();
	setChannel(n);
	ledOn(s);


}

//======================================================================================

ISR (TIMER1_OVF_vect)
{
    inIRQTimer();
    // run timer
    TCNT1 = 65536 - 1; //  31220;
    TCCR1B = (1<<CS12 | 1<<CS10);
    TIMSK |= (1<<TOIE1);


}

//=====================================================

void initTimer()
{
	TCNT1 = 65536-1;
    TCCR1B = (1<<CS12 | 1<<CS10);
    TIMSK |= (1<<TOIE1);
    sei();


}

//=====================================================

void initLeds()
{
	PORTC = 0;
	DDRC = 0xff;

	PORTD &= ~(0b11111100);
	DDRD |= 0b11111100;

	PORTB &= ~(0b00011111);
	DDRB |= 0b00011111;

	PORTA &= ~(0b00011111);
	DDRA |= 0b00011111;

}

//=====================================================

int main (void)
{
	initTimer();
	initLeds();


	loggerInit();
	loggerWriteToMarker((LogMesT)"\r\nFFT sample program\r\n*", '*');
	loggerWriteToMarker((LogMesT)"\r\n>*", '*');			/* Prompt */
	for(;;) {
		
		capture_wave(capture, FFT_N);
				
		fft_input(capture, bfly_buff);
		fft_execute(bfly_buff);
		fft_output(bfly_buff, spektrum);
		
		_delay_ms(50);
	}
}
