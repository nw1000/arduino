#include <IRremote.h>
#include <IRremoteInt.h>

#include <SR04.h>

#define sideDistance 6 //adjust to get the right sensistivity for how close it can get to the side
#define frontDistance 30 //adjust to get how far right

#define KEY_POWER (0xFFA25D)
#define KEY_FUNC_STOP (0xFFE21D)
#define KEY_VOL_ADD (0xFF629D)
#define KEY_FAST_BACK (0xFF22DD)
#define KEY_PAUSE (0xFF02FD)
#define KEY_FAST_FORWARD (0xFFC23D)
#define KEY_DOWN (0xFFE01F)
#define KEY_VOL_DE (0xFFA857)
#define KEY_UP (0xFF906F)
#define KEY_EQ (0xFF9867)
#define KEY_ST_REPT (0xFFB04F)
#define KEY_0 (0xFF6897)
#define KEY_1 (0xFF30CF)
#define KEY_2 (0xFF18E7)
#define KEY_3 (0xFF7A85)
#define KEY_4 (0xFF10EF)
#define KEY_5 (0xFF38C7)
#define KEY_6 (0xFF5AA5)
#define KEY_7 (0xFF42BD)
#define KEY_8 (0xFF4AB5)
#define KEY_9 (0xFF52AD)
#define KEY_REPEAT (0xFFFFFFFF)
#define KEY_NUM 21

#define leftMotorF 6
#define leftMotorB 7
#define rightMotorF 8
#define rightMotorB 9

#define TRIG_PIN 0
#define ECHO_PIN 1

#define RIGHT_TRIG_PIN 2
#define RIGHT_ECHO_PIN 3
#define LEFT_TRIG_PIN 4
#define LEFT_ECHO_PIN 5

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long distance;

SR04 right_sr04 = SR04(RIGHT_ECHO_PIN,RIGHT_TRIG_PIN);
long rightDistance;

SR04 left_sr04 = SR04(LEFT_ECHO_PIN,LEFT_TRIG_PIN);
long leftDistance;

int reciever = 10;

IRrecv irrecv(reciever);
decode_results results;

long randomNumber;

bool power = false;
int mode = 1;
bool safe = false;
bool forward = false;

bool turningRight = false;
bool turningLeft = false;

void setup() {
  //put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(leftMotorF, OUTPUT);
  pinMode(leftMotorB, OUTPUT);
  pinMode(rightMotorF, OUTPUT);
  pinMode(rightMotorB, OUTPUT);
}

void walk(){
   Serial.println("driving");
   digitalWrite(leftMotorF, HIGH);
   digitalWrite(rightMotorF, HIGH);
}

void off(){
  
  digitalWrite(leftMotorF, LOW);
  digitalWrite(rightMotorF, LOW);
  digitalWrite(leftMotorB, LOW);
  digitalWrite(rightMotorB, LOW);
}

void longWalk(){
   digitalWrite(leftMotorF, HIGH);
   digitalWrite(rightMotorF, HIGH);
   delay(400);
   digitalWrite(leftMotorF, LOW);
   digitalWrite(rightMotorF, LOW);
   
}

void longBack(){
   digitalWrite(leftMotorB, HIGH);
   digitalWrite(rightMotorB, HIGH);
   delay(400);
   digitalWrite(leftMotorB, LOW);
   digitalWrite(rightMotorB, LOW);
   
}

void slightRight(){
  off();
  digitalWrite(leftMotorB, HIGH);
  digitalWrite(rightMotorF, HIGH);
  delay(100);
  off();
}

void slightLeft(){
  off();
  digitalWrite(rightMotorB, HIGH);
  digitalWrite(leftMotorF, HIGH);
  delay(100);
  off();
}

void _Right(){
  digitalWrite(leftMotorB, HIGH);
  digitalWrite(rightMotorF, HIGH);
  delay(300);
  off();
}

void _Left(){
  digitalWrite(rightMotorB, HIGH);
  digitalWrite(leftMotorF, HIGH);
  delay(300);
  off();
}

void turnRight(){
  digitalWrite(leftMotorB, HIGH);
  digitalWrite(rightMotorF, HIGH);
  if (turningRight == false) {
    turningRight = true;
  }
  else if (rightDistance <= leftDistance) {
    turningRight = false;
    off();
  }
}

void turnLeft(){
  digitalWrite(rightMotorB, HIGH);
  digitalWrite(leftMotorF, HIGH);
  if (turningLeft == false) {
    turningLeft = true;
  }
  else if (leftDistance <= rightDistance) {
    turningLeft = false;
    off();
  }
}

void turn(){

 Serial.println("turning");
 // randomNumber = random(1, 3);
  if(turningRight == true) {
    Serial.println("turning right");
    turnRight();
  }
  else if (turningLeft == false and leftDistance <= rightDistance) {
    turnRight();
  }
  else if (turningLeft == true) {
    Serial.println("turning left");
    turnLeft();
  }
  else if (turningRight == false and rightDistance <= leftDistance) {
    turnLeft();
  }
}
  


void loop() {
  // put your main code here, to run repeatedly:
  rightDistance = right_sr04.Distance();
  distance = sr04.Distance();
  leftDistance = left_sr04.Distance();
  Serial.println(leftDistance);
  forward = false;
  Serial.println("hi");
  if (irrecv.decode(&results)){
     
     switch(results.value){
        case KEY_POWER:
                        power = !power; off(); Serial.println(power); break;
        case KEY_1:   
                        mode = 1; off(); break; 
        case KEY_2:
                        mode = 2; off(); break;
        case KEY_3:
                        mode = 3; off(); break;
        case KEY_VOL_ADD:
                         if (mode == 3){
                            longWalk();
                         }
                         else if (mode == 2){
                            forward = true;
                         }
                         break;
        case KEY_FAST_FORWARD:
                        if (mode == 3 or mode == 2){
                            _Right();
                         }break;
        case KEY_FAST_BACK:
                        if (mode == 3 or mode == 2){
                            _Left();
                         }break;
        case KEY_VOL_DE:
                        if (mode == 3 or mode == 2){
                            longBack();
                         }break;
     }
     irrecv.resume();
     }
     

  
 
  if (power ==  true){
    
    if (mode == 1){

      delay(1);

      if (distance <= frontDistance){
          Serial.println("turning");
          turn();
      }
      else if (leftDistance <= sideDistance){
          Serial.println("slight right");
          slightRight();
      }
      else if (rightDistance <= sideDistance) {
          Serial.println("slight left");
          slightLeft();
      }
      else {
          Serial.println("driving striaght");
          walk();
      }
    }
    if (mode == 2){
      if (distance >= 20 and forward == true){
        longWalk();
        
      }
      forward = false;
    }
    /*if (mode == 3){
      //write walking here
    }*/
    
   
  }

}
