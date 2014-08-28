#ifndef _UART_DRIVER_
#define _UART_DRIVER_
 
#include "lpc17xx.h"
#include "mcu_regs.h"

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define UART_TX_FIFO_SIZE 16 //valor retirado de lpc177x_8x_uart.h (dos exemplos da placa)

#define BUFFERRXSIZE 32
#define BUFFERTXSIZE 64
#define SystemFrequency SystemCoreClock

//Tratamento de interrupcao da UART
void UART_IRQHandler(void);

//Inicializacao da UART
void UART_init(uint32_t baudrate);

//Função de escrita da UART
void UART_write(char c);

//Função de leitura da UART
//Retorna a quantidade de caracteres lidos
int UART_read(char* c);

//Retorna o numero de caracteres no buffer de recebimento
uint8_t UART_GetNumCharsRX();

//Retorna o numero de caracteres no buffer de envio 
uint8_t UART_GetNumCharsTX();

uint8_t UART_isbusy();

void UART_clearbusy();

#endif