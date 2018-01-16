#include "dio.h"
#include "lcd.h"
#include "password.h"
#include "farah.h"
#include "motor.h"
#include <util/delay.h>


void Lcd_vidInit();
void vidChoose(void);
void calc(void);
s8 s8GetChoice(void);
void init(void);
void vidPutInEquation(u8);
void vidShowResult(void);
u8 u8Shift = 0b00010100;
s8 * s8Message;
u8 flag = 0xff;
extern u8 u8KeyPad[4][4];
extern u8 number;
extern u8 iUser;
u8 res;
void main(void)  {
	init();
	while(1) {
		vidAskPassword();		
		vidChoose();
		_delay_ms(10000);
	}
}

void vidChoose(void) {
	Lcd_vidSendCommand(LCD_CLEAR_SCREEN);
	Lcd_vidSendCommand(LCD_RETURN_HOME);
	s8 s8Choise;
	s8Message = "1. Calc 2. Motor\0";
	Lcd_vidInsertMessage(s8Message);
	Lcd_vidSendCommand(LCD_RETURN_HOME); /*If I don't add this, there will be a delay in showing the third option*/
	Lcd_vidGoToXY(0,2);
	s8Message = "3. Farah\0";
	Lcd_vidInsertMessage(s8Message);
	s8Choise = s8GetChoice();
	_delay_ms(500);
	switch (s8Choise) {
		case '1':
			calc();
			break;
		case '2':
			vidMotor();
			break;
		case '3':
			vidFarah();
			break;
		default :
			s8Message = "Invalid ans.\0";
			Lcd_vidSendCommand(LCD_CLEAR_SCREEN);
			Lcd_vidSendCommand(LCD_RETURN_HOME);
			Lcd_vidInsertMessage(s8Message);
	}
}

void init(void) {
	Dio_vidSetPortDirection(DIO_PORTD,0b11111111);
	Dio_vidSetPortDirection(DIO_PORTA,0b00001111);
	Dio_vidSetPortDirection(DIO_PORTB,0b00001111);
	Dio_vidSetPortDirection(DIO_PORTC,0b11111111);
	Dio_vidSetPortValue(DIO_PORTB,0b11110000);
	Dio_vidSetPortValue(DIO_PORTD,0b00000000);
	Lcd_vidInit();
}
s8 s8GetChoice (void) {
	while (1) {
		for (u8 r = 0; r < 4; r++) {
			Dio_vidSetPinValue(DIO_PORTB,r,0);
			for (u8 c = 4; c <= 7; c++) {
				if (Dio_u8GetPinValue(DIO_PORTB,c) == 0) {
					Lcd_vidSendCommand(LCD_CLEAR_SCREEN);
					if(u8KeyPad[c-4][r] == 'c') {
						Lcd_vidSendCommand(0b00000001); /*Clear screen*/
						number = 0;
						iUser = 0;
						break;
					}
					return u8KeyPad[c-4][r];
				}
			}
			Dio_vidSetPinValue(DIO_PORTB,r,1);
		}
	}
}

void calc() {

	while (1) {
		for (u8 r = 0; r < 4; r++) {
			Dio_vidSetPinValue(DIO_PORTB,r,0);
			for (u8 c = 4; c <= 7; c++) {
				if (Dio_u8GetPinValue(DIO_PORTB,c) == 0) {
					if(u8KeyPad[c-4][r] == 'c') {
						Lcd_vidSendCommand(LCD_CLEAR_SCREEN);						
						_delay_ms(300);
					}
					Lcd_vidWriteCharacter(u8KeyPad[c-4][r]);
					_delay_ms(500);
					vidPutInEquation(u8KeyPad[c-4][r]);
				}
			}
			Dio_vidSetPinValue(DIO_PORTB,r,1);
		}
	}
}

void vidShowResult(void) {
	Lcd_vidSendCommand(LCD_CLEAR_SCREEN);
	Lcd_vidSendCommand(LCD_RETURN_HOME);
	if (res > 9) {
		s8 b = (res/10)+48;
		s8 a = (res%10)+48;
		Lcd_vidWriteCharacter(b);
		Lcd_vidWriteCharacter(a);
	}
	else {
		s8 s8Res = res+48;
	Lcd_vidWriteCharacter(s8Res);
	}
	_delay_ms(1000);
}
void vidPutInEquation(u8 key) {
	static s32 i = 0;
	static u8 eq[4];
	u8 symbol;
	if (key == ' ') {
		symbol = eq[1];
		switch (symbol) {
			case '+':
				res = (eq[0]-48) + (eq[2]-48);
				vidShowResult();
				break;
			case '-':
				res = (eq[0]-48) - (eq[2]-48);
				vidShowResult();
				break;
			case '*':
				res = (eq[0]-48) * (eq[2]-48);
				vidShowResult();
				break;
			case '/':
				res = (eq[0]-48) / (eq[2]-48);
				vidShowResult();
				break;
			default :
				Lcd_vidSendCommand(LCD_CLEAR_SCREEN);
				Lcd_vidSendCommand(LCD_RETURN_HOME);
				s8Message = "Invalid equation\0";
				Lcd_vidBlinkMessage(s8Message,3);

				break;
		}
	}
	else {
		eq[i] = key;
		i++;
	}
}
