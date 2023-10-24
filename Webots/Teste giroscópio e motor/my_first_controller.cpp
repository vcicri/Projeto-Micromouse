#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/Gyro.hpp>
#include <cmath>

#define TIME_STEP 64

using namespace webots;



Robot *robot = new Robot();
Motor *motor1 = robot->getMotor("motor_1");
Motor *motor2 = robot->getMotor("motor_2");
Gyro *gyro = robot->getGyro("gyro");

double orientation = 0.0;
double target = 0.0;

void updateGyrol(int timeStep){
    orientation += (timeStep / 1000.0) * gyro->getValues()[2] * 180/(3.1415);
  }

int main() {

  //const double F = 2.0;   // frequency 2 Hz
  double t = 0.0;         // elapsed simulation time
  double max_speed = 8.37758;
  double speed = 0.5 * max_speed;
  
  motor1->setPosition(INFINITY);
  motor1->setVelocity(0.0);
  motor2->setPosition(INFINITY);
  motor2->setVelocity(0.0);
  gyro->enable(TIME_STEP);

  

  while (robot->step(TIME_STEP) != -1) {
    updateGyrol(TIME_STEP);
    if(orientation > 180){
      orientation -= 360; 
    }
    
    
    motor1->setVelocity(speed);
    motor2->setVelocity(-speed);
    std::cout << "Target: " << target << "°, response: " << orientation << "°" << std::endl;
    t += (double)TIME_STEP / 1000.0;
  }

  delete robot;
  return 0;
}