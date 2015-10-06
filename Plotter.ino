#include <Stepper.h>

/*
Adafruit Arduino - Lesson 16. Stepper - modified for scannerStepper with H-Bridge
*/

#include <Stepper.h>

int YPin1 = 4;
int YPin2 = 5;
int YPin3 = 6;
int YPin4 = 7;

int XPin1 = 15;
int XPin2 = 14;
int XPin3 = 16;
int XPin4 = 10;

int xFLB = 2; //Fork light barrier for the x-axis
//bool stepped = false;
int steps = 0;

double XmmSt = 0.02108333;
double YmmSt = 0.020875;

int cmd_id;

Stepper XStepper(48, XPin1, XPin2, XPin3, XPin4);
Stepper YStepper(48, YPin1, YPin2, YPin3, YPin4);

void setup()
{
  pinMode(xFLB, INPUT);
  digitalWrite(xFLB, HIGH);
  
  // this line is for Leonardo's, it delays the serial interface
  // until the terminal window is opened
  while (!Serial);

  Serial.begin(9600);
  Serial.println("Hello user");
  
  XStepper.setSpeed(632);  //Xrpm 1000 max 632,4 BaseSpeed
  YStepper.setSpeed(626);  //Yrpm 1000 max 626,3 BaseSpeed
  Serial.print("resetting X axis ...");
  resetX();
  Serial.println("[done]");
  
}

void loop()
{
  if (Serial.available())
  {
    /*steps = Serial.parseInt();
    schreite(steps);*/

    cmd_id = Serial.read();
    switch (cmd_id)
    {
      case 68:  //D(iagonale)
        Serial.print("\nD");
        if (Serial.available() > 0)
          steps = Serial.parseInt();
        Diagonale(steps);
        //Serial.print(cmd_val);
        break;

      case 69:  //ED(iagonale)
        Serial.print("\nE");
        if (Serial.available() > 0)
          steps = Serial.parseInt();
        EDiagonale(steps);
        //Serial.print(cmd_val);
        break;

      case 78:  //Nikolaushaus
        Serial.print("\nN");
        if (Serial.available() > 0)
          steps = Serial.parseInt();
        Nikolaushaus(steps);
        //Serial.print(cmd_val);
      break;

      case 88:  //X
        Serial.print("\nX");
        if (Serial.available() > 0)
          steps = Serial.parseInt();
        schreiteX(steps);
        //Serial.print(cmd_val);
        break;

      case 89:  //Y
        Serial.print("\nY");
        if (Serial.available() > 0)
          steps = Serial.parseInt();
        schreiteY(steps);
        //Serial.print(cmd_val);
        break;

      case 81:  //Q(adrat)
        Serial.print("\nQ");
        if (Serial.available() > 0)
          steps = Serial.parseInt();
        Quadrat(steps);
        break;

      default:
        Serial.print("\nERROR\n");
        break;

    }
  }



  /*if (stepped == true)
  {
    Serial.print(steps); Serial.print(" Schritte ~ "); Serial.print(steps*0.02108333); Serial.println("mm");
    stepped = false;
  }*/
}

void resetX()
{
  while(!digitalRead(xFLB))
    XStepper.step(-1);
  digitalWrite(XPin1, LOW);  digitalWrite(XPin2, LOW);  digitalWrite(XPin3, LOW);  digitalWrite(XPin4, LOW);
}

void schreiteX(int schritte)
{
  Serial.print(steps); Serial.print(" Schritte ~ "); Serial.print(steps * XmmSt); Serial.print("mm");
  XStepper.step(schritte);
  digitalWrite(XPin1, LOW);  digitalWrite(XPin2, LOW);  digitalWrite(XPin3, LOW);  digitalWrite(XPin4, LOW);
  Serial.println(" in X-Richtung gemacht");
}

void schreiteY(int schritte)
{
  Serial.print(steps); Serial.print(" Schritte ~ "); Serial.print(steps * YmmSt); Serial.print("mm");
  YStepper.step(schritte);
  digitalWrite(YPin1, LOW);  digitalWrite(YPin2, LOW);  digitalWrite(YPin3, LOW);  digitalWrite(YPin4, LOW);
  Serial.println(" in Y-Richtung gemacht");
}

void Quadrat(int seitenlaenge)
{
  Serial.println("Zeichne Quadrat");
  YStepper.step(seitenlaenge / YmmSt);
  XStepper.step(seitenlaenge / XmmSt);
  YStepper.step(seitenlaenge / YmmSt * -1); //rückwärts
  XStepper.step(seitenlaenge / XmmSt * -1); //rückwärts
  HaltStop();
  Serial.print("Quadrat mit Seitenlaenge von ");  Serial.print(seitenlaenge); Serial.print(" Schritten gezeichnet.");
}

void Nikolaushaus(int seitenlaenge)
{
  Serial.println("Zeichne Nikolaushaus");
  XStepper.step(seitenlaenge / XmmSt *-1); 
  Diagonale(seitenlaenge / XmmSt); 
  XStepper.step(seitenlaenge / XmmSt *-1); 
  Diagonale(seitenlaenge / XmmSt /2); 
  EDiagonale(seitenlaenge / XmmSt /-2); //OK
  YStepper.step(seitenlaenge / YmmSt *-1);
  EDiagonale(seitenlaenge / XmmSt);
  YStepper.step(seitenlaenge / YmmSt *-1);
  HaltStop();
  Serial.print("Nikolaushaus gezeichnet");
}

void Diagonale(int laenge)
{ //richtung f(x)
  Serial.println("Zeichne Diagonale");
  if (laenge >= 0)
  {
    for (int i = 0; i < laenge; i++)
    {
      YStepper.step(1);
      XStepper.step(1);
    }
  }
  else
  {
    laenge = laenge * -1;
    for (int i = 0; i < laenge; i++)
    {
      YStepper.step(-1);
      XStepper.step(-1);
    }
  }
  HaltStop();
}

void EDiagonale(int laenge)
{ //richtung f(-x)
  Serial.println("Zeichne Diagonale");
  if (laenge >= 0)
  {
    for (int i = laenge; i > 0; i--)
    {
      YStepper.step(1);
      XStepper.step(-1);
    }
  }
  else
  {
    laenge = laenge * -1;
    for (int i = laenge; i > 0; i--)
    {
      YStepper.step(-1);
      XStepper.step(1);
    }
  }
  HaltStop();
}

void HaltStop()
{
  digitalWrite(XPin1, LOW);  digitalWrite(XPin2, LOW);  digitalWrite(XPin3, LOW);  digitalWrite(XPin4, LOW);
  digitalWrite(YPin1, LOW);  digitalWrite(YPin2, LOW);  digitalWrite(YPin3, LOW);  digitalWrite(YPin4, LOW);
}


