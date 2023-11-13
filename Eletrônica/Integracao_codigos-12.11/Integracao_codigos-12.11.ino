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
#define MOTOR_MAX_SPEED 3.1415
#define NUMBER_OF_SAMPLES 15
#define HC_SR04_DISTANCE_CONVERSION_FACTOR 17
#define MOTOR_TEST

MPU6050 mpu6050(Wire);
float yaw = 0;
float offset = 0;


struct SensorDistance{
  int pinTrigger;
  int pinEcho;
  SensorDistance(int p_pinTrigger, int p_pinEcho){
    pinTrigger = p_pinEcho;
    pinEcho = p_pinEcho;
  }
  void setup(){
    pinMode(pinTrigger, OUTPUT);
    pinMode(pinEcho, INPUT);
  }
  float readOneSample(){
    //send trigger
    digitalWrite(pinTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrigger, LOW);
    //read the duration between the pulse response.
    auto duration = pulseIn(ECHO_PIN_LEFT, HIGH);
    //adjust and return distance read.
    return duration * HC_SR04_DISTANCE_CONVERSION_FACTOR/1000.0; 
  }
  float readSampleAverageDist(int numberOfSamplesToRead){
    float distAvg = 0;
    //Accumulate distances read
    for (int i = 0; i < numberOfSamplesToRead; i++)
      distAvg += readOneSample();
    //return the average
    return distAvg/numberOfSamplesToRead;
  }
};

float speed = -3.14;

struct Motor {
  int pin1;
  int pin2;
  float vcc;
  float maxSpeed;
  
  Motor(int pPin1, int pPin2, float pMaxSpeed) {
    pin1 = pPin1;
    pin2 = pPin2;
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

Motor MotorA(MOTOR_IN_A1, MOTOR_IN_A2, MOTOR_MAX_SPEED);
SensorDistance sensorR(TRIG_PIN_RIGHT, ECHO_PIN_RIGHT); 
SensorDistance sensorL(TRIG_PIN_LEFT, ECHO_PIN_LEFT);
SensorDistance sensorF(TRIG_PIN_FRONT, ECHO_PIN_FRONT);

void setup() {
  Serial.begin(9600); 
  sensorL.setup();
  sensorR.setup();
  MotorA.setupMotor();
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);          
}

void loop(){
  //if(speed > 3.14) speed = -3.14;
  //speed += 0.314;
  MotorA.setMotorSpeed(-MOTOR_MAX_SPEED);
  //Serial.println(speed);
  auto distR = sensorR.readSampleAverageDist(NUMBER_OF_SAMPLES);
  Serial.print("Sensor Right: ");
  Serial.print (distR);
  //turnLeft();
  Serial.print(", Angle: ");
  Serial.println(getAngle());
  delay(1000);
}
