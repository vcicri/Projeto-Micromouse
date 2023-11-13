#include <Wire.h>
#include <stdio.h>
#include <MPU6050_tockn.h>
#define TRIG_PIN_LEFT 17
#define ECHO_PIN_LEFT 16
#define TRIG_PIN_RIGHT 2
#define ECHO_PIN_RIGHT 15
#define TRIG_PIN_FRONT 18
#define ECHO_PIN_FRONT 5
#define MOTOR_IN_A1 32
#define MOTOR_IN_A2 33
#define MOTOR_IN_B1 10
#define MOTOR_IN_B2 11
#define MOTOR_VCC 5
#define MOTOR_MAX_SPEED 3.1415
#define MOTOR_TEST

MPU6050 mpu6050(Wire);
float yaw = 0;
float offset = 0;
int count = 0;
int samples = 15;
float duration_left_us, duration_right_us, duration_front_us;
float dist[2][30] = {                                          // [0] = Left; [1] = Right; [2] = Front;
  {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0}, 
  {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0}
  };
float distMed[2] = {0,0}; // [Leftt, Right, Front]
float speed = -3.14;

struct Motor {
  int pin1;
  int pin2;
  float vcc;
  float maxSpeed;
  
  Motor(int pPin1, int pPin2, float pVcc, float pMaxSpeed) {
    pin1 = pPin1;
    pin2 = pPin2;
    vcc = pVcc;
    maxSpeed = pMaxSpeed;
  }

  void setupMotor() {
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
  }

  void setMotorSpeed(float vel);
};

void Motor::setMotorSpeed(float vel) {
  float velocity = vel / maxSpeed;
  bool forward = velocity > 0;
  if (abs(velocity) > 1) velocity = 1;
  int pwmOUT = round(abs(velocity) * 255);
  if (forward) {  //forward
    analogWrite(pin1, pwmOUT);      
    analogWrite(pin2, 0);
  } else {  // reverse
    analogWrite(pin1, 0);
    analogWrite(pin2, pwmOUT);
  }
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
   Serial.println("ai");
   delay(50);
  }
  Serial.println("Turned left");
  return;
}

float ReadDistance(){
  delay(1000);
  digitalWrite(TRIG_PIN_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_LEFT, LOW);

  //digitalWrite(TRIG_PIN_RIGHT, HIGH);
  //delayMicroseconds(10);
  //digitalWrite(TRIG_PIN_RIGHT, LOW);

  //digitalWrite(TRIG_PIN_fRONT, HIGH);
  //delayMicroseconds(10);
  //digitalWrite(TRIG_PIN_FRONT, LOW);
  
  duration_left_us = pulseIn(ECHO_PIN_LEFT, HIGH);
  //duration_right_us = pulseIn(ECHO_PIN_RIGHT, HIGH);
  //duration_front_us = pulseIn(ECHO_PIN_FRONT, HIGH);
  
  dist[0][count] = 0.017 * duration_left_us;
  //dist[1][count] = 0.017 * duration_right_us;

  Serial.print("Left Distance: ");
  Serial.print(distMed[0]);
  Serial.println(" cm, ");

  //Serial.print("Right Distance: ");
  //Serial.print(distMed[1]);
  //Serial.println(" cm, ");

  //Serial.print("Front Distance: ");
  //Serial.print(distMed[2]);
  //Serial.println(" cm, ");
  
  count++;
  if(count == samples){
    for(int i=0;i<samples;i++){
      for (int j=0; j<1;j++){
        distMed[j] += dist[j][i];
      }
    }
    distMed[0] = distMed[0]/samples;
    //distMed[1] = distMed[1]/samples;
    count = 0;
  }
}

Motor A(MOTOR_IN_A1, MOTOR_IN_A2, MOTOR_VCC, MOTOR_MAX_SPEED);

void setup() {
  Serial.begin(9600); 
  pinMode(TRIG_PIN_LEFT, OUTPUT);
  pinMode(ECHO_PIN_LEFT, INPUT); 
  pinMode(TRIG_PIN_RIGHT, OUTPUT);
  pinMode(ECHO_PIN_RIGHT, INPUT);
  A.setupMotor();
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);          
}

void loop(){
  //if(speed > 3.14) speed = -3.14;
  //speed += 0.314;
  A.setMotorSpeed(-MOTOR_MAX_SPEED);
  //Serial.println(speed);
  //ReadDistance();
  //turnLeft();
  Serial.print("Angle: ");
  Serial.println(getAngle());
  delay(1000);
}
