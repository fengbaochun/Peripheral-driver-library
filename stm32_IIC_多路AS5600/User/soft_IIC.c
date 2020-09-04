#include "soft_IIC.h"
//全局使用哪个IIC的索引

int IIC_index=0;

//引脚数据结构

struct GPIO_ARY
{
	GPIO_TypeDef* 	GPIOx;
	uint16_t 		GPIO_Pin;
};

typedef struct GPIO_ARY GPIO_ARY_def;

GPIO_ARY_def IIC_pin[2][IIC_NUM];

//宏定义
#define SCL_GPIO_x		IIC_pin[SCL][IIC_index].GPIOx
#define SCL_GPIO_Pin	IIC_pin[SCL][IIC_index].GPIO_Pin

#define SDA_GPIO_x		IIC_pin[SDA][IIC_index].GPIOx
#define SDA_GPIO_Pin	IIC_pin[SDA][IIC_index].GPIO_Pin

void IIC_pin_ary_init()
{
	int i=0;

	IIC_pin[SCL][i].GPIOx		=	SCL_GPIO_PORT_0;	
	IIC_pin[SCL][i].GPIO_Pin	=	SCL_PIN_0;
		
	IIC_pin[SDA][i].GPIOx		=	SDA_GPIO_PORT_0;
	IIC_pin[SDA][i].GPIO_Pin	=	SDA_PIN_0;	
		
	i++;	
		
	IIC_pin[SCL][i].GPIOx		=	SCL_GPIO_PORT_1;	
	IIC_pin[SCL][i].GPIO_Pin	=	SCL_PIN_1;
		
	IIC_pin[SDA][i].GPIOx		=	SDA_GPIO_PORT_1;
	IIC_pin[SDA][i].GPIO_Pin	=	SDA_PIN_1;		
		
	i++;		
	
	IIC_pin[SCL][i].GPIOx		=	SCL_GPIO_PORT_2;	
	IIC_pin[SCL][i].GPIO_Pin	=	SCL_PIN_2;
		
	IIC_pin[SDA][i].GPIOx		=	SDA_GPIO_PORT_2;
	IIC_pin[SDA][i].GPIO_Pin	=	SDA_PIN_2;		
		
	i++;	
	

}


void IIC_SCL(int n)
{
	n?SET_PIN_LEVEL(SCL_GPIO_x,SCL_GPIO_Pin,1):\
	  SET_PIN_LEVEL(SCL_GPIO_x,SCL_GPIO_Pin,0);
}

void IIC_SDA(int n)
{
	n?SET_PIN_LEVEL(SDA_GPIO_x,SDA_GPIO_Pin,1):\
	  SET_PIN_LEVEL(SDA_GPIO_x,SDA_GPIO_Pin,0);
}

int READ_SDA()
{
	 return HAL_GPIO_ReadPin(SDA_GPIO_x,SDA_GPIO_Pin);
} 


//IIC初始化
void IIC_Init(void)
{
	//将数组填充到引脚中
	IIC_pin_ary_init();
	
    GPIO_InitTypeDef GPIO_Initure;
    /*********对应时钟初始化************/
    SCL_GPIO_CLK_ENABLE_0();   
	SDA_GPIO_CLK_ENABLE_0();
	
    SCL_GPIO_CLK_ENABLE_1();   
	SDA_GPIO_CLK_ENABLE_1();	
	
	SCL_GPIO_CLK_ENABLE_2();
	SDA_GPIO_CLK_ENABLE_2();
	


	/*********对应模式初始化************/
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //快速

	for(int i=0;i<IIC_NUM;i++)
	{
		GPIO_Initure.Pin=IIC_pin[SDA][i].GPIO_Pin;
		HAL_GPIO_Init(IIC_pin[SDA][i].GPIOx,&GPIO_Initure);
		
		GPIO_Initure.Pin=IIC_pin[SCL][i].GPIO_Pin;
		HAL_GPIO_Init(IIC_pin[SCL][i].GPIOx,&GPIO_Initure);	
	}

    
    IIC_SDA(1);
    IIC_SCL(1);  
}

