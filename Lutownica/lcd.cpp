#include <lcd.h>

void LCD::Delay(uint16_t volatile t)
{
	while(t--){};
};

// io0 - io2 = pind5-pind7
// io3 - io7 = pinb0 - pinb4

void LCD::Command(uint8_t command)
{
	RS_L;
	uint8_t lsb3bit = (command & 0x07) << 5;
	PORTD &=~(0xE0);
	PORTD |= lsb3bit; 

	uint8_t msb5bit = (command & 0xF8) >> 3;
	PORTB &=~(0x1F);
	PORTB = msb5bit;
	
	EN_H;
	Delay(0xff);
	EN_L;
};

void LCD::Data(uint8_t data)
{

};

void LCD::WriteCGR(uint8_t data)
{
	RS_H;
	uint8_t lsb3bit = (data & 0x07) << 5;
	PORTD &=~(0xE0);
	PORTD |= lsb3bit;

	uint8_t msb5bit = (data & 0xF8) >> 3;
	PORTB &=~(0x1F);
	PORTB = msb5bit;
	
	EN_H;
	Delay(0xff);
	EN_L;
		
	RS_L;
};

void LCD::Init(void)
{
	LCD_DRV_DDR |= (1<<RS)|(1<<RW)|(1<<EN);
	LCD_DRV_PORT &=~((1<<RS)|(1<<RW)|(1<<EN));
	DDRD |= (1<<PIND5)|(1<<PIND6)|(1<<PIND7);
	PORTD &=~((1<<PIND5)|(1<<PIND6)|(1<<PIND7));
	
	DDRB |=(1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3)|(1<<PINB4);
	PORTB &=~((1<<PINB0)|(1<<PINB1)|(1<<PINB2)|(1<<PINB3)|(1<<PINB4));
	
	// set io
	PORTB |= (1<<PINB0)|(1<<PINB1)|(1<<PINB2);
	
	EN_H;
	Delay(0xffff);
	EN_L;
	
	// clr io
	PORTB &=~((1<<PINB1)|(1<<PINB2));
	
	
	// set io
	PORTD |=(1<<PIND6)|(1<<PIND7);
	
	EN_H;
	Delay(0xfff);
	EN_L;
	
	EN_H;
	Delay(0xfff);
	EN_L;

	// clr io
	PORTD &=~((1<<PIND6)|(1<<PIND7));
	PORTB &=~((1<<PINB0));
	
	Command(LCD_CLR_SCR);
	Command(LCD_RET_HOME);
	Command(LCD_ENTRY_MODE_SET | LCD_MOVE_RIGHT);
	Command(LCD_DISP_CONTROL | LCD_DISP_ON | LCD_COURSOR_ON | LCD_BLINK_ON);
	Command(LCD_SHIFT_OPT | LCD_SHIFT_COURSOR_R);
	Command(LCD_RET_HOME);

	
};

void LCD::WriteString(char* str)
{
	for(uint8_t i=0; i<20; i++){
		if(str[i] == '\n'){
			if( i < 20 ){
				uint8_t j = 20 - i;
				for(uint8_t n=0; n<j; n++){
					WriteCGR((uint8_t)(' '));
				};
			};
		};
		WriteCGR((uint8_t)(str[i]));
	};
};

void LCD::WriteChar(char c)
{
	
};

void LCD::GoToFirstLine(void)
{
	Command(LCD_SET_DDRAM_ADDR| 0);
};

void LCD::GoToSecondLine(void)
{
	Command(LCD_SET_DDRAM_ADDR| 40);
};

