/*
 * EEPROM_INTERNAL_program.c
 * Purpose: A driver for the internal EEPROM of AVR ATMega16/32
 * Created: 7/1/2019 8:00:46 AM
 */ 

#include "SETTINGS_interface.h"
#include "Macros.h"
#include "Std_Types.h"
#include "DIO_interface.h"
#include "INTERRUPTS_interface.h"
#include "EEPROM_INTERNAL_private.h"
#include "EEPROM_INTERNAL_interface.h"
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void EEPROM_INTERNAL_vidWriteByte(u16 u16Address, u8 u8Data) {
	/*Wait for completion of previous write*/
	while(EECR & (1<<EEWE));

	/*Put address in address register*/
	EEAR = u16Address;
	/*Put data in data register*/
	EEDR = u8Data;
	/*Preparing write operation by setting logical one to EEMWE*/
	cli();
	asm("SBI 0x1C,2");
	//SET_BIT(EECR,EEMWE);
	/*Starting write operation*/
	asm("SBI 0x1C,1");
	//SET_BIT(EECR,EEWE);
	
}

u8 EEPROM_INTERNAL_u8ReadByte(u16 u16Address) {
	/*Wait for completion of previous write*/
	while(EECR & (1<<EEWE));
	while(SPMCR & (1<<SPMEN));
	
	/*Put address of data to be read in register*/
	EEAR = u16Address;
	/*Start EEPROM read by writing EEMWE*/
	SET_BIT(EECR,EERE);
	
	/*Return data from  data register*/
	return EEDR;
}

void EEPROM_INTERNAL_vidWriteByte_test(u16 u16Address, u8 u8Data) {
	eeprom_write_byte(u16Address,u8Data);
}