void Set_SDA_mode(int mode)
{
	GPIO_InitTypeDef GPIO_Initure;

	GPIO_Initure.Pin=IIC_pin[SDA][IIC_index].GPIO_Pin;
	if(mode)
	{
		GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
	}
	else
	{
		GPIO_Initure.Mode=GPIO_MODE_INPUT;  //浮空输入模式
	}
	
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //快速
	HAL_GPIO_Init(IIC_pin[SDA][IIC_index].GPIOx,&GPIO_Initure);

}



//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA(1);	  	  
	IIC_SCL(1);
	delay_us(4);
 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL(0);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL(0);
	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL(1); 
	IIC_SDA(1);//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA(1);delay_us(1);	   
	IIC_SCL(1);delay_us(1);	 
	while(READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL(1);
		delay_us(2); 
		IIC_SCL(0);	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL(0); 
        delay_us(2);
		IIC_SCL(1);
        receive<<=1;
        if(READ_SDA())receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

/******************************AS5600**************************************************/

void AS5600_Init(void)
{
	IIC_Init();
}

uint16_t AS5600_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
	IIC_Start();
	
	IIC_Send_Byte((addr << 1) | Write_Bit);
	
	if (IIC_Wait_Ack())
	{
		IIC_Stop();
		
		return 1;
	}
	
	IIC_Send_Byte(reg);
	
	IIC_Wait_Ack();
	
	IIC_Start();
	
	IIC_Send_Byte((addr<<1) | Read_Bit);//发送器件地址+读命令
	
	IIC_Wait_Ack();		//等待应答 
	
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//读数据,发送nACK 
		else *buf=IIC_Read_Byte(1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
	
	IIC_Stop();	//产生一个停止条件 
	
	return 0;	
		
}

float Get_Angle(void)
{
	uint8_t buf[2] = {0};
	uint8_t i = 0;
	
	float temp = 0;
	float temp1 = 0.0;
	
	for (i = 0; i < 20; i++)
	{
		AS5600_Read_Len(Slave_Addr,Angle_Hight_Register_Addr,2,buf);
		
		temp1 +=buf[0]*256+buf[1];
		
		delay_ms(5);
		//temp = (((u16)buf[0] & (0x0f00)) << 8) | buf[1];
	}
	
	//软件滤波，防止数据不稳定
	temp = temp1/20;
	
	return temp/4096*360;
	
}


/******************************MT6701**************************************************/


//初始化IIC接口
void MT681X_Init(void)
{
	IIC_Init();
}
//在MT6701指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void MT681X_WriteOneByte(uint8_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
    IIC_Start();  
	IIC_Send_Byte((0x06<<1)+0);   //发送器件地址0X06,写数据 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr);   //发送地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	//delay_ms(10);	 
}
//在MT6701指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t MT681X_ReadOneByte(uint16_t ReadAddr)
{				  
	uint8_t temp=0;	
	
    IIC_Start();  
	IIC_Send_Byte((0x06<<1)+0);   //发送器件地址0X06,写数据 	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr);   //发送地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte((0x06<<1)+1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);	//////master no ack	   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}

uint8_t MT681X_Read(uint16_t ReadAddr)
{
	uint8_t data=0x50;
	
	data=MT681X_ReadOneByte(ReadAddr);
	
  return(data);
}  

void MT681X_Write(uint16_t WriteAddr,uint8_t data)
{
   MT681X_WriteOneByte(WriteAddr,data);
} 



float Get_Angle_MT(void)
{
	uint8_t i = 0;
	
	uint16_t datatemp[5]={5,6,7,8,9};
	uint32_t temp=0;
	uint16_t Angle = 0;
	
	int max_range = 4096;
	int count = 5;
	
	for (i = 0; i < count; i++)
	{
		datatemp[0]=MT681X_Read(0x03);
		datatemp[1]=MT681X_Read(0x04);
		Angle = (((datatemp[0]&0x00ff)<<8)|(datatemp[1]&0x00fc))>>2;;
		temp = temp + Angle ;
		
		delay_ms(2);
	}
	
	//软件滤波，防止数据不稳定
	temp = temp/count;
	
	return temp*max_range/16384 ;
	
}
