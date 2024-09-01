#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#include "math.h"
#include "MPU6050_Reg.h"
//读写寄存器，配置寄存器，读取数据
#define MPU6050_ADDRESS    0xD0
float pitch;
float roll ;
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;
	while(I2C_CheckEvent(I2Cx,I2C_EVENT)!=SUCCESS)
	{
		Timeout--;
		if(Timeout ==0)break;
	}
}//超时退出的封装函数


void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{

	I2C_GenerateSTART(I2C2,ENABLE);//产生起始条件

	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT );//监测EV5事件
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS ,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  )!=SUCCESS);//EV6
	
	I2C_SendData(I2C2,RegAddress);
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING   )!=SUCCESS);//EV8
	
	I2C_SendData(I2C2,Data);
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS); //EV8_2  
	
	I2C_GenerateSTOP(I2C2,ENABLE);
	
	
}//指定地址写

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2,ENABLE);//产生起始条件
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT  )!=SUCCESS);//监测EV5事件
	
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS ,I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED )!=SUCCESS);//EV6
	
	I2C_SendData(I2C2,RegAddress);
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED )!=SUCCESS);//EV8
	
	I2C_GenerateSTART(I2C2,ENABLE);//产生重复起始条件
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT  )!=SUCCESS);//监测EV5事件
	
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS ,I2C_Direction_Receiver);
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED  )!=SUCCESS);//EV6
	
	I2C_AcknowledgeConfig(I2C2,DISABLE);//把Ack位设置为0
	I2C_GenerateSTOP(I2C2,ENABLE);
	
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED   )!=SUCCESS);//EV7
	Data=I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2,ENABLE);//把Ack的值改回默认值
	
	return Data;
	
}



void MPU6050_Init(void)
{
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//复用开漏输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode  = I2C_Mode_I2C ;
	I2C_InitStructure.I2C_ClockSpeed = 50000 ;
	I2C_InitStructure.I2C_DutyCycle  = I2C_DutyCycle_2 ;
	I2C_InitStructure.I2C_Ack  = I2C_Ack_Enable ;
	I2C_InitStructure.I2C_AcknowledgedAddress  =I2C_AcknowledgedAddress_7bit ;
	I2C_InitStructure.I2C_OwnAddress1  =  0x00; 
	
	I2C_Init (I2C2,&I2C_InitStructure);
	
	I2C_Cmd (I2C2,ENABLE);
	
	
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

