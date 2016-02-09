/*------------------------------------------------*/
/* FFTEST : A test program for FFT module         */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "uart_logger.h"		/* Defs for using Software UART module (Debugging via AVRSP-COM) */
#include "ffft.h"		/* Defs for using Fixed-point FFT module */
#include <avr/delay.h>
#include <stdio.h>

#define	SYSCLK		8000000



/*------------------------------------------------*/
/* Global variables                               */

char pool[16];	/* Console input buffer */

int16_t capture[FFT_N];			/* Wave captureing buffer */
complex_t bfly_buff[FFT_N];		/* FFT buffer */
uint16_t spektrum[FFT_N/2];		/* Spectrum output buffer */



/*------------------------------------------------*/
/* Capture waveform                               */

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


/* This is an alternative function of capture_wave() and can omit captureing buffer.

void capture_wave_inplace (complex_t *buffer, uint16_t count)
{
	const prog_int16_t *window = tbl_window;
	int16_t v;

	ADMUX = _BV(REFS0)|_BV(ADLAR)|_BV(MUX2)|_BV(MUX1)|_BV(MUX0);	// channel

	do {
		ADCSRA = _BV(ADEN)|_BV(ADSC)|_BV(ADFR)|_BV(ADIF)|_BV(ADPS2)|_BV(ADPS1);
		while(bit_is_clear(ADCSRA, ADIF));
		v = fmuls_f(ADC - 32768, pgm_read_word_near(window));
		buffer->r = v;
		buffer->i = v;
		buffer++; window++;
	} while(--count);

	ADCSRA = 0;
}
*/

/*------------------------------------------------*/
/* Online Monitor via an ISP cable                */

int main (void)
{
	char cp;
	uint16_t m, n, s;
	uint16_t t1,t2,t3;


	//DDRE = 0b00000010;	/* PE1:<conout>, PE0:<conin> in N81 38.4kbps */
	//TCCR1B = 3;	/* clk/64 */

	loggerInit();
	loggerWriteToMarker((LogMesT)"\r\nFFT sample program\r\n*", '*');
	loggerWriteToMarker((LogMesT)"\r\n>*", '*');			/* Prompt */
	for(;;) {
		
//		rcvrstr(pool, sizeof(pool));	/* Console input */
//		cp = pool;
		cp = 's';

		switch (cp) {	/* Pick a header char (command) */
			case '\0' :		/* Blank line */
				break;

			case 'w' :		/* w: show waveform */
				capture_wave(capture, FFT_N);
				for (n = 0; n < FFT_N; n++) {
					s = capture[n];
					//xmitf(PSTR("\r\n%4u:%6d "), n, s);
					s = (s + 32768) / 1024;
					//for (m = 0; m < s; m++) xmit(' ');
					//xmit('*');
				}
				break;

			case 's' :		/* s: show spectrum */
				//loggerWriteToMarker((LogMesT)"show spectrum\r\n>*", '*');
				capture_wave(capture, FFT_N);
				/*
				for(char i = 0; i < FFT_N; i++){
					char str[20];
					sprintf(str, "%d\n\r*",capture[i]);
						loggerWriteToMarker((LogMesT)str, '*');
						

				}
				*/
				
				TCNT1 = 0;	/* performance counter */
				fft_input(capture, bfly_buff);
				fft_execute(bfly_buff);
				fft_output(bfly_buff, spektrum);
				
				#define CHK 10
				char channels[CHK] = {1,2,3,5,7,10,16,29,57,62};
				//unsigned int printvals[CHK];
				loggerWrite("\n\r",2);
				loggerWrite("\n\r",2);
				for (n = 0; n < CHK ; n++) {
					s = spektrum[channels[n]] ;
					loggerWrite("\n\r",2);
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

					
					for (m = 0; m < s; m++) loggerWrite("**",1);
				}

				_delay_ms(50);
				break;

			default :		/* Unknown command */
				;//xmitstr(PSTR("\n???"));
		}
	}
}
