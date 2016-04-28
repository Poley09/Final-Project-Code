#define ACCEL_CONFIG 0x1C
#define GYRO_CONFIG 0x1B
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define TEMP_OUT_H 0x41
#define GYRO_XOUT_H 0x43
#define GYRO_YOUT_H 0x45
#define GYRO_ZOUT_H 0x47
#define PWR_MGMT_1 0x6B
#define COMP_STATUS 0x02
#define COMP_XOUT_L 0x03
#define COMP_YOUT_L 0x05
#define COMP_ZOUT_L 0x07
#define ACCELRANGE_2g 0
#define ACCELRANGE_4g 1
#define ACCELRANGE_8g 2
#define ACCELRANGE_16g 3
#define GYRORANGE_250DPS 0
#define GYRORANGE_500DPS 1
#define GYRORANGE_1000DPS 2
#define GYRORANGE_2000DPS 3
#define I2CACCGYROADD 0x68 
#define I2CCOMPADD 0x0C

#include <Wire.h>

void setup()
{
  Wire.begin();  
  Serial.begin(9600);
  Init_AccelGyro(ACCELRANGE_8g, GYRORANGE_2000DPS);
}

void loop()
{
  Serial.print("Temp: ");  
  Serial.print((double)(Read_Acc_Gyro(TEMP_OUT_H) + 11900) / 340);
  Serial.print(" ");
  
  Serial.print("Acc X: "); 
  Serial.print(Read_Acc_Gyro(ACCEL_XOUT_H));
  Serial.print(" Acc Y: ");
  Serial.print(Read_Acc_Gyro(ACCEL_YOUT_H));
  Serial.print(" Acc Z: ");
  Serial.print(Read_Acc_Gyro(ACCEL_ZOUT_H));

  Serial.print(" Gyro X: ");
  Serial.print(Read_Acc_Gyro(GYRO_XOUT_H));
  Serial.print(" Gyro Y: ");
  Serial.print(Read_Acc_Gyro(GYRO_YOUT_H));
  Serial.print(" Gyro Z: ");
  Serial.print(Read_Acc_Gyro(GYRO_ZOUT_H));
  
  Init_Compass();
  
  Serial.print(" Comp X: ");
  Serial.print(Read_mag(COMP_XOUT_L));
  Serial.print(" Comp Y: ");
  Serial.print(Read_mag(COMP_YOUT_L));
  Serial.print(" Comp Z: ");
  Serial.println(Read_mag(COMP_ZOUT_L));
}

int Read_Acc_Gyro(byte axis)
{
  int Data;

  Wire.beginTransmission(I2CACCGYROADD); 
  Wire.write(axis); 
  Wire.endTransmission(); 
  Wire.requestFrom(I2CACCGYROADD, 2);
  Data = (int)Wire.read() << 8;
  Data = Data | Wire.read();
  Wire.endTransmission(); 
  
  return Data;
}

void Init_AccelGyro(byte Accel_Range, byte Gyro_Range)
{
  Wire.beginTransmission(I2CACCGYROADD);
  Wire.write(PWR_MGMT_1); 
  Wire.write(0); 
  Wire.endTransmission(); 

  Wire.beginTransmission(I2CACCGYROADD);
  Wire.write(ACCEL_CONFIG); 
  Wire.write(Accel_Range << 3); 
  Wire.endTransmission(); 

  Wire.beginTransmission(I2CACCGYROADD);
  Wire.write(GYRO_CONFIG); 
  Wire.write(Gyro_Range << 3); 
  Wire.endTransmission(); 
  
  Wire.beginTransmission(I2CACCGYROADD); 
  Wire.write(0x6A); 
  Wire.write(0x00); 
  Wire.endTransmission(true);

  Wire.beginTransmission(I2CACCGYROADD); 
  Wire.write(0x37); 
  Wire.write(0x02); 
  Wire.endTransmission(true); 
}

int Read_mag(byte axis)
{
  int Data;
 
  Wire.beginTransmission(I2CCOMPADD); 
  Wire.write(axis); 
  Wire.endTransmission(); 

  Wire.requestFrom(I2CCOMPADD, 2);
  Data = Wire.read();
  Data = Data | (int)(Wire.read() << 8);
  Wire.endTransmission(); 
  
  return Data;
}

void Init_Compass(void)
{
  Wire.beginTransmission(I2CCOMPADD); 
  Wire.write(0x0A); 
  Wire.write(0x01); 
  Wire.endTransmission(true);

  do
  {
    Wire.beginTransmission(I2CCOMPADD); 
    Wire.write(COMP_STATUS); 
    Wire.endTransmission(); 
 
    Wire.requestFrom(I2CCOMPADD, 1);
  }while(!Wire.read()); 
}
