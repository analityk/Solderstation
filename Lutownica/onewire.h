#ifndef onewire_h__
#define onewire_h__

#include <avr/io.h>
#include <delays.h>

#define DS_PINxn	PINC0
#define DS_PINx		PINC
#define DS_PORT		PORTC
#define DS_DDR		DDRC

class Dallas18B20{
private:
	uint8_t readbyte(void);
	void writebyte(uint8_t byte);
	
	void writebit(uint8_t bit);

public:
	Dallas18B20();
	uint8_t reset(void);
	void ResetLine(void);
	void CallReadTemp(void);
	uint16_t ReadTemp(void);
	uint8_t readbit(void);

};

#endif // onewire_h__
