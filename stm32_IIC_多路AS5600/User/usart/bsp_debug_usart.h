#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f4xx.h"
#include <stdio.h>

//////串口波特率
////#define DEBUG_USART_BAUDRATE                    115200

//////引脚定义
/////*******************************************************/
////#define DEBUG_USART                             USART1
////#define DEBUG_USART_CLK_ENABLE()                __USART1_CLK_ENABLE();

////#define RCC_PERIPHCLK_UARTx                     RCC_PERIPHCLK_USART1
////#define RCC_UARTxCLKSOURCE_SYSCLK               RCC_USART1CLKSOURCE_SYSCLK

////#define DEBUG_USART_RX_GPIO_PORT                GPIOA
////#define DEBUG_USART_RX_GPIO_CLK_ENABLE()        __GPIOB_CLK_ENABLE()
////#define DEBUG_USART_RX_PIN                      GPIO_PIN_10
////#define DEBUG_USART_RX_AF                       GPIO_AF7_USART1

////#define DEBUG_USART_TX_GPIO_PORT                GPIOA
////#define DEBUG_USART_TX_GPIO_CLK_ENABLE()        __GPIOB_CLK_ENABLE()
////#define DEBUG_USART_TX_PIN                      GPIO_PIN_9
////#define DEBUG_USART_TX_AF                       GPIO_AF7_USART1

////#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
////#define DEBUG_USART_IRQ                 		    USART1_IRQn
/////************************************************************/

#define USARTx                                 USART1
#define USARTx_BAUDRATE                        115200
#define USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART1_CLK_ENABLE()
#define USART_RCC_CLK_DISABLE()                __HAL_RCC_USART1_CLK_DISABLE()

#define USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_Tx_GPIO_PIN                     GPIO_PIN_6
#define USARTx_Tx_GPIO                         GPIOB
#define USARTx_Rx_GPIO_PIN                     GPIO_PIN_7   
#define USARTx_Rx_GPIO                         GPIOB

#define USARTx_AFx                             GPIO_AF7_USART1

#define USARTx_IRQHANDLER                      USART1_IRQHandler
#define USARTx_IRQn                            USART1_IRQn


/* 扩展变量 ------------------------------------------------------------------*/
extern UART_HandleTypeDef husartx;

/* 函数声明 ------------------------------------------------------------------*/
void MX_USARTx_Init(void);



void Usart_SendString(uint8_t *str);
void DEBUG_USART_Config(void);
//int fputc(int ch, FILE *f);
extern UART_HandleTypeDef UartHandle;
#endif /* __USART1_H */
