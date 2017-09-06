int Relay1  =  A0;
int Relay2  = "A1";
int ButtonPin = 2; //must be interrupt pin
int Encoderpin1 = 3; //must be interrupt pin
int Encoderpin2 = 4;


#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"

// LCD
// LCD D4 to Arduino 9
// LCD D5 to Arduino 10
// LCD D6 to Arduino 11
// LCD D7 to Arduino 12
// RS 4 to Arduino 7
// EN 6 to Arduino 8
// RH 5 to GND
// pin 3 to center pot
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// DS1307 RTC
// SDA pin to Arduino A4
// SCL pin to Arduino A5
RTC_DS1307 RTC;


#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(Encoderpin1, Encoderpin2);
//   avoid using pins with LEDs attached
//   用法： long Position = myEnc.read();

int var = 0;
int SetA= 0;
int SetB= 0;

bool Enable1 = 0;
int OnHour1 = 00;
int OnMinute1 = 00;
int OffHour1 = 00;
int OffMinute1 = 00;
int Panic1 = 00;

bool Enable2 = 0;
int OnHour2 = 00;
int OnMinute2 = 00;
int OffHour2 = 00;
int OffMinute2 = 00;
int Panic2 = 00;

void setup() {

  pinMode(4, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP); //Enc
  pinMode(2, INPUT_PULLUP); //Button

  pinMode(Relay1, OUTPUT); //Ch1
  digitalWrite(Relay1, LOW);
  pinMode(Relay2, OUTPUT); //Ch2
  digitalWrite(Relay2, LOW);


  Serial.begin(57600);
  Wire.begin();
  RTC.begin();



  lcd.begin(16, 2); // 16x2 LCD
  lcd.clear(); // blank the display

  attachInterrupt(digitalPinToInterrupt(ButtonPin), ButtonPressed, FALLING );// Must be perfectly debounced!!!!!!!


}

void ButtonPressed() { // this is an interrupt
  if (var > 5)
  {
    var = 1;
    Serial.println(var);
    Serial.println("var overflowed and reseted");
  }
  else
  {
    // action B
    var++;
    Serial.println(var);
    Serial.println("var added");
  }

  lcd.clear();
}

void ChangeSetA() { // this is an interrupt
SetA++;
}


void loop() {

  DateTime now = RTC.now();


  lcd.clear();// is it nessary??

  lcd.setCursor(0, 0);

  if (now.month() < 10) {
    lcd.print("0");
  }
  lcd.print(now.month(), DEC);

  lcd.print("/");


  if (now.day() < 10) {
    lcd.print("0");
  }
  lcd.print(now.day(), DEC);

  lcd.print("|");

  if (now.hour() < 10) {
    lcd.print("0");
  }

  lcd.print(now.hour(), DEC);
  lcd.print(":");
  if (now.minute() < 10) {
    lcd.print("0");
  }

  lcd.print(now.minute(), DEC);
  lcd.print(":");
  if (now.second() < 10) {
    lcd.print("0");
  }

  lcd.print(now.second(), DEC);

  lcd.print("#");
  lcd.print(var);

  switch (var) {
    case 1:
      //do something when var equals 1 display normally

      if (Enable1 == 1)
      {
        // action A
        lcd.setCursor(0, 1);
        if (OnHour1 < 10) {
          lcd.print("0");
        }
        lcd.print(OnHour1);

        if (OnMinute1 < 10) {
          lcd.print("0");
        }
        lcd.print(OnMinute1);
        if (OffHour1 < 10) {
          lcd.print("0");
        }
        lcd.print(OffHour1);
        if (OffMinute1 < 10) {
          lcd.print("0");
        }
        lcd.print(OffMinute1);
      }
      else
      {
        // action B
        lcd.setCursor(0, 1);
        lcd.print("A=Off&");
      }


      // another set

      if (Enable2 == 1)
      {
        // action A
        if (OnHour2 < 10) {
          lcd.print("0");
        }
        lcd.print(OnHour2);

        if (OnMinute2 < 10) {
          lcd.print("0");
        }
        lcd.print(OnMinute2);

        if (OffHour2 < 10) {
          lcd.print("0");
        }
        lcd.print(OffHour2);

        if (OffMinute2 < 10) {
          lcd.print("0");
        }
        lcd.print(OffMinute2);
      }
      else
      {
        // action B
        lcd.setCursor(8, 1);
        lcd.print("B=Off&");
      }


      delay(1000);
      break;

    case 2:

    detachInterrupt(digitalPinToInterrupt(Buttonpin));
    attachInterrupt(digitalPinToInterrupt(Buttonpin), ChangeSetA, FALLING);
    
      //do something when var equals 2 Set A channel
      lcd.setCursor(0, 1);
      lcd.print("A");
      if (Enable1 == 1)
      {
        // action A
        lcd.print("on |");
      }
      else
      {
        // action B
        lcd.print("off|");
      }


      if (OnHour1 < 10) {
        lcd.print("0");
      }
      lcd.print(OnHour1);
      lcd.print(":");

      if (OnMinute1 < 10) {
        lcd.print("0");
      }
      lcd.print(OnMinute1);

        lcd.print(",");
      if (OffHour1 < 10) {
        lcd.print("0");
      }
      lcd.print(OffHour1);
        lcd.print(":");
      if (OffMinute1 < 10) {
        lcd.print("0");
      }
      lcd.print(OffMinute1); 

 switch (SetA) {
    case 1:
      //do something when var equals 1
      break;
    case 2:
      //do something when var equals 2
      break;
    default: 
      detachInterrupt(digitalPinToInterrupt(Buttonpin));
      attachInterrupt(digitalPinToInterrupt(Buttonpin), Change, FALLING);
    break;
  }

      break;

    case 3:
      //do something when var equals 2 Set B channel
      lcd.setCursor(0, 1);
      lcd.print("B");
      if (Enable2 == 1)
      {
        // action A
        lcd.print("on |");
      }
      else
      {
        // action B
        lcd.print("off|");
      }


      if (OnHour2 < 10) {
        lcd.print("0");
      }
      lcd.print(OnHour2);
      lcd.print(":");
      if (OnMinute2 < 10) {
        lcd.print("0");
      }
      lcd.print(OnMinute2);
            lcd.print(",");
      if (OffHour2 < 10) {
        lcd.print("0");
      }
      lcd.print(OffHour2);
      lcd.print(":");
      if (OffMinute2 < 10) {
        lcd.print("0");
      }
      lcd.print(OffMinute2);

      delay (500);

      break;
      
    default:
      // if nothing else matches, do the default
      // default is optional
      var = 1;
      Serial.println("default var ed");
      break;
  }


}
