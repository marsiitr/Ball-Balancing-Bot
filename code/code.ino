#include <stdint.h>
#include "TouchScreen.h"
#include <SPI.h>
#include <Wire.h>
#include<Servo.h>

#define YP A0                         //4
#define XM A2                         //3
#define YM 3                          //2    
#define XP 2                          //1

int errorX, d_errorX, prev_errorX, s_errorX, errorY, d_errorY, prev_errorY, s_errorY = 0;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);       //300 is the resistance across X-plates

double k=0;
unsigned int noTouchCount = 0;                                      //variable for noTouch
double SetpointX, InputX, OutputX = 0; //for X
double SetpointY, InputY, OutputY = 0; //for Y
Servo servoX;
Servo servoY;
float convertX = 30.0 / 132.0;               // converts raw x values to mm. found through manual calibration
float convertY = 30.0 / 80.0;                // converts raw y values to mm. found through manual calibration
unsigned long Stable = 0;
float KpX = 0.3;                                              //0.3
float KiX = 0;                                                // 0
float KdX = 4.5;                                              //2.5
float KpY = 0.3;                                              //0.3
float KiY = 0;                                                //0
float KdY = 2.0;                                              //1.5
void setup() {
  Serial.begin(9600);
  servoX.attach(13);
  servoY.attach(9);
  OutputX = 32;
  OutputY = 21;
  servoX.write(OutputX);       //Set servo to mid point
  servoY.write(OutputY);      //Set servoY to mid point  
  delay(1000);
}

void loop() {
  
      setDesiredPosition();

      TSPoint p = ts.getPoint();                                    // measure actual position

      InputX = ((p.x - 534.0) * convertX);                          // read and convert X coordinate
      InputY = ((p.y - 518.0) * convertY);                          // read and convert Y coordinate
      
      errorX = InputX - SetpointX;
      if(errorX<7 && errorX>-7)
      errorX=0;
      d_errorX = errorX - prev_errorX;
      s_errorX += errorX;
      OutputX = errorX * KpX + d_errorX * KdX + s_errorX * KiX;
      prev_errorX = errorX;
      if (OutputX < -22)
        OutputX = -22;
      if (OutputX > 22)     //22
        OutputX = 22;     //22


      errorY = InputY - SetpointY;
      if(errorY<7 && errorY>-7)
      errorY=0;
      d_errorY = errorY - prev_errorY;
      s_errorY += errorY;
      OutputY = errorY * KpY + d_errorY * KdY + s_errorY * KiY;
      prev_errorY = errorY;
      if (OutputY < -50)
        OutputY = -50;
      if (OutputY > 50)
        OutputY = 50;                                                           //action control  Y compute
    
    OutputX=map(OutputX,-22,22,0,64); //46              Max Angle-65
    OutputY=map(OutputY,-50,50,42,0); //46

    servoX.write(OutputX);                      //control
    servoY.write(OutputY);                      //control

    Serial.print("( ");
    Serial.print(OutputX);
    Serial.print(",");
    Serial.print(OutputY);
    Serial.print(" )");
    Serial.print("( ");
    Serial.print(InputX);
    Serial.print(",");
    Serial.print(InputY);
    Serial.print(" )");
    Serial.print("( ");
    Serial.print(errorX);
    Serial.print(",");
    Serial.print(errorY);
    Serial.println(" )");
    delay(100);  
}

void setDesiredPosition()                               //Desired Position
{
  SetpointX = 0;
  SetpointY = 0;

//          SetpointX = 60 * cos(k);
//        SetpointY = 60 * sin(k);
//        k = k - 0.2;

}


