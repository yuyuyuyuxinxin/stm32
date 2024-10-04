#ifndef  __DMA_SERIAL_H
#define  __DMA_SERIAL_H
#include "dma.h"
#include "usart.h"
void DMA_init(void);
void processData(uint8_t data);   
void data_deal(void);
void UART_DMA_Transmit(uint8_t *pData, uint16_t Size);
void Yaw_zero(void);
typedef struct
{
	uint16_t ReceiveNum;
	uint8_t ReceiveData[512];
	uint8_t BuffTemp[512]; 
} xUart_typedef;
extern uint8_t yaw_cmd[3];
extern xUart_typedef xUart1;
extern xUart_typedef xUart2;
extern xUart_typedef xUart3;
extern xUart_typedef xUart4;
extern xUart_typedef xUart5;
extern xUart_typedef xUart6;
extern  float  Roll ;
extern  float  Pitch ;
extern  float  Yaw ;
#endif
