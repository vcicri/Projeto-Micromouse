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
Gyro *gyro = robot->getGyro("gyro");
DistanceSensor *ds[3];


char dsNames[3][20] = {"distance_sensor1", "distance_sensor2", "distance_sensor3"};//esq, centro, dir
double dsValues[3];//em centímetros
double orientation = 0.0;
double target = 0, previous_target=0.0;//previous não usando pra nada ainda nesse código

void updateGyrol(int timeStep){
    orientation += (timeStep / 1000.0) * gyro->getValues()[2];
    if(orientation > M_PI){
      orientation -= 2*M_PI; 
    }
    if(orientation < -M_PI){
      orientation += 2*M_PI; 
    }
}

double selectDirection(double current_target){
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
  
  double final_direction = direction + current_target;
  if(final_direction > M_PI){
    final_direction -= 2 * M_PI;
  }else if (final_direction < -M_PI){
    final_direction += 2 * M_PI;
  }
  return final_direction; 
}

int main() {

  //const double F = 2.0;   // frequency 2 Hz
  double t = 0.0, turning_delay=0.0, reset_delay=0;         // elapsed simulation time
  double max_speed = 8.37758;
  double base_speed = 0.0;
  double right_speed=0.0, left_speed=0.0;
  double error = 0.0, sum_error=0.0;
  double Kp=15.0, Ki=0.1;
  double proportional=0.0, integral=0.0;
  double new_direction = 0.0;
  
   //fazendo o enable nos sensores
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
    //fazendo a leitura nos sensores
    for(int i = 0; i < 3; i++){
      dsValues[i] = ds[i]->getValue()/100;
    }
    
    //codicão inicial
    if(t == 0){
    std::cout << "condição inicial: " << target << std::endl;
      std::cout << "target: " << target << std::endl;
      target = selectDirection(target);
      std::cout << "target após selectDirection: " << target << std::endl;
    }
    
    
    //escolhendo o formato do PID (um pra curvas abruscas e outro pra andar em linha reta)
    if(abs(error) < 0.1){
      Kp = 15.0;
      Ki=0.0;
      base_speed = 1.0*max_speed;
    }else{
      Kp=50.0;
      Ki=0.0;
      base_speed = 0.0*max_speed;
    }
    
    //corregindo erro pra não ter mudanças bruscas
    error = target - orientation;
    if (error > M_PI) {
        error -= 2 * M_PI;
    } else if (error < -M_PI) {
        error += 2 * M_PI;
    }
    
    // Cálculo do erro integrador
    sum_error += error;
    integral = Ki * sum_error;
    
    proportional = Kp * error;
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
    
    
    if(turning_delay > 1.2){
      new_direction = selectDirection(target);
    }
    
    
    if(new_direction != target and reset_delay == 0){
      turning_delay = 0.0;
      reset_delay = 1.0;
    }
    
    if(new_direction != target and turning_delay > 1.2){
      previous_target = target;
      target = new_direction;
      reset_delay = 0.0;
      turning_delay = 0.0;
    }
    if(dsValues[1] <= 5.5 and target != new_direction){
      motor2->setVelocity(0);
      motor1->setVelocity(0);
    }else{
      motor2->setVelocity(left_speed);
      motor1->setVelocity(right_speed);
    }
    
    
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << "t: " << t << ", t_delay: " << turning_delay <<std::endl;
    std::cout << "Target: " << target << ", P_target: " << previous_target << " rad, orientation: " << orientation << " rad, new_dire: " << new_direction << " rad"<< std::endl;
    std::cout << "s1: " << dsValues[0] << ", s2: " << dsValues[1] << ", s3: " << dsValues[2] << std::endl;
    std::cout << "dif: " << abs(previous_target - target) << ", error: " << error << ", Kp: " << Kp << std::endl;
    
    t += (double)TIME_STEP / 1000.0;
    turning_delay += (double)TIME_STEP / 1000.0;
    //caminho ida e volta definido
    // if(t > 5){
      // if(t==5){
        // previous_target = target;
      // }
      // target = M_PI;
    // }
    // if(t > 10){
      // if(t==10){
        // previous_target = target;
      // }
      // target = 0;
      // t=0;
    // }
    
    //caminho circular pré definido
    // if(t > 2){
      // if(t==2){
        // previous_target = target;
      // }
      // target = M_PI/2;
    // }
    // if(t > 4){
      // if(t==4){
        // previous_target = target;
      // }
      // target = M_PI;
    // }
    // if(t > 6){
      // if(t==6){
        // previous_target = target;
      // }
      // target = -M_PI/2;
    // }
    // if(t > 8){
      // if(t==8){
        // previous_target = target;
      // }
      // target = 0;
      // t=0;
    // }
    
    
  }

  delete robot;
  return 0;
}