#ifndef SOFT_IIC_H
#define SOFT_IIC_H
#include "stm32f4xx.h"
#include "./delay/core_delay.h"   



/******************************************************/
//IIC 引脚
//第一组
#define SCL_PIN_0                  GPIO_PIN_0
#define SCL_GPIO_PORT_0            GPIOE                   
#define SCL_GPIO_CLK_ENABLE_0()    __GPIOE_CLK_ENABLE()

#define SDA_PIN_0                  GPIO_PIN_1
#define SDA_GPIO_PORT_0            GPIOE                   
#define SDA_GPIO_CLK_ENABLE_0()    __GPIOE_CLK_ENABLE()

//第二组
#define SCL_PIN_1                  GPIO_PIN_3
#define SCL_GPIO_PORT_1            GPIOE                   
#define SCL_GPIO_CLK_ENABLE_1()    __GPIOE_CLK_ENABLE()

#define SDA_PIN_1                  GPIO_PIN_2
#define SDA_GPIO_PORT_1            GPIOE                   
#define SDA_GPIO_CLK_ENABLE_1()    __GPIOE_CLK_ENABLE()

//第三组
#define SCL_PIN_2 					GPIO_PIN_6
#define SCL_GPIO_PORT_2            GPIOE
#define SCL_GPIO_CLK_ENABLE_2()    __GPIOE_CLK_ENABLE()

#define SDA_PIN_2                  GPIO_PIN_5
#define SDA_GPIO_PORT_2            GPIOE              
#define SDA_GPIO_CLK_ENABLE_2()    __GPIOE_CLK_ENABLE()

//第四组
//#define SCL_PIN_3					GPIO_PIN_10
//#define SCL_GPIO_PORT_3            GPIOE
//#define SCL_GPIO_CLK_ENABLE_3()    __GPIOE_CLK_ENABLE()

//#define SDA_PIN_3                  GPIO_PIN_9
//#define SDA_GPIO_PORT_3            GPIOE              
//#define SDA_GPIO_CLK_ENABLE_3()    __GPIOE_CLK_ENABLE()


//IIC协议引脚组数
#define IIC_NUM  	3
//引脚名字
enum IIC_PIN_NAME
{
	SCL=0,
	SDA
};


/*********************************通用设置**************************/
//IO方向设置
#define SDA_IN()  						Set_SDA_mode(0)	//SDA输入模式
#define SDA_OUT()  						Set_SDA_mode(1) 	//SDA输出模式

#define SET_PIN_LEVEL(gpio,pin,x)		HAL_GPIO_WritePin(gpio,pin,x)


//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 

/*********************************芯片相关**************************/

# define Slave_Addr 	0x36
# define Write_Bit 		0
# define Read_Bit 		1
# define Angle_Hight_Register_Addr 	0x0C
# define Angle_Low_Register_Addr 	0x0D

void AS5600_Init(void);
uint16_t AS5600_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
float Get_Angle(void);




#endif

