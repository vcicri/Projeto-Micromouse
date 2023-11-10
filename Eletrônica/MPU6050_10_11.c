/* O código funciona basicamente criando um offset para o ângulo sempre que uma função de turn for chamada, 
 * fazendo com que o angulo começe a contar do zero. Coloquei um range para ele parar de 89.5-90.5 graus
 * Da pra mudar esse range, porém corre o risco dele passar pelo intervalo sem acionar a parada.
 * Mas como a velocidade de rotação vai ser baixa, acho que vai ser tranquilo.
 * O problema é que ele só ta pegando o ângulo durante as rotações, nao durante as linhas retas.
 * Tem que instalar a lib tockn, é só procurar nas bibliotecas do arduino por MPU6050_tockn
 */



#include <Wire.h>
#include <MPU6050_tockn.h>
MPU6050 mpu6050(Wire);
float yaw=0;
float offset = 0;
void setup() {
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  Serial.begin(9600);
  while (!Serial);
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
   delay(50);
  }
  Serial.println("Turned left");
  return;
}

void loop() {
  turnLeft();
  //Serial.println(getAngle());
  delay(50);
}
