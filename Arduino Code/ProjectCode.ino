#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <LiquidCrystal.h> //--> Includes the LiquidCrystal Library.

LiquidCrystal lcd(2, 3, 8, 9, 10, 11); //--> Initialize LiquidCrystal with "lcd". lcd(RS,E,D4,D5,D6,D7).

byte heart1[8] = {B11111, B11111, B11111, B11111, B01111, B00111, B00011, B00001};//-------------***----****
byte heart2[8] = {B00011, B00001, B00000, B00000, B00000, B00000, B00000, B00000};//------------*************
byte heart3[8] = {B00011, B00111, B01111, B11111, B11111, B11111, B11111, B01111};//-----------***************
byte heart4[8] = {B11000, B11100, B11110, B11111, B11111, B11111, B11111, B11111};//------------*************-
byte heart5[8] = {B00011, B00111, B01111, B11111, B11111, B11111, B11111, B11111};//-------------***********--
byte heart6[8] = {B11000, B11100, B11110, B11111, B11111, B11111, B11111, B11110};//--------------*********---
byte heart7[8] = {B11000, B10000, B00000, B00000, B00000, B00000, B00000, B00000};//---------------*******----
byte heart8[8] = {B11111, B11111, B11111, B11111, B11110, B11100, B11000, B10000};//-----------------**-------
 
#define REPORTING_PERIOD_MS     1000
#define TempPin  A0
 
PulseOximeter pox;
uint32_t tsLastReport = 0;
float ATemp = 0;
int Temp = 0 ;
 
void setup()
{
  lcd.begin(16, 2); //--> Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display   
  lcd.setCursor(15,0); 
  lcd.createChar(1, heart1);
  lcd.createChar(2, heart2);
  lcd.createChar(3, heart3);
  lcd.createChar(4, heart4);
  lcd.createChar(5, heart5);
  lcd.createChar(6, heart6);
  lcd.createChar(7, heart7);
  lcd.createChar(8, heart8);
  lcd.setCursor(0,0); 
  lcd.print("H: "); 
  lcd.setCursor(0,1);
  lcd.print("O: ");
  lcd.setCursor(8,1);
  lcd.print("T: ");
  pinMode(A0 , INPUT);
  Serial.begin(9600);
  Serial.print("Initializing pulse oximeter..");
 
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()) 
    {
        Serial.println("FAILED");
        lcd.clear();
        lcd.setCursor(0,3);
        lcd.print("System Fail");
        while(1);
    } else 
    {
        Serial.println("SUCCESS");
    }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
     pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{
    pox.update();
    ATemp = analogRead(A0);
    ATemp = ((5 * ATemp)/1023) ;
    ATemp = ATemp * 100 ;
    Temp = ATemp ;
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
    {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");

        lcd.setCursor(0,4);
        lcd.print(pox.getHeartRate());
        lcd.setCursor(1,4);
        lcd.print(pox.getSpO2());
        lcd.setCursor(1,11);
        lcd.print(Temp);
 
        tsLastReport = millis();
    }
}


void onBeatDetected()
{
    Serial.println("Beat!");
}
 
