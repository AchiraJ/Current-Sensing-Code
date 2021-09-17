#include <LiquidCrystal.h> //library for LCD

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13,12,11,10,9,8,7,6,5,4,3);//rs, en, d4, d5, d6, d7
#include <EEPROM.h>
 float timetaken=0;
unsigned long nowtime=0;
unsigned long starttime=0;
int a=0;
float timeafter=0;
float chargeout=0;
const int analogIn = A0; //Connect current sensor with A0 of Arduino
int mVperAmp = 100; // use 100 for 20A Module and 66 for 30A Module
int RawValue= 0;
int ACSoffset = 2500; 
float DataValue=0;
double Voltage = 0; //voltage measuring
double Amps = 0;// Current measuring
int milliamps= 0;


void Display(float chargeout,float timeafter,double Amps)
 {
 lcd.clear();//clears the display of LCD
 lcd.display();

 lcd.setCursor(0,0);
 lcd.print("T:");
 lcd.setCursor(2,0);
 lcd.print(timeafter,3);
 
 lcd.setCursor(0,1);// column then row
 lcd.print("Cap:");
 lcd.setCursor(4,1);
 lcd.print(chargeout,3);
 
 lcd.setCursor(9,0);
 lcd.print("C:");
 lcd.setCursor(11,0);
 lcd.print(Amps,3);
 

 //lcd.setCursor(13,1);
 //lcd.print("Min"); //unit for the current to be measured
// delay(2000);
  }

void setup()
{
  //baud rate
  Serial.begin(9600);//baud rate at which arduino communicates with Laptop/PC
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);  //LCD order
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hello");
  delay(100);//delay for 2 sec
  
 //lcd.clear();
 Display(chargeout,timetaken,Amps);

}

void Currentsense()
{
 RawValue = analogRead(analogIn);//reading the value from the analog pin
 Voltage = (RawValue / 1024.0) * 5000; // Gets you mV
 Amps = ((Voltage - ACSoffset) / mVperAmp);

 }
void loop() //method to run the source code repeatedly
{
 Currentsense();
if(Amps>=0.5)
{  

starttime = millis();
Serial.print("starttime =  ");Serial.println(starttime);
while(Amps>=0.5)
  { 
  nowtime = millis();
  Serial.print("Now Millis =  ");Serial.println(nowtime);
delay(1000);
  Currentsense();
  timeafter=millis();
  Serial.print("After Millis =  ");Serial.println(timeafter);
  timetaken=(timeafter - nowtime);//time is in milliseconds
  //Serial.print("time taken =  ");Serial.println(timetaken);
 //timetaken=(timeafter-nowtime);//in seconds
 Serial.print("time taken =  ");Serial.println(timetaken);
  
//  chargeout = chargeout;//the previous capacity
//  Serial.print("CAP= ");Serial.println(chargeout);
  
  float currentmAh= Amps*timetaken/3600;
  Serial.print("Current mAh= ");Serial.println(currentmAh);
  
   chargeout = chargeout + currentmAh;//instateneous discharge in mAh
   Serial.print("Capacity out (mAh)= ");Serial.println(chargeout);
    milliamps = chargeout;
    byte milli1 = milliamps & 0x00FF; 
    byte milli2 = milliamps >>8;
    int y = word(milli2,milli1);
    Serial.print( "reconfigured value "); Serial.println(y);
    EEPROM.write(1,milli1);
    EEPROM.write(2,milli2);
//    lcd.setCursor(0,0);lcd.print("Amps = ");lcd.print(Amps);
// lcd.setCursor(0,1);lcd.print("CAP= ");lcd.print(chargeout);
//float Chargeout= chargeout/1000;
// lcd.print(" ");
//lcd.print(Chargeout);
  
Display(chargeout,(timeafter-starttime)/1000,Amps);
  
//delay(2000);
 //lcd.clear();
  //Display(timetaken,chargeout,timetaken,Amps);
  //delay(1000);
  }
}

  else
  {

  byte milli1x = EEPROM.read(2);
  byte milli2x = EEPROM.read (1);
  int y = word(milli1x,milli2x);
  Serial.print("final capacity  "); Serial.println(y);
    chargeout=0;
    Display(y,(timeafter-starttime)/1000,0);
    delay(1000);
    Amps=0;
    //Currentsense();

    }
 }



 
 
