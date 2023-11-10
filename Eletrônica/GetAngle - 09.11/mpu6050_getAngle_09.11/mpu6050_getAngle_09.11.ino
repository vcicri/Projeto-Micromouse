#include <Wire.h>
#include <MPU6050_tockn.h>
MPU6050 mpu6050(Wire);
float yaw=0;
float offset = 0;
void setup() {
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  Serial.begin(9600);
  while (!Serial);
}

float getAngle(){
  mpu6050.update();
  float angleZ = mpu6050.getAngleZ();
  return(angleZ);
}

void turnLeft(){
  yaw = -9999999;
  offset = getAngle();
  while(!(yaw >89.5 && yaw < 90.5)){
   yaw = getAngle() - offset;
   Serial.println(yaw);
   delay(50);
  }
  Serial.println("Turned left");
  return;
}

void loop() {
  //turnLeft();
  Serial.println(getAngle());
  delay(50);
}
