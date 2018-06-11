#include <stdint.h>
#include "TouchScreen.h"
#include<Servo.h>

//----------------------------------------------------------DEFINING PINS-------------------------------------------------------------------

#define YP A0                         //4
#define XM A2                         //3
#define YM 3                          //2    
#define XP 2                          //1

//---------------------------------------------------------ERROR VARIABLES------------------------------------------------------------------

float errorX, d_errorX, prev_errorX, s_errorX, errorY, d_errorY, prev_errorY, s_errorY = 0;

//--------------------------------------------------------INITIALISE TOUCHSCREEN------------------------------------------------------------

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);


double SetpointX, InputX, OutputX = 0;
double SetpointY, InputY, OutputY = 0;

//----------------------------------------------------------INITIALISE SERVO------------------------------------------------------------------

Servo servoX;
Servo servoY;

//---------------------------------------------------SOME RANDOM SCALE TO CARTESIAN-----------------------------------------------------------

float convertX = 30.0 / 132.0;               // converts raw x values to mm. found through manual calibration
float convertY = 30.0 / 80.0;                // converts raw y values to mm. found through manual calibration

//---------------------------------------------------------PID CONSTANTS------------------------------------------------------------------

float KpX = 0.3;                                              
float KiX = 0;                                                
float KdX = 1.5;                                              
float KpY = 0.5;                                              
float KiY = 0;                                                
float KdY = 1.5;                                              

void setup() 
{
  Serial.begin(9600);
  
  servoX.attach(12);
  servoY.attach(9);
  
  OutputX = 29;
  OutputY = 18;
  
  servoX.write(OutputX);       //Set servoX to mid point
  servoY.write(OutputY);       //Set servoY to mid point  
  
  delay(1000);
}

void loop() 
{  
      setDesiredPosition();

      TSPoint p = ts.getPoint();                                    // measure actual position

      InputX = ((p.x - 534.0) * convertX);                          // read and convert X coordinate
      InputY = ((p.y - 518.0) * convertY);                          // read and convert Y coordinate
      
//---------------------------------------------------------PID FOR X DIRECTION VARIABLES-------------------------------------------------------

      errorX = InputX - SetpointX;
      
      if(errorX<10 && errorX>-10)
      errorX=0;
      
      d_errorX = errorX - prev_errorX;
      s_errorX += errorX;
      OutputX = errorX * KpX + d_errorX * KdX + s_errorX * KiX;
      prev_errorX = errorX;
      
      if (OutputX < -23)
        OutputX = -23;
      if (OutputX > 23)     
        OutputX = 23;     

//---------------------------------------------------------PID FOR X DIRECTION VARIABLES-------------------------------------------------------
      errorY = InputY - SetpointY;
      
      if(errorY<10 && errorY>-10)
      errorY=0;
      
      d_errorY = errorY - prev_errorY;
      s_errorY += errorY;
      OutputY = errorY * KpY + d_errorY * KdY + s_errorY * KiY;
      prev_errorY = errorY;
      
      if (OutputY < -23)
        OutputY = -23;
      if (OutputY > 23)
        OutputY = 23;
    
    OutputX=map(OutputX,-22,22,14,44);              
    OutputY=map(OutputY,-23,23,30,6); 

    servoX.write(OutputX);                      
    servoY.write(OutputY);                      

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

void setDesiredPosition()
{
  SetpointX = 0;
  SetpointY = 0;
}
