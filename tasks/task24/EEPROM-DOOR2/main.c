#include "Macros.h"
#include "Std_Types.h"
#include "DIO_interface.h"
#include "LCD_interface.h"
#include "Keypad_interface.h"
#include "Password_interface.h"
#include "Services_interface.h"
#include "TWI_interface.h"
#include "EEPROM_interface.h"
#include <util/delay.h>


void vidAsk(void);

u8 u8Repeat = 0,u8MatchFlag;
u8 u8IDMatchFlag = 0;
u8 u8Flag;
extern u8 u8IDSize;
extern u8 u8PasswordSize;
extern u8 u8PasswordStart;

int main(void) {
	LCD_vidInit();
	KEYPAD_vidInit();	
	EEPROM_vidInit();
	if(Password_u8CheckSaveFlag() == PASSWORD_UNSAVED_FLAG) {
		Password_vidEnterID();
		Password_vidEnterPassword();
		Password_vidReenterPassword();
		do {
			u8MatchFlag = Password_vidCheckPasswordMatch(u8PasswordSize);
			if (u8MatchFlag == 0) {
				Password_vidReenterPassword();
			}
		}while(u8MatchFlag == 0);

		Password_vidSave(u8IDSize);
	}
	else {	
		vidAsk();
	}
	while(1);
}

void vidAsk(void) {
	do {
		u8IDMatchFlag = Password_u8CheckID();
	}while(u8IDMatchFlag == 0);
	if (u8IDMatchFlag == 1) {
		LCD_vidSendCommand(LCD_CLEAR_SCREEN);
		u8Flag = Password_u8CheckBlock();
		if (u8Flag == PASSWORD_BLOCKED) {
			LCD_vidWriteString("Blocked");
		}
		else{
			do {
				u8MatchFlag = Password_u8CheckPassword();
				if (u8MatchFlag == 1) {
					LCD_vidSendCommand(LCD_CLEAR_SCREEN);
					LCD_vidWriteString("Welcome");
					break;
				}
				u8Repeat++;
			}while(u8Repeat < 3);
			if ((u8Repeat == 3) && (u8MatchFlag == 0)) {
				Password_vidBlockUser();
				LCD_vidSendCommand(LCD_CLEAR_SCREEN);
				LCD_vidWriteString("You're blocked");
			}
		}
	}
}
