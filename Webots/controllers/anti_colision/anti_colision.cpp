#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>

#include <cmath>

#define TIME_STEP 64

using namespace webots;



Robot *robot = new Robot();
Motor *motor1 = robot->getMotor("motor_1");
Motor *motor2 = robot->getMotor("motor_2");
DistanceSensor *ds[3];

char dsNames[3][20] = {"distance_sensor1", "distance_sensor2", "distance_sensor3"};//esq, centro, dir
double dsValues[3];//em centímetros

int main() {

  //const double F = 2.0;   // frequency 2 Hz
  double t = 0.0;         // elapsed simulation time
  double max_speed = 8.37758;
  double base_speed = 4.0;
  double right_speed=0.0, left_speed=0.0;
  double error_sensors= 0.0, sum_error_sensors=0.0;
  double Kp=2.0, Ki=0.0;
  double proportional=0.0, integral=0.0;
 
   //fazendo o enable nos sensores
  for(int i = 0; i < 3; i++){
    ds[i] = robot->getDistanceSensor(dsNames[i]);
    ds[i]->enable(TIME_STEP);;
  }
  
  motor1->setPosition(INFINITY);
  motor1->setVelocity(0.0);
  motor2->setPosition(INFINITY);
  motor2->setVelocity(0.0);

  

  while (robot->step(TIME_STEP) != -1) {
    //fazendo a leitura nos sensores
    for(int i = 0; i < 3; i++){
      dsValues[i] = ds[i]->getValue()/100;
    }
    
    //corregindo erro pra não ter mudanças bruscas
    error_sensors = dsValues[0] - dsValues[2];
    
    // Cálculo do erro integrador
    sum_error_sensors += error_sensors;
    integral = Ki * sum_error_sensors;
    proportional = Kp * error_sensors;
    
    right_speed = base_speed + proportional + integral;
    left_speed = base_speed - proportional - integral;
    
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
    
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << "t: " << t << std::endl;
    std::cout << "s1: " << dsValues[0] << ", s2: " << dsValues[1] << ", s3: " << dsValues[2] << std::endl;
    std::cout << "dif_sensors: " << error_sensors << ", Kp: " << Kp << std::endl;
    std::cout << "left_speed: " << left_speed << ", right_speed: " << right_speed << std::endl;
    
    t += (double)TIME_STEP / 1000.0;    
  }

  delete robot;
  return 0;
}