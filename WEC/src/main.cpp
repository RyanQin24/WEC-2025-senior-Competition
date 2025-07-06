#include <Arduino.h>
#include <Servo.h>

//timing safe state variables
unsigned long prevDataTime{millis()};
unsigned long timeout{30};

volatile char buffer{};

//Servo pin definition and angle variable
const int Servo0n{10};
Servo servo0;
int servo0maxPwm{90};
int servo0minPwm{5};
int servo0Pwm{5};

//Motor Controller Pin Definition
const int driverA{4};
const int driverB{5};
const int driverC{6};
const int driverD{7};

const int testled{13};

//function declarations
void motoroff();
void forward();
void reverse();
void left();
void right();
void sequence(int seq);

void setup() {
  //Pin and Serial Communication Initialization
  Serial.begin(9600);
  pinMode(driverA,OUTPUT);
  pinMode(driverB,OUTPUT);
  pinMode(driverC,OUTPUT);
  pinMode(driverD,OUTPUT);
  pinMode(testled,OUTPUT);
  servo0.attach(Servo0n);

  //Safe State startup logic initialization
  servo0.write(servo0Pwm);
  digitalWrite(driverA,LOW);
  digitalWrite(driverB,LOW);
  digitalWrite(driverC,LOW);
  digitalWrite(driverD,LOW);
}

void loop() {

  //reading serial data and controlling motors
  if(Serial.available()>0){
    digitalWrite(testled,HIGH);  
    buffer = Serial.read();
    switch(buffer){
      case 'w':
        forward(); 
        break;
      case 's':
        reverse();
        break;
      case 'a':
        left();
        break;
      case 'd':
        right();
        break;
      case 'h':
        servo0Pwm = servo0maxPwm;
        servo0.write(servo0Pwm);
        break;
      case 'j':
        servo0Pwm = servo0minPwm;
        servo0.write(servo0Pwm);
        break;
    }
    prevDataTime = millis();
  }

  //timeout and no activity safe state logic
  if(millis() - prevDataTime >= timeout){
    digitalWrite(testled,LOW);
    servo0.write(servo0Pwm);
    motoroff();
  }
}

void motoroff(){
  digitalWrite(testled,LOW);
  digitalWrite(driverA,LOW);
  digitalWrite(driverB,LOW);
  digitalWrite(driverC,LOW);
  digitalWrite(driverD,LOW);
}

void forward(){
  digitalWrite(driverA,HIGH);
  digitalWrite(driverB,LOW);
  digitalWrite(driverC,HIGH);
  digitalWrite(driverD,LOW);
}

void reverse(){
  digitalWrite(driverA,LOW);
  digitalWrite(driverB,HIGH);
  digitalWrite(driverC,LOW);
  digitalWrite(driverD,HIGH);
}

void left(){
  digitalWrite(driverA,HIGH);
  digitalWrite(driverB,LOW);
  digitalWrite(driverC,LOW);
  digitalWrite(driverD,HIGH);
}

void right(){
  digitalWrite(driverA,LOW);
  digitalWrite(driverB,HIGH);
  digitalWrite(driverC,HIGH);
  digitalWrite(driverD,LOW);
}
  