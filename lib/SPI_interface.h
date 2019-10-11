/*
 * Author: Mohammed Ahmed Abd Al-Fattah Mahmoud
 */

#ifndef _SPI_INTERFACE_H
#define _SPI_INTERFACE_H


// SPI Configurations
#define SPI_prescaller			1	// prescaller[8]={4,16,64,128 , 2,8,32,64}
#define SPI_polarity_phase		0	// polarity_phase[4]={(LRSR) , (LRSF) , (LFSF) , (LFSR)}
// L>>leading , S>> sample, R>> Raising, F>> Falling
#define SPI_interrupt_polling	0	// 1 for interr ,, 0 for polling
#define SPI_data_ordered		1	//1 for LSB first send ,, 0 for MSB first send


#define SPI_CLK_RISING 0
#define SPI_CLK_FALLING 1

#define SPI_CLK_PHASE_SAMPLE 0
#define SPI_CLK_PHASE_SETUP 1

// functions modified by zizo
void SPI_vidInitMaster(void);
void SPI_vidInitSlave(void);
u8 SPI_u8Tranciever(u8 u8data);
void SPI_vidSend(u8 u8data);
u8 SPI_u8Recieve(void);
void SPI_vidEnableSS(void);
void SPI_vidDisableSS(void);
// will be discussed later
void SPI_vidEnableInterrupt(void);
void SPI_vidDisableInterrupt(void);

#endif

