#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "MPU6050_Reg.h"
#include "math.h"

float pitch;
float roll ;
void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction )BitValue );
	Delay_us (10);
}//SCL写入的操作（主机输出SCL）

void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction )BitValue );
	Delay_us (10);
}//写入SDA（主机输出SDA）

uint8_t MyI2C_R_SDA(void)//
{
	uint8_t BitVlaue;
	BitVlaue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us (10);
	return BitVlaue;
}//主机读取SDA的数据


void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits (GPIOB,GPIO_Pin_11 | GPIO_Pin_10);
}

void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);//重复起始条件Sr时，SCL为低电平，趁此时，先确保释放SDA，再释放SCL
	MyI2C_W_SCL(1);
	
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}//产生起始条件,兼容重复起始条件Sr
  

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);//先将SDA拉低，确保后面的SDA能够产生上升沿
	MyI2C_W_SCL(1);
	
	MyI2C_W_SDA(1);
}//产生终止条件

void MyI2C_SendByte(uint8_t Byte)
{
	
	uint8_t i;
	for (i=0;i<8;i++) 
	{
	MyI2C_W_SDA(Byte&(0x80)>>i);//0x80右移i位，按位与，0x80是1000 0000，将Byte的最高位取出了
	MyI2C_W_SCL(1);//从机读取SDA的数据
	MyI2C_W_SCL(0);
	}
	
	
}//主机发送一个字节

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i,Byte = 0x00;
	MyI2C_W_SDA(1);//主机放手SDA
	for(i=0;i<8;i++)
	{
	MyI2C_W_SCL(1);
	if(MyI2C_R_SDA()==1)
	{Byte|=(0x80>>i);}
	MyI2C_W_SCL (0);
	
    }
	return Byte;
}//主机接收一个字节


void MyI2C_SendAck(uint8_t AckBit)
{
	
	
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
	
	
	
}//发送应答

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);//主机放手SDA
	MyI2C_W_SCL(1);
	AckBit=MyI2C_R_SDA ();
	MyI2C_W_SCL (0);
	
	return AckBit;
}//接收应答

#define MPU6050_ADDRESS    0xD0
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{
	MyI2C_Start ();
	MyI2C_SendByte (MPU6050_ADDRESS);
	MyI2C_ReceiveAck ();
	
	MyI2C_SendByte (RegAddress);
	MyI2C_ReceiveAck ();
	
	MyI2C_SendByte (Data);
	MyI2C_ReceiveAck ();
	
	MyI2C_Stop ();
}//指定地址写

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	MyI2C_Start ();
	MyI2C_SendByte (MPU6050_ADDRESS);
	MyI2C_ReceiveAck ();
	
	MyI2C_SendByte (RegAddress);
	MyI2C_ReceiveAck ();
	
	MyI2C_Start ();
	MyI2C_SendByte (MPU6050_ADDRESS|0x01);//将读写位变为1
	MyI2C_ReceiveAck ();
	
	Data=MyI2C_ReceiveByte ();
	MyI2C_SendAck (1);
	MyI2C_Stop ();
	
	return Data;
	
}



void MPU6050_Init(void)
{
	MyI2C_Init ();
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1	,0x01);//解除睡眠，选择陀螺仪时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2	,0x00);//6个轴都不待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV		,0x09);//10分频
	MPU6050_WriteReg(MPU6050_CONFIG		,0x06);//滤波参数最大
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG	,0x18);//陀螺仪最大量程
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG	,0x18);//加速度计最大量程
	
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg (MPU6050_WHO_AM_I);
}


void MPU6050_GetData(int16_t * AccX,int16_t * AccY,int16_t * AccZ,
								int16_t * GyroX,int16_t * GyroY,int16_t *GyroZ)
{
	
	uint8_t DataH,DataL;
	
	DataH=MPU6050_ReadReg (MPU6050_ACCEL_XOUT_H);
	DataL=MPU6050_ReadReg (MPU6050_ACCEL_XOUT_L);
	* AccX = (DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg (MPU6050_ACCEL_YOUT_H);
	DataL=MPU6050_ReadReg (MPU6050_ACCEL_YOUT_L);
	* AccY = (DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg (MPU6050_ACCEL_ZOUT_H);
	DataL=MPU6050_ReadReg (MPU6050_ACCEL_ZOUT_L);
	* AccZ = (DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg (MPU6050_GYRO_XOUT_H);
	DataL=MPU6050_ReadReg (MPU6050_GYRO_XOUT_L);
	* GyroX = (DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg (MPU6050_GYRO_YOUT_H);
	DataL=MPU6050_ReadReg (MPU6050_GYRO_YOUT_L);
	* GyroY = (DataH<<8)|DataL;
	
	DataH=MPU6050_ReadReg (MPU6050_GYRO_ZOUT_H);
	DataL=MPU6050_ReadReg (MPU6050_GYRO_ZOUT_L);
	* GyroZ = (DataH<<8)|DataL;
	
}//分别读取6个轴数据寄存器的高位和低位，拼接成16位的数据，再通过指针变量返回


void Pose_Calculating(int16_t AX,int16_t AY,int16_t AZ,int16_t GX,int16_t GY,int16_t GZ)
{
	double alpha = 0.01; // 滤波系数，取值范围在0到1之间，值越小平滑效果越强
double filtered_ax = 0, filtered_ay = 0, filtered_az = 0;


    filtered_ax= alpha * AX + (1 - alpha) * filtered_ax;
   filtered_ay = alpha *AY + (1 - alpha) * filtered_ax;
  filtered_az= alpha * AZ+ (1 - alpha) * filtered_ax;
	
		 pitch = atan2((double)filtered_ax, sqrt((double)filtered_ay *(double) filtered_ay + (double)filtered_az * (double)filtered_az))*180/3.14159;
        roll = atan2(-(double)filtered_ay, (double)filtered_az)*180/3.14159;
	
	Delay_ms (800);

}


