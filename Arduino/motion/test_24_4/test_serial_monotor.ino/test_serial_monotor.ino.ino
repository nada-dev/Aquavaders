#include <Wire.h>
#include "Servo.h"

//mafhoooom
#define pinA 2 // check if these pins are for serial 
#define pinB 3
#define pinC 4
#define pinD 5
#define pinE 6
#define pinF 7
#define LED 13



//SERIAL
char motorBit;
#define MAX_BUF 128//-------------------------------------why 128?? 
char buffer[MAX_BUF];
Servo A, B, C, D, E, F;
int motorvalA = 1500, motorvalB = 1500, motorvalC = 1500, motorvalD = 1500, motorvalE = 1500, motorvalF = 1500;
//END OF SERIAL

//declaration end

//Functions
void motorsValues(String inp)  //  this void required to separate the string data into motor values
{
  //MOTION

  motorvalA = (inp.substring(inp.indexOf('A') + 1, inp.indexOf('B'))).toInt(); 
  motorvalB = (inp.substring(inp.indexOf('B') + 1, inp.indexOf('C'))).toInt(); 
  motorvalC = (inp.substring(inp.indexOf('C') + 1, inp.indexOf('D'))).toInt(); 
  motorvalD = (inp.substring(inp.indexOf('D') + 1, inp.indexOf('E'))).toInt(); 
  motorvalE = (inp.substring(inp.indexOf('E') + 1, inp.indexOf('F'))).toInt(); 
  motorvalF = (inp.substring(inp.indexOf('F') + 1, inp.indexOf('$'))).toInt(); 

}

void moveROV()
{
  if(motorvalA > 1900 || motorvalA < 1100)
      motorvalA = 1500;
  if(motorvalB > 1900 || motorvalB < 1100)
      motorvalB = 1500;
  if(motorvalC > 1900 || motorvalC < 1100)
      motorvalC = 1500;
  if(motorvalD > 1900 || motorvalD < 1100)
      motorvalD = 1500;
  if(motorvalE > 1900 || motorvalE < 1100)
      motorvalE = 1500;
  if(motorvalF > 1900 || motorvalF < 1100)
      motorvalF = 1500;

  Serial.print("A = ");
  Serial.println(motorvalA);
  A.writeMicroseconds (motorvalA);
  B.writeMicroseconds (motorvalB);
  C.writeMicroseconds (motorvalC);
  D.writeMicroseconds (motorvalD);
  E.writeMicroseconds (motorvalE);
  F.writeMicroseconds (motorvalF);
}

void read_from_ros()
{
  int i = 0;
    while (Serial.available())
    {      
      motorBit = Serial.read();

      if (i < MAX_BUF -1)
      buffer[i++] = motorBit;
      else break;
             
      //A1500B1200C110........$
      if (motorBit == '$')
      {
        i=0;          
        motorsValues(buffer);
        moveROV();
      }
    }
}



void init_thrusters()
{
   A.attach(pinA);
   B.attach(pinB);
   C.attach(pinC);
   D.attach(pinD);
   E.attach(pinE);
   F.attach(pinF);
   A.writeMicroseconds(1500);
   B.writeMicroseconds(1500);
   C.writeMicroseconds(1500);
   D.writeMicroseconds(1500);
   E.writeMicroseconds(1500);
   F.writeMicroseconds(1500);
   delay(1000);      
   digitalWrite(LED, HIGH); //Torch Note: assign a botton from the joystick to send its signal 
}  

//end Functions




/*void setup(void) 
{
  Serial.begin(57600);
  //Serial1.begin(57600); 
   
  init_thrusters();
}


void loop(void) 
{
    // Read serially from ros and write to the motors
    read_from_ros();   
    
}*/

void setup(void) 
{
  Serial.begin(57600);
  init_thrusters();
}


void loop(void) 
{
    
    read_from_ros();  
     
    
}
