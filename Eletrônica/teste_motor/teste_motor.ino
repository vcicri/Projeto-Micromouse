#define MOTOR_IN_A1 15
#define MOTOR_IN_A2 2
#define MOTOR_IN_B1 4
#define MOTOR_IN_B2 16
#define MOTOR_VCC 5
#define MOTOR_MAX_SPEED 3.1415

#define MOTOR_TEST

struct Motor{
   int pin1;
   int pin2;
   float vcc;
   float maxSpeed;
  Motor( int pPin1,  int pPin2,  float pVcc,  float pMaxSpeed){
   pin1 = pPin1;
   pin2 = pPin2;
   vcc = pVcc;
   maxSpeed = pMaxSpeed;
  }

  void setupMotor(){
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
  }

  void setMotorSpeed(float vel){
    float velocity = vel/maxSpeed;
    if (abs(velocity) > 1) velocity = 1;
    velocity = abs(velocity);
    if (velocity > 0){ //forward
      analogWrite(pin1, round(vcc*velocity*255));
      digitalWrite(pin2, LOW);
    } 
    else { // reverse
      digitalWrite(pin1, LOW);
      analogWrite(pin2, round(vcc*velocity*255));
    }
  }

};



#ifdef MOTOR_TEST

Motor A(MOTOR_IN_A1, MOTOR_IN_A2, MOTOR_VCC, MOTOR_MAX_SPEED);

void setup() {
  // put your setup code here, to run once:
  A.setupMotor();
}

void loop() {
  A.setMotorSpeed(3.14);
  delay(1000);

  A.setMotorSpeed(1.57);
  delay(1000);

  A.setMotorSpeed(-1.57);
  delay(1000);  

  A.setMotorSpeed(-3.14);
  delay(1000);
  // put your main code here, to run repeatedly:
}
#endif