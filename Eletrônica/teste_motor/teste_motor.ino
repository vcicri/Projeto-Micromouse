#define MOTOR_IN_A1 5
#define MOTOR_IN_A2 6
#define MOTOR_IN_B1 10
#define MOTOR_IN_B2 11
#define MOTOR_VCC 5
#define MOTOR_MAX_SPEED 3.1415

#define MOTOR_TEST

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
  Serial.print(velocity);
  Serial.print(",");
  if (abs(velocity) > 1) velocity = 1;
  int pwmOUT = round(abs(velocity) * 255);
  Serial.print(pwmOUT);
  Serial.print(",");
  Serial.print(forward);
  Serial.print(",");
  if (forward) {  //forward
    analogWrite(pin1, pwmOUT);      
    analogWrite(pin2, 0);
  } else {  // reverse
    analogWrite(pin1, 0);
    analogWrite(pin2, pwmOUT);
  }
}

#ifdef MOTOR_TEST
Motor A(MOTOR_IN_B1, MOTOR_IN_B2, MOTOR_VCC, MOTOR_MAX_SPEED);

void setup() {
  A.setupMotor();
  Serial.begin(9600);
}
float speed = -3.14;
void loop() {
  if(speed > 3.14) speed = -3.14;
  speed += 0.314;
  A.setMotorSpeed(speed);
  Serial.println(speed);
  
  delay(1000);
}
#endif
