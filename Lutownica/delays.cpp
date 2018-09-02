#include <delays.h>

void delay_us(uint8_t us){
	while(us--){
		DELAY_1us;
	};
};

void delay_10x_us(uint8_t us){
	while(us--){
		DELAY_10us;
	};
};

