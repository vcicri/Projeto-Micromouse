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

int main() {

  //const double F = 2.0;   // frequency 2 Hz
  double t = 0.0;         // elapsed simulation time
  double max_speed = 8.37758;
  double base_speed = 0.0;
  double right_speed=0.0, left_speed=0.0;
  double error = 0.0, sum_error=0.0;
  double Kp=15.0, Ki=0.1;
  double proportional=0.0, integral=0.0;
 
  
  motor1->setPosition(INFINITY);
  motor1->setVelocity(0.0);
  motor2->setPosition(INFINITY);
  motor2->setVelocity(0.0);
  gyro->enable(TIME_STEP);

  

  while (robot->step(TIME_STEP) != -1) {
    updateGyrol(TIME_STEP);
    
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
    
    
    motor2->setVelocity(left_speed);
    motor1->setVelocity(right_speed);
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << "t: " << t << std::endl;
    std::cout << "Target: " << target << ", P_target: " << previous_target << " rad, orientation: " << orientation << " rad" << std::endl;
    std::cout << "dif: " << abs(previous_target - target) << ", error: " << error << ", Kp: " << Kp << std::endl;
    
    t += (double)TIME_STEP / 1000.0;
    
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
    
    // //caminho circular pré definido
    if(t > 2){
      if(t==2){
        previous_target = target;
      }
      target = M_PI/2;
    }
    if(t > 4){
      if(t==4){
        previous_target = target;
      }
      target = M_PI;
    }
    if(t > 6){
      if(t==6){
        previous_target = target;
      }
      target = -M_PI/2;
    }
    if(t > 8){
      if(t==8){
        previous_target = target;
      }
      target = 0;
      t=0;
    }
    
    
  }

  delete robot;
  return 0;
}