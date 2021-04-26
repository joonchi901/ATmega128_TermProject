#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#define HARD 1
#define EASY 0
#define true 1
#define false 0

void init_adc();
unsigned short read_adc();
void arr_rand();
unsigned char num[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};
int led[] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
int hertz[] = {261, 293, 329, 349, 392, 440, 494, 523};
int level_select = false;
int play_select = false;
int level = 0;
int num_rand[100];
unsigned short value;
void show_level();
int rnd = 1;
int i;


int main() 
{
	DDRB |= (1 << DDB4);
	DDRC = 0xFF;
	DDRG = 0x0F;
	DDRA = 0xFF;
	
	TCNT0 = 255;
	TCNT2 = 255;
	TCNT1 = 65536 - 62500;
	EICRA = 0x00;
	EICRB = 0x0A;

	EIMSK = 0x30;
	TIMSK = 0x45;
	sei();
	
	TCCR0 = 0x01;
	TCCR1A = 0x00;
	TCCR1B = 0x04;
	TCCR2 = 0x01;

	init_adc();

	value = read_adc();
	srand(value);
	
	arr_rand(num_rand);

	for(int t = 0; t< 100; t++){
		if(num_rand[i]%8 == 0) {
				num_rand[i] = led[1];
			}
			else if(num_rand[i]%8 == 1){
				num_rand[i] = led[2];
			}
			else if(num_rand[i]%8 == 2){
				num_rand[i] = led[3];
			}
			else if(num_rand[i]%8 == 3){
				num_rand[i] = led[4];
			}
			else if(num_rand[i]%8 == 4){
				num_rand[i] = led[5];
			}
			else if(num_rand[i]%8 == 5){
				num_rand[i] = led[6];
			}
			else if(num_rand[i]%8 == 6){
				num_rand[i] = led[7];
			}
			else if(num_rand[i]%8 == 7){
				num_rand[i] = led[8];			
		}
		
	}

	while(1) {
		if(play_select == true){
			PORTA = num_rand[i];			
		}
	}
	return 0;
}

ISR(TIMER1_OVF_vect)
{
	TCNT1 = 65536 - 62500;
	i += 1;
}


ISR(TIMER2_OVF_vect)
{
	TCNT2 = 255;
	if(play_select == true){
		PORTA = num_rand[i];			
	}
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 255;
	if(play_select == false){
		if (level_select == EASY){
			PORTC = 0x79;
			PORTG = 0x08;
			_delay_ms(1);
			PORTC = 0x77;
			PORTG = 0x04;
			_delay_ms(1);
			PORTC = 0x6D;
			PORTG = 0x02;
			_delay_ms(1);
			PORTC = 0x6E;
			PORTG = 0x01;
			_delay_ms(1);
		}
		else if (level_select == HARD){
			PORTC = 0x76;
			PORTG = 0x08;
			_delay_ms(1);
			PORTC = 0x77;
			PORTG = 0x04;
			_delay_ms(1);
			PORTC = 0x50;
			PORTG = 0x02;
			_delay_ms(1);
			PORTC = 0x5E;
			PORTG = 0x01;
			_delay_ms(1);
		}
	}
	else if (play_select == true){
		if (rnd == 1){
			PORTC = num[rnd];
			PORTG = 0x08;
			_delay_ms(1);
			PORTC = 0x50;
			PORTG = 0x04;
			_delay_ms(1);
			PORTC = 0x54;
			PORTG = 0x02;
			_delay_ms(1);
			PORTC = 0x5E;
			PORTG = 0x01;
			_delay_ms(1);
		}
		else if (rnd == 2){
			PORTC = num[rnd];
			PORTG = 0x08;
			_delay_ms(1);
			PORTC = 0x50;
			PORTG = 0x04;
			_delay_ms(1);
			PORTC = 0x54;
			PORTG = 0x02;
			_delay_ms(1);
			PORTC = 0x5E;
			PORTG = 0x01;
			_delay_ms(1);
		}
	}
}


ISR(INT4_vect){
	if (play_select == false){
		if (level_select == EASY){
			level_select = HARD;
		}
		else if (level_select == HARD){
			level_select = EASY;
		}
	}
}

ISR(INT5_vect){
	if (play_select == false){
		if (level_select == EASY){
			level = EASY;
			play_select = true;
		}
		else if (level_select == HARD){
			level = HARD;
			play_select = true;
		}
	}
}

void arr_rand(int *num_rand)
{
	for(int x = 0; x < 100; x++){
		int temp = rand() % 100 + 1;
		int check = false;
		for (int y = 0; y < 100; y++){
			if (num_rand[y] == temp){
				x -= 1;
				check = true;
			}
		}
		if (check == false) {
			num_rand[x] = temp;
		}
		else check = false;
	}	
}

void init_adc()
{
	DDRF &= 0xFE;
	ADMUX = 0x60;
	ADCSR = 0x87;
}

unsigned short read_adc()
{
	unsigned char adc_low, adc_high;
	unsigned short value;
	ADCSR |= 0x40;
	while ((ADCSR & 0x10) != 0x10)
		;
	adc_low = ADCL;
	adc_high = ADCH;
	value = ((unsigned short)adc_high<<8) | ((unsigned short)adc_low);
	return value;
}
