#include <IRremote.h>
#include <IRremoteInt.h>

#include <SR04.h>

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

#define leftMotor 7
#define rightMotor 8

#define TRIG_PIN 12
#define ECHO_PIN 10

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long distance;

int reciever = 11;

IRrecv irrecv(reciever);
decode_results results;

long randomNumber;

bool power = false;
int mode = 1;
bool safe = false;
bool forward = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(leftMotor, OUTPUT);
  pinMode(rightMotor, OUTPUT);
}

void walk(){
   digitalWrite(leftMotor, HIGH);
   digitalWrite(rightMotor, HIGH);
   delay(20);
   digitalWrite(leftMotor, LOW);
   digitalWrite(rightMotor, LOW);
}

void longWalk(){
   digitalWrite(leftMotor, HIGH);
   digitalWrite(rightMotor, HIGH);
   delay(400);
   digitalWrite(leftMotor, LOW);
   digitalWrite(rightMotor, LOW);
   
}

void turnRight(){
  digitalWrite(rightMotor, HIGH);
  delay(500);
  digitalWrite(rightMotor, LOW);

}

void turnLeft(){
  digitalWrite(leftMotor, HIGH);
  delay(500);
  digitalWrite(leftMotor, LOW);

}

void turn(){

  randomNumber = random(1, 3);

  digitalWrite(leftMotor, LOW);
  digitalWrite(rightMotor, LOW);

  if(randomNumber == 1){
    turnRight();
    //Serial.println("turning right");
  }
  else if(randomNumber == 2){
    turnLeft();
    //Serial.println("turning left");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  distance = sr04.Distance();
  forward = false;
  if (irrecv.decode(&results)){
     
     switch(results.value){
        case KEY_POWER:
                        power = !power; Serial.println(power); break;
        case KEY_1:   
                        mode = 1; break;
        case KEY_2:
                        mode = 2; break;
        case KEY_3:
                        mode = 3; break;
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
                            turnRight();
                         }break;
        case KEY_FAST_BACK:
                        if (mode == 3 or mode == 2){
                            turnLeft();
                         }break;
     }
     irrecv.resume();
     } 
     

  
 
  if (power ==  true){
    
    if (mode == 1){ 
      
      //Serial.println(distance);
      delay(1);

      if (distance <= 50){
          turn();
      }
      else {
          walk();
      }
    }
    if (mode == 2){
      if (distance >= 20 and forward == true){
        longWalk();
        
      }
    }
    /*if (mode == 3){
      //write walking here
    }*/
    
   
  }

}
