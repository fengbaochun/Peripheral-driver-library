#ifndef SOFT_IIC_H
#define SOFT_IIC_H
#include "stm32f4xx.h"
#include "./delay/core_delay.h"   



/******************************************************/
//IIC ����
//��һ��
#define SCL_PIN_0                  GPIO_PIN_0
#define SCL_GPIO_PORT_0            GPIOE                   
#define SCL_GPIO_CLK_ENABLE_0()    __GPIOE_CLK_ENABLE()

#define SDA_PIN_0                  GPIO_PIN_1
#define SDA_GPIO_PORT_0            GPIOE                   
#define SDA_GPIO_CLK_ENABLE_0()    __GPIOE_CLK_ENABLE()

//�ڶ���
#define SCL_PIN_1                  GPIO_PIN_3
#define SCL_GPIO_PORT_1            GPIOE                   
#define SCL_GPIO_CLK_ENABLE_1()    __GPIOE_CLK_ENABLE()

#define SDA_PIN_1                  GPIO_PIN_2
#define SDA_GPIO_PORT_1            GPIOE                   
#define SDA_GPIO_CLK_ENABLE_1()    __GPIOE_CLK_ENABLE()

//������
#define SCL_PIN_2 					GPIO_PIN_6
#define SCL_GPIO_PORT_2            GPIOE
#define SCL_GPIO_CLK_ENABLE_2()    __GPIOE_CLK_ENABLE()

#define SDA_PIN_2                  GPIO_PIN_5
#define SDA_GPIO_PORT_2            GPIOE              
#define SDA_GPIO_CLK_ENABLE_2()    __GPIOE_CLK_ENABLE()

//������
//#define SCL_PIN_3					GPIO_PIN_10
//#define SCL_GPIO_PORT_3            GPIOE
//#define SCL_GPIO_CLK_ENABLE_3()    __GPIOE_CLK_ENABLE()

//#define SDA_PIN_3                  GPIO_PIN_9
//#define SDA_GPIO_PORT_3            GPIOE              
//#define SDA_GPIO_CLK_ENABLE_3()    __GPIOE_CLK_ENABLE()


//IICЭ����������
#define IIC_NUM  	3
//��������
enum IIC_PIN_NAME
{
	SCL=0,
	SDA
};


/*********************************ͨ������**************************/
//IO��������
#define SDA_IN()  						Set_SDA_mode(0)	//SDA����ģʽ
#define SDA_OUT()  						Set_SDA_mode(1) 	//SDA���ģʽ

#define SET_PIN_LEVEL(gpio,pin,x)		HAL_GPIO_WritePin(gpio,pin,x)


//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 

/*********************************оƬ���**************************/

# define Slave_Addr 	0x36
# define Write_Bit 		0
# define Read_Bit 		1
# define Angle_Hight_Register_Addr 	0x0C
# define Angle_Low_Register_Addr 	0x0D

void AS5600_Init(void);
uint16_t AS5600_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
float Get_Angle(void);




#endif

