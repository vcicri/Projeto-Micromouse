#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/Gyro.hpp>
#include <webots/DistanceSensor.hpp>
#include <cmath>

#define TIME_STEP 64

using namespace webots;



Robot *robot = new Robot();
Motor *motor1 = robot->getMotor("motor_1");
Motor *motor2 = robot->getMotor("motor_2");
DistanceSensor *ds[3];
Gyro *gyro = robot->getGyro("gyro");

char dsNames[3][20] = {"distance_sensor1", "distance_sensor2", "distance_sensor3"};//esq, centro, dir
double dsValues[3];//em centímetros
double orientation = 0.0;
double target = 0, previous_target=0.0;//previous não usando pra nada ainda nesse código
double new_direction=0.0;

void updateGyrol(int timeStep){
    orientation += (timeStep / 1000.0) * gyro->getValues()[2];
    if(orientation > M_PI){
      orientation -= 2*M_PI; 
    }
    if(orientation < -M_PI){
      orientation += 2*M_PI; 
    }
}

double selectDirection(double current_angle){
  double direction = 0.0;
  if(dsValues[0] >= 8){
    direction = M_PI/2;
  }else if(dsValues[1] >= 8){
    direction = 0;
  }else if(dsValues[2] >= 8){
    direction = -M_PI/2;    
  }else{
    direction = M_PI;
  }
  double final_direction = direction + current_angle;
  if(final_direction > M_PI){
    final_direction -= 2 * M_PI;
  }else if (final_direction < -M_PI){
    final_direction += 2 * M_PI;
  }
  return final_direction;
  
}

int main() {

  //const double F = 2.0;   // frequency 2 Hz
  double t = 0.0;         // elapsed simulation time
  double max_speed = 8.37758;
  //double base_speed = 0.0;
  double speed=max_speed * 0.5;
 
  for(int i = 0; i < 3; i++){
    ds[i] = robot->getDistanceSensor(dsNames[i]);
    ds[i]->enable(TIME_STEP);;
  }
  
  motor1->setPosition(INFINITY);
  motor1->setVelocity(0.0);
  motor2->setPosition(INFINITY);
  motor2->setVelocity(0.0); 
  gyro->enable(TIME_STEP); 

  while (robot->step(TIME_STEP) != -1) {  
    updateGyrol(TIME_STEP);
    new_direction = selectDirection(orientation);
    motor2->setVelocity(speed);
    motor1->setVelocity(speed);
    
    for(int i = 0; i < 3; i++){
      dsValues[i] = ds[i]->getValue()/100;
    }
    
    
    
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << "t: " << t << std::endl;
    std::cout << "s1: " << dsValues[0] << ", s2: " << dsValues[1] << ", s3: " << dsValues[2] << std::endl;
    std::cout << "ori: " << orientation << ", new_direction: " << new_direction << std::endl;
    t += (double)TIME_STEP / 1000.0;
    
  }

  delete robot;
  return 0;
}