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
double target = 0;

void updateGyrol(int timeStep){
    orientation += (timeStep / 1000.0) * gyro->getValues()[2];
    if(orientation > 3.1415){
      orientation -= 2*3.1415; 
    }
    if(orientation < -3.1415){
      orientation += 2*3.1415; 
    }
  }

int main() {

  //const double F = 2.0;   // frequency 2 Hz
  double t = 0.0;         // elapsed simulation time
  double max_speed = 8.37758;
  double base_speed = 1 * max_speed;
  double right_speed=0.0, left_speed=0.0;
  double error = 0.0;
  double Kp=15.0;
  double proportional=0.0;
 
  
  motor1->setPosition(INFINITY);
  motor1->setVelocity(0.0);
  motor2->setPosition(INFINITY);
  motor2->setVelocity(0.0);
  gyro->enable(TIME_STEP);

  

  while (robot->step(TIME_STEP) != -1) {
    updateGyrol(TIME_STEP);
    
    
    error = target - orientation;
    if (error > M_PI) {
        error -= 2 * M_PI;
    } else if (error < -M_PI) {
        error += 2 * M_PI;
    }
    proportional = Kp * error;
    right_speed = base_speed + proportional;
    left_speed = base_speed - proportional;
    
    //limitando a velocidade
    if(left_speed > max_speed){
      left_speed = max_speed;
    }
    if(left_speed < -max_speed){
      left_speed = -max_speed;
    }
    if(right_speed > max_speed){
      right_speed   = max_speed;
    }
    if(right_speed < -max_speed){
      right_speed = -max_speed;
    }
    
    
    motor2->setVelocity(left_speed);
    motor1->setVelocity(right_speed);
    std::cout << "Target: " << target << " rad, response: " << orientation << " rad" << std::endl;
    t += (double)TIME_STEP / 1000.0;
    
    //caminho circular pré definido
    if(t >2.5){
      target = M_PI/2;
    }
    if(t >5){
      target = M_PI;
    }
    if(t > 7.5){
      target = -M_PI/2;
    }
    if(t > 10){
      target = 0;
      t=0;
    }
    
    
  }

  delete robot;
  return 0;
}