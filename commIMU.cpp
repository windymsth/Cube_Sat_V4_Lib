#include <Wire.h>
#include "commIMU.h"
#include "Arduino.h"



#define    MPU9250_ADDRESS            0x69
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18



void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  
  // Read Nbytes
  Wire.requestFrom(Address, Nbytes); 
  uint8_t index=0;
  while (Wire.available())
    Data[index++]=Wire.read();
}

void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

void imuInit(){
  // Set accelerometers low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,29,0x06);
  // Set gyroscope low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,26,0x06);
  
  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_250_DPS);
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_2_G);
  
  // Set by pass mode for the magnetometers
  I2CwriteByte(MPU9250_ADDRESS,0x37,0x02); 
  
  // Request continuous magnetometer measurements in 16 bits
  I2CwriteByte(MAG_ADDRESS,0x0A,0x16);   
}

  uint8_t Buf[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  uint8_t Mag[7]={0,0,0,0,0,0,0}; 
  
void commIMU::getData(){

  for(int i = 0;i < 14;i ++){
	Buf[i] = 0;
  }
  I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);	
  
  // Accelerometer
  int16_t ax=-(Buf[0]<<8 | Buf[1]);
  int16_t ay=-(Buf[2]<<8 | Buf[3]);
  int16_t az=Buf[4]<<8 | Buf[5];

  // Gyroscope
  int16_t gx=-(Buf[8]<<8 | Buf[9]);
  int16_t gy=-(Buf[10]<<8 | Buf[11]);
  int16_t gz=Buf[12]<<8 | Buf[13];
  
  // Accelerometer +- 8g
  AX = ax*0.002407; 
  AY = ay*0.002407;
  AZ = az*0.002407;  
  
  // Gyroscope +- 1000
  GX = gx*0.030518; 
  GY = gy*0.030518;
  GZ = gz*0.030518;  
  
  // Read register Status 1 and wait for the DRDY: Data Ready
  
  uint8_t ST1;


  
  I2Cread(MAG_ADDRESS,0x02,1,&ST1);

  if(ST1&0x01){
  // Read magnetometer data  
  I2Cread(MAG_ADDRESS,0x03,7,Mag);
  }

  // Create 16 bits values from 8 bits data
  
  // Magnetometer
  int16_t mx=-(Mag[3]<<8 | Mag[2]);
  int16_t my=-(Mag[1]<<8 | Mag[0]);
  int16_t mz=-(Mag[5]<<8 | Mag[4]);
  
  
  // Magnetometer  4912 / 32760 uT
  MX = mx*0.1499; 
  MY = my*0.1499;
  MZ = mz*0.1499;  

}




