#include "Std_Types.h"
#include "Macros.h"
#include "DIO_interface.h"
#include "LCD_interface.h"
#include "KEYPAD_interface.h"
#include "Services_interface.h"
#include <util/delay.h>

void Services_vidWriteCharacter(void) {
	u8keyPressed = 0xFF;
	do {
		u8keyPressed = KEYPAD_u8GetKey_1();
		_delay_ms(300);
		u8keyPressed = KEYPAD_u8GetKey_1();
	}while(u8keyPressed == 0xFF);
	if (u8keyPressed == '#') {
		_delay_ms(10);
	}
	/*else {
	  if((u8keyPressed >= 0) && (u8keyPressed <= 9)) {
	  u8keyPressed += 0x30;
	  } */
	LCD_vidWriteCharacter(u8keyPressed);
}

