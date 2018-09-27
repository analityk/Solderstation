/*
 * Lutownica.cpp
 *
 * Created: 2018-09-02 16:03:38
 * Author : Szymon
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <pid.h>
#include <lcd.h>
#include <delays.h>
#include <onewire.h>

#define T0	PINC2
#define SET_T0	{ DDRC |= (1<<T0); PORTC |= (1<<T0); }
#define CLR_T0	{ DDRC |= (1<<T0); PORTC &=~(1<<T0); }

const uint16_t work_point = 650;
PID pid(0.274, 400.0, 750.0, 75.0, work_point);

uint16_t mmap(float x, float a, float b, float c, float d){
	float da = (float)(b) - (float)(a);
	float dc = (float)(d) - (float)(c);
	if( dc == 0.0 || da == 0.0 )return 0.0;
	float dd = dc / da;
	float units = (float)(x - a);
	float result = (units * dd) + (float)c;
	return (uint16_t)(result);
};

void delay(uint32_t volatile t){
	while(t--){};
};

uint16_t adc_read(uint8_t pinc){
	ADMUX = (0<<REFS1)|(1<<REFS0)|pinc;
	ADCSRA |= (1<<ADSC);
	while( ADCSRA & (1 << ADSC )){};
	uint8_t lsb = ADCL;
	uint8_t msb = ADCH;
	uint16_t t = lsb + (msb<<8);
	return t;
};

void cstr(char* t){
	for(uint8_t i=0; i<255; i++){
		if(! (((t[i] >= 48) && (t[i] <=59 )) || (t[i] == '.') || (t[i] == '-')) ){
			t[i] = '\n';
			break;
		};
	};
};

uint8_t volatile phase = 0;

uint16_t volatile drive_time = 30000;

uint16_t volatile t_adc[4];
uint16_t volatile tavg = 0;
uint8_t volatile tcnt = 0;
float volatile r = 0;
int32_t volatile drvtm1 = 0;
int32_t volatile drvtm2 = 0;

uint16_t real_tcnt1 = 0;
uint8_t volatile isr = 0;
uint16_t volatile time = 0;

ISR(TIMER1_OVF_vect){
	isr = 1;
	if( phase == 0 ){
		CLR_T0;
		while(adc_read(1) > 700){};
	
		delay(0Xff);
	
		t_adc[0] = adc_read(1);
		t_adc[1] = adc_read(1);
		t_adc[2] = adc_read(1);
		t_adc[3] = adc_read(1);
	
		tavg = t_adc[0] + t_adc[1] + t_adc[2] + t_adc[3];
		tavg /= 4;
	
		pid.Feed((float)(tavg));
		pid.Compute();
		r = pid.Output();
	
		if( r > 64000.0){
			r = 64000.0;
		};
	
		if( r < -64000.0){
			r = -64000.0;
		};
	
		time = mmap(r, -64000, 64000, 0, 64000);
	
		real_tcnt1 = time;
	
		if( time >= 64000 ){
			time = 64000;
		};
	
		if( time <= 10 ){
			time = 10;
		};
	
	
		TCNT1 = (uint16_t)(time);
		CLR_T0;
		phase = 1;
	}else{
		TCNT1 = (uint16_t)(64000 - time);
		SET_T0;
		phase = 0;
	};
	isr = 0;
};

void strcpy(char* dst, char* scr){
	uint8_t first_null = 0;
	uint8_t sec_null = 0;
	
	for(uint8_t i=0; i<253; i++){
		if( dst[i] == '\n' ){
			first_null = i;
			break;
		};
		if(i >= 254)return;
	};
	
	for(uint8_t i=0; i<253; i++){
		if( scr[i] == '\n' ){
			sec_null = i;
			break;
		};
		if(i >= 254)return;
	};
	
	uint8_t n = 0;
	for(uint8_t i = first_null; i<255; i++){
		dst[i] = scr[n];
		if( scr[n] == '\n' ){
			break;
		};
		n++;
	};
};



int main(void)
{
	LCD lcd;
	lcd.Init();

	lcd.Command(1);
	lcd.Command(2);
	lcd.Command(0x0C);

	Dallas18B20 ds18;

	char str1[20];
	char str2[20];
	char str3[20];

	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	TCCR1B = (1<<CS11)|(1<<CS10);
	TCNT1 = 0;
	TIMSK1 = (1<<TOIE1);
	
	
	uint8_t volatile preheat = 1;
	
	CLR_T0;
	uint16_t asd = 0;
	
	//while(1){
		//if(ds18.reset() == 1){
			//ds18.CallReadTemp();
			//lcd.GoToFirstLine();
			//lcd.WriteString("reset\n");
		//};
		//
		//delay(0x14ffff);
		//
		//if(ds18.reset()){
			//asd = ds18.ReadTemp() / 16;
			//itoa(asd, str1, 10);
			//cstr(str1);
			//lcd.GoToSecondLine();
			//lcd.WriteString(str1);
		//};
		//
	//};
	
	while(preheat){
		CLR_T0;
		
		while(adc_read(1) > 700){};
		
		t_adc[0] = adc_read(1);
		t_adc[1] = adc_read(1);
		t_adc[2] = adc_read(1);
		t_adc[3] = adc_read(1);
		
		tavg = t_adc[0] + t_adc[1] + t_adc[2] + t_adc[3];
		tavg /= 4;
		
		itoa(tavg, str1, 10);
		cstr(str1);
		
		lcd.GoToFirstLine();
		lcd.WriteString(str1);
		
		if(tavg >= (work_point)){
			CLR_T0;
			preheat = 0;
		}else{
			SET_T0;
			delay(20000);
		};
	};
	
	CLR_T0;
	
	sei();
	
	
	while (1)
	{
		if(isr == 0){
			
			uint16_t pr = real_tcnt1 / 640;
			uint16_t pw = real_tcnt1 / 1569;
			uint16_t tm = tavg / 1.85;
			
			itoa((uint16_t)(pr), str1, 10);
			itoa((uint16_t)(pw), str3, 10);
			itoa((uint16_t)(tm), str2, 10);
		
			cstr(str1);
			cstr(str3);
			cstr(str2);
			
			if( str1[1]=='\n'){
				str1[2] = '\n';
				str1[1] = str1[0];
				str1[0] = ' ';
			};
			
			if( str3[1]=='\n'){
				str3[2] = '\n';
				str3[1] = str3[0];
				str3[0] = ' ';
			};
			
			strcpy(str1, "% \n");
			strcpy(str1, str3);
			strcpy(str1, "W\n");
			strcpy(str2, "'C \n");
			strcpy(str2, str1);
			
			lcd.GoToFirstLine();
			lcd.WriteString(str2);
		};
		delay(0xFFFF);
	};
};
