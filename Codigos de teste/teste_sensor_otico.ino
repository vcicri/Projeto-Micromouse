
#define TRIG_PIN 18
#define ECHO_PIN 19
#define SERVO_PIN 14

int count =0;
float duration_us, dist[30] = {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0}, distMed=0.0;

int samples = 15;


void setup() {
  Serial.begin(115200); 
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
               
}



void ReadSensor(){    
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration_us = pulseIn(ECHO_PIN, HIGH);
  dist[count] = 0.017 * duration_us;

  Serial.print("distance: ");
  Serial.print(distMed);
  Serial.println(" cm, ");
  count++;
  if(count == samples){
    for(int i=0;i<samples;i++){
      distMed += dist[i];
    }
    distMed = distMed/samples;
    count = 0;
  }
}

void loop(){
      ReadSensor();
      delay(10);
}
