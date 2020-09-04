#include "soft_IIC.h"
//ȫ��ʹ���ĸ�IIC������

int IIC_index=0;

//�������ݽṹ

struct GPIO_ARY
{
	GPIO_TypeDef* 	GPIOx;
	uint16_t 		GPIO_Pin;
};

typedef struct GPIO_ARY GPIO_ARY_def;

GPIO_ARY_def IIC_pin[2][IIC_NUM];

//�궨��
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


//IIC��ʼ��
void IIC_Init(void)
{
	//��������䵽������
	IIC_pin_ary_init();
	
    GPIO_InitTypeDef GPIO_Initure;
    /*********��Ӧʱ�ӳ�ʼ��************/
    SCL_GPIO_CLK_ENABLE_0();   
	SDA_GPIO_CLK_ENABLE_0();
	
    SCL_GPIO_CLK_ENABLE_1();   
	SDA_GPIO_CLK_ENABLE_1();	
	
	SCL_GPIO_CLK_ENABLE_2();
	SDA_GPIO_CLK_ENABLE_2();
	


	/*********��Ӧģʽ��ʼ��************/
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //����

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
		GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
	}
	else
	{
		GPIO_Initure.Mode=GPIO_MODE_INPUT;  //��������ģʽ
	}
	
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;    //����
	HAL_GPIO_Init(IIC_pin[SDA][IIC_index].GPIOx,&GPIO_Initure);

}



//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA(1);	  	  
	IIC_SCL(1);
	delay_us(4);
 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL(0);
	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL(1); 
	IIC_SDA(1);//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL(0);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA((txd&0x80)>>7);
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL(1);
		delay_us(2); 
		IIC_SCL(0);	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
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
	
	IIC_Send_Byte((addr<<1) | Read_Bit);//����������ַ+������
	
	IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//������,����nACK 
		else *buf=IIC_Read_Byte(1);		//������,����ACK  
		len--;
		buf++; 
	}    
	
	IIC_Stop();	//����һ��ֹͣ���� 
	
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
	
	//����˲�����ֹ���ݲ��ȶ�
	temp = temp1/20;
	
	return temp/4096*360;
	
}


/******************************MT6701**************************************************/


//��ʼ��IIC�ӿ�
void MT681X_Init(void)
{
	IIC_Init();
}
//��MT6701ָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void MT681X_WriteOneByte(uint8_t WriteAddr,uint8_t DataToWrite)
{				   	  	    																 
    IIC_Start();  
	IIC_Send_Byte((0x06<<1)+0);   //����������ַ0X06,д���� 	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr);   //���͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	//delay_ms(10);	 
}
//��MT6701ָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
uint8_t MT681X_ReadOneByte(uint16_t ReadAddr)
{				  
	uint8_t temp=0;	
	
    IIC_Start();  
	IIC_Send_Byte((0x06<<1)+0);   //����������ַ0X06,д���� 	 
	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr);   //���͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte((0x06<<1)+1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);	//////master no ack	   
    IIC_Stop();//����һ��ֹͣ����	    
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
	
	//����˲�����ֹ���ݲ��ȶ�
	temp = temp/count;
	
	return temp*max_range/16384 ;
	
}
