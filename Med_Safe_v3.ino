/*
 * Medicine Safe Program
 * 2016 Bryant Webster Schultz
 * Arduino Powerd 
 */

#include <math.h>

double Thermistor(int RawADC) {
 double Temp;             // current temperature in F
 double average = 0;      // variable for temperature average
 int n = 10;              // number of temp samples to round for average
 int incomingByte = 0;   //  incoming serial data for control and data retrieval

//Subroutine to get the Temperature of thermister in series with 10K resistor
//voltage read accross thermister
 for (int i=1; i <= 10; i++){
  Temp = log(10000.0*((1024.0/RawADC-1))); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;          
  Temp = (Temp * 9.0)/ 5.0 + 32.0;
  average = Temp + average;
  delay(5); //to allow A to D to recover
 }
 Temp = average/n;
 return Temp;
}

double hightemp;  // Highest Temperature read
double lowtemp;   // Lowest Temperature read
double setpoint;  // Temperature to hold in F
int cooler = 13;  // Output Which controlls cooler
byte byteRead;    // Serial input byte read

void setup() {
 hightemp = 0;
 lowtemp = 150;
 setpoint = 80;   // Starting Temperature set high for demonstration
 pinMode(cooler, OUTPUT);
 Serial.begin(9600);
}

void loop() {             
  int val;                
  double temp;            
  val=analogRead(0);      
  temp=Thermistor(val);
  if (temp < lowtemp) lowtemp = temp;
  if (temp > hightemp) hightemp = temp;
  if (temp > setpoint) {digitalWrite(cooler, HIGH);}
  else {digitalWrite(cooler, LOW);}

  if (Serial.available() > 0) {   //Outputs information if if any byte is received
      byteRead = Serial.read();
      if (byteRead == '1')  // ASCII "1"  increase setpoint 1 degree
      {setpoint = setpoint +1;}
      else if (byteRead == '2')  // ASCII "2" decrease setpoint 1 degree
      {setpoint = setpoint -1;}
      else {}
      Serial.print("Set Temp = ");
      Serial.print(setpoint);   
      Serial.println(" F");
      Serial.print("Temperature = ");
      Serial.print(temp);   
      Serial.print(" F");
      if (temp > setpoint) {Serial.println("   Cooling On");}
      else {Serial.println("   Idle");}      
      Serial.print("hightemp = ");
      Serial.print(hightemp);   
      Serial.println(" F");
      Serial.print("lowtemp = ");
      Serial.print(lowtemp);   
      Serial.println(" F");
      Serial.println(""); 
  }     // end if(byteRead     
}

