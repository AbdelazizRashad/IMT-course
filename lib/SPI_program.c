#include "Std_Types.h"
#include "Macros.h"
#include "DIO_interface.h"
#include "SPI_private.h"
#include "SPI_interface.h"

/******************************************************** FOR Master ******************************************************/

void SPI_vidInitMaster(void) {
	// initializing PINS
	DIO_vidSetPinDirection(DIO_PORTB,DIO_PIN5,DIO_OUTPUT); /*MOSI*/
	DIO_vidSetPinDirection(DIO_PORTB,DIO_PIN6,DIO_INPUT); /*MISO*/
	DIO_vidSetPinDirection(DIO_PORTB,DIO_PIN7,DIO_OUTPUT); /*SCK*/
	DIO_vidSetPinDirection(DIO_PORTB,DIO_PIN4,DIO_OUTPUT); /*SS*/

	/*Enable master*/
	SET_BIT(SPCR,4);
	// enable SPI (SPIE)
	SET_BIT(SPCR,6);

	// interrupt/polling (PIE PIN)
	CHANGE_BIT(SPCR, 7, SPI_interrupt_polling);
	// Data Order least
	CHANGE_BIT(SPCR,5,SPI_data_ordered);
	/* "16" prescaler	fastest one*/
	SPCR |= SPI_prescaller&(~(1<<2)) ;
	SPSR |= SPI_prescaller>>2;

	// Polarity = 0 & Phase = 1
	SPCR |= SPI_polarity_phase<<2 ;
}

/******************************************************** FOR slave ******************************************************/

void SPI_vidInitSlave(void)
{
	// initializing PINS
	DIO_vidSetPinDirection(DIO_PORTB,DIO_PIN5,DIO_INPUT); /*MOSI*/
	DIO_vidSetPinDirection(DIO_PORTB,DIO_PIN6,DIO_OUTPUT); /*MISO*/
	DIO_vidSetPinDirection(DIO_PORTB,DIO_PIN7,DIO_INPUT); /*SCK*/
	DIO_vidSetPinDirection(DIO_PORTB,DIO_PIN4,DIO_INPUT); /*SS*/

	/*Enable slave*/
	CLEAR_BIT(SPCR,4);
	// enable SPI (SPIE)
	SET_BIT(SPCR,6);

	// interrupt/polling (PIE PIN)
	CHANGE_BIT(SPCR, 7, SPI_interrupt_polling);
	// Data Order least
	CHANGE_BIT(SPCR,5,SPI_data_ordered);
	/* "16" prescaler	fastest one*/
	SPCR |= SPI_prescaller&(~(1<<2)) ;
	SPSR |= SPI_prescaller>>2;

	// Polarity = 0 & Phase = 1
	SPCR |= SPI_polarity_phase<<2 ;
}

/************************************************* FOR both master & slave ***********************************************/

u8 SPI_u8Tranciever(u8 u8data)
{
	SPDR = u8data;
	while(!(SPSR & (1<<7)));	/* wait here while SPIF flag set */
	return SPDR;
}

void SPI_vidSend(u8 u8data)
{
	SPDR = u8data;
	while(!(SPSR & (1<<7)));	/* wait here while SPIF flag set */
}

u8 SPI_u8Recieve(void)
{
	// initialing SPI registers value to enable sending
	SPDR = 0xFE;
	while(!(SPSR & (1<<7)));
	return SPDR;
}

void SPI_vidEnableSS(void) {
	DIO_vidSetPinValue(DIO_PORTB,DIO_PIN4,STD_LOW);
}

void SPI_vidDisableSS(void) {
	DIO_vidSetPinValue(DIO_PORTB,DIO_PIN4,STD_HIGH);
}

/******************************************************** FOR Interrupt ************************************************/
// next step to make this driver work with interrupt 
void SPI_vidEnableInterrupt(void) {
	SET_BIT(SPCR,7);
}

void SPI_vidDisableInterrupt(void) {
	CLEAR_BIT(SPCR,7);
}
