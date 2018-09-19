#include <onewire.h>

#define DS_PIN_SET_LOW	{ DS_DDR |= (1<<DS_PINxn); DS_PORT &=~(1<<DS_PINxn); }

void ds_pin_set_low(void){
	DS_DDR |= (1<<DS_PINxn);
	DS_PORT &=~(1<<DS_PINxn);
};

void ds_pin_set_hi(void){
	DS_DDR |= (1<<DS_PINxn);
	DS_PORT|= (1<<DS_PINxn);
};

void ds_pin_hi_z(void){
	DS_DDR &=~(1<<DS_PINxn);
	DS_PORT &=~(1<<DS_PINxn);
};

uint8_t ds_pin_state(void){
	ds_pin_set_hi();
	uint8_t t = DS_PINx;
	if( (t & (1<<DS_PINxn)) != 0){
		return 1;
	}else{
		return 0;
	};
};

uint8_t Dallas18B20::reset(void)
{
	ds_pin_set_low();
	delay_10x_us(50);
	ds_pin_hi_z();
	delay_10x_us(6);
	for(uint8_t i=0; i<30; i++){
		if( ds_pin_state() == 0 ){
			return 1;
		};
		DELAY_10us;
	};
	return 0;
};

void Dallas18B20::CallReadTemp(void)
{
	writebyte(0xCC);
	writebyte(0x44);
};

uint16_t Dallas18B20::ReadTemp(void)
{
	writebyte(0xCC);
	writebyte(0xBE);
	uint8_t msb = readbyte();
	uint8_t lsb = readbyte();
	return (msb<<8)|lsb;
};

uint8_t Dallas18B20::readbyte(void)
{
	uint8_t t = 0;
	for(uint8_t i=0; i<8; i++){
		if(readbit() == 1){
			t |= (1<<i);
		};
	};
	return t;
};

void Dallas18B20::writebyte(uint8_t byte)
{
	for(uint8_t i=0; i<8; i++){
		if( (byte &(1<<i)) != 0 ){
			writebit(1);
		}else{
			writebit(0);
		};
	};
};

uint8_t Dallas18B20::readbit(void)
{
	ds_pin_set_low();
	delay_us(5);
	ds_pin_hi_z();
	delay_us(10);
	return ds_pin_state();
};

void Dallas18B20::writebit(uint8_t bit)
{
	if(bit == 0){
		ds_pin_set_low();
		delay_10x_us(10);
		ds_pin_hi_z();
	}else{
		ds_pin_set_low();
		delay_us(30);
		ds_pin_hi_z();
		delay_10x_us(3);
	};
};

 Dallas18B20::Dallas18B20()
{
	ds_pin_hi_z();
};
