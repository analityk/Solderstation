#ifndef lcd_h__
#define lcd_h__

#include <avr/io.h>

#define LCD_DRV_PORT	PORTD
#define LCD_DRV_DDR		DDRD
#define RS	PIND2
#define RW	PIND3
#define EN	PIND4

#define EN_H	{ LCD_DRV_PORT |= (1<<EN); }
#define EN_L	{ LCD_DRV_PORT &=~(1<<EN); }

#define RS_L	{ LCD_DRV_PORT &=~(1<<RS); }
#define RS_H	{ LCD_DRV_PORT |= (1<<RS); }
	
#define LCD_CLR_SCR			1
#define LCD_RET_HOME		2
#define LCD_ENTRY_MODE_SET	(1<<2)
#define LCD_DISP_CONTROL	(1<<3)
#define LCD_SHIFT_OPT		(1<<4)
#define LCD_SET_CGRAM_ADDR	(1<<6)
#define LCD_SET_DDRAM_ADDR	(1<<7)

#define LCD_MOVE_RIGHT		(1<<1)
#define LCD_MOVE_LEFT		(0<<1)
#define LCD_SHIFT_EN		(1<<0)

#define LCD_DISP_ON			(1<<2)
#define LCD_DISP_SLEEP		(0<<2)
#define LCD_COURSOR_ON		(1<<1)
#define LCD_COUROSR_SLEEP	(0<<1)
#define LCD_BLINK_ON		(1<<0)
#define LCD_BLINK_OFF		(0<<0)

#define LCD_SHIFT_COURSOR_R	(1<<2)

class LCD{
private:
	void Delay(uint16_t volatile t);
public:	
	
	void WriteCGR(uint8_t data);
	

	void Command(uint8_t command);
	void Data(uint8_t data);
	void Init(void);
	void WriteString(char* str);
	void WriteChar(char c);
	void GoToFirstLine(void);
	void GoToSecondLine(void);
	
	LCD(){};
	~LCD(){};
			
};

#endif // lcd_h__
