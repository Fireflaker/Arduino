int Relay1  =  A0;
int Relay2  =  A1;

#include <ClickEncoder.h>
#include <TimerOne.h>

ClickEncoder *encoder;
int16_t last, value;

void timerIsr() {
  encoder->service();
}

#include <IRremote.h>
int RECV_PIN = 6;
decode_results results;
IRrecv irrecv(RECV_PIN);

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

int var = 0;
int SetA = 0;
int SetB = 0;
int SetPanA = 0;
int SetPanB = 0;

int Enable1 = 0;
int OnHour1 = 00;
int OnMinute1 = 00;
int OffHour1 = 00;
int OffMinute1 = 00;
int Panic1 = 00;

int Enable2 = 0;
int OnHour2 = 00;
int OnMinute2 = 00;
int OffHour2 = 00;
int OffMinute2 = 00;
int Panic2 = 00;

int change = 0;
int ToggleA = 0;
int ToggleB = 0;

int PanEn1 = 1;
int PanWait1 = 5;
int PanOn1 = 10;
int PanOff1 = 10;

int PanEn2 = 1;
int PanWait2 = 5;
int PanOn2 = 10;
int PanOff2 = 10;

int TestEn = 0;
int  NowPanic = 0;

unsigned long time1 = millis();
unsigned long time2 = millis();

int SkipPanWait1 = 0;
int SkipPanWait2 = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  encoder = new ClickEncoder(3, 4, 5, 4);                       //(R1,R2,Button,Steps Per Nutch)
  pinMode(4, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP); //Enc
  pinMode(5, INPUT_PULLUP); //Button

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  last = -1;
  pinMode(Relay1, OUTPUT); //Ch1
  digitalWrite(Relay1, LOW);
  pinMode(Relay2, OUTPUT); //Ch2
  digitalWrite(Relay2, LOW);


  Wire.begin();
  RTC.begin();

  irrecv.enableIRIn(); // Start the receiver

  lcd.begin(16, 2); // 16x2 LCD
  lcd.clear(); // blank the display
}



void loop() {                                                        //Loop

  value += encoder->getValue();                                      //Rotory Encoder values generat

  if (value != last) {
    change = value - last;
    Serial.print("Encoder change: ");
    Serial.println(change);
    last = value;
    Serial.print("Encoder Value: ");
    Serial.println(value);
  }
  else
  {
    change = 0;

  }

  if (OnHour1 > 23) {
    OnHour1 = 0; //Clean up input data
  }
  if (OnHour2 > 23) {
    OnHour2 = 0;
  }
  if (OffHour1 > 23) {
    OffHour1 = 0;
  }
  if (OffHour2 > 23) {
    OffHour2 = 0;
  }
  if (OnMinute1 > 59) {
    OnMinute1 = 0;
  }
  if (OnMinute2 > 59) {
    OnMinute2 = 0;
  }
  if (OffMinute1 > 59) {
    OffMinute1 = 0;
  }
  if (OffMinute2 > 59) {
    OffMinute2 = 0;
  }

  if (OnHour1 < 0) {
    OnHour1 = 23;
  }
  if (OnHour2 < 0) {
    OnHour2 = 23;
  }
  if (OffHour1 < 0) {
    OffHour1 = 23;
  }
  if (OffHour2 < 0) {
    OffHour2 = 23;
  }
  if (OnMinute1 < 0) {
    OnMinute1 = 59;
  }
  if (OnMinute2 < 0) {
    OnMinute2 = 59;
  }
  if (OffMinute1 < 0) {
    OffMinute1 = 59;
  }
  if (OffMinute2 < 0) {
    OffMinute2 = 59;
  }

  if (irrecv.decode(&results)) {                                                 //IR controller
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
    if (results.value == 16720605 ){                                          //A off
        digitalWrite(Relay1, HIGH);
      Serial.println(results.value, HEX);
         irrecv.resume(); // Receive the next value
      }

    else if (results.value == 16712445  ){                                         //A on
         digitalWrite(Relay1, LOW);
      Serial.println(results.value, HEX);
          irrecv.resume(); // Receive the next value
    }

else if (results.value == 16769055 ){                                          //B off
         digitalWrite(Relay2, HIGH);
      Serial.println(results.value, HEX);
          irrecv.resume(); // Receive the next value
    }
    
    else if (results.value == 16754775 ){                                          //B on
         digitalWrite(Relay2, LOW);
      Serial.println(results.value, HEX);
          irrecv.resume(); // Receive the next value
    }



    
  }

  ClickEncoder::Button b = encoder->getButton();                                  //encoder controller
  if (b != ClickEncoder::Open) {
    Serial.print("Button: ");
#define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      //用法
      //case ClickEncoder::Pressed:
      //Serial.println("ClickEncoder::Pressed");
      // break;

      case ClickEncoder::Pressed:
        //似乎没用！
        Serial.println("ClickEncoder::Pressed");
        break;

      case ClickEncoder::Held:
        Serial.println("ClickEncoder::Held");
        TestEn = 0;
        SkipPanWait1 = 0;
        SkipPanWait2 = 0;
        break;

      case ClickEncoder::Released:
        Serial.println("ClickEncoder::Released");
        lcd.clear();
        var = 1;
        break;


      case ClickEncoder::Clicked:
        SetA++;
        SetB++;
        SetPanA++;
        SetPanB++;
        Serial.println("SetA++ed");
        Serial.println(SetA);
        Serial.println("ClickEncoder::Clicked");
        break;

      case ClickEncoder::DoubleClicked:
        var++;
        Serial.println("ClickEncoder::DoubleClicked");
        encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
        Serial.print("  Acceleration is ");
        Serial.println((encoder->getAccelerationEnabled()) ? "enabled" : "disabled");
        break;
    }
  }


  DateTime now = RTC.now();

  if ((NowPanic == 1) ||
      (
        (now.hour() == OnHour1)
        && (now.minute() == OnMinute1)
        && (Enable1 == 1)
      )) {                                                                                         //Ch A Time!

    if (PanEn1 == 1) {

      if (((millis() - time1) > (PanWait1 * 1000)) && (SkipPanWait1 == 0)) //Has one second passed?                             // Wait
      {
        Serial.println("P1wait");
        time1 = millis();           //and reset time1.
        SkipPanWait1 = 1;           //wait only once
      }

      if (((millis() - time1) > (PanOn1 * 50)) && (SkipPanWait1 == 1)) //Has one second passed?                  // On
      {
        Serial.println("P1on");
        digitalWrite(Relay1, LOW);  //ON
        time1 = millis();           //and reset time1.
        SkipPanWait1 = 2;
      }

      if (((millis() - time1) > (PanOff1 * 50)) && (SkipPanWait1 == 2)) //Has one second passed?                  // Off
      {
        Serial.println("P1off");
        digitalWrite(Relay1, HIGH);  //Off
        time1 = millis();           //and reset time1.
        SkipPanWait1 = 1;
      }


    }
  }                                                                                                //End A CHannel


  if ((NowPanic == 1) ||
      (
        (now.hour() == OnHour2)
        && (now.minute() == OnMinute2)
        && (Enable2 == 1)
      )) {                                                                                         //Ch B time2!

    if (PanEn2 == 1) {

      if (((millis() - time2) > (PanWait2 * 1000)) && (SkipPanWait2 == 0)) //Has one second passed?                             // Wait
      {
        Serial.println("P2waitdone");
        time2 = millis();           //and reset time2.
        SkipPanWait2 = 1;           //wait only once
      }

      if (((millis() - time2) > (PanOn2 * 50)) && (SkipPanWait2 == 1)) //Has one second passed?                  // On
      {
        Serial.println("P2on");
        digitalWrite(Relay2, LOW);  //ON
        time2 = millis();           //and reset time2.
        SkipPanWait2 = 2;
      }

      if (((millis() - time2) > (PanOff2 * 50)) && (SkipPanWait2 == 2)) //Has one second passed?                  // Off
      {
        Serial.println("P2off");
        digitalWrite(Relay2, HIGH);  //Off
        time2 = millis();           //and reset time2.
        SkipPanWait2 = 1;
      }


    }
  }                                                                                              //Ch B END








  if ((Enable1 == 1) && (now.hour() == OffHour1) && (now.minute() == OffMinute1)) {                                    //AAAshut off

    digitalWrite(Relay1, HIGH);

  }
  if ((Enable2 == 1) && (now.hour() == OffHour2) && (now.minute() == OffMinute2)) {                              //BBBshut off

    digitalWrite(Relay2, HIGH);
  }



  lcd.setCursor(0, 0);                                   //Top Line of LCD

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
  if (Enable1 == 1) {
    if ((now.hour(), DEC) == OnHour1) {
      if ((now.minute(), DEC) == OnMinute1)

      {
        digitalWrite(Relay1, HIGH);
      }
    }
  }
  if (Enable1 == 1) {
    if ((now.hour(), DEC) == OffHour1) {
      if ((now.minute(), DEC) == OffMinute1) {
        digitalWrite(Relay1, LOW);
      }
    }
  }
  if (Enable2 == 1) {
    if ((now.hour(), DEC) == OnHour2) {
      if ((now.minute(), DEC) == OnMinute2) {
        digitalWrite(Relay2, HIGH);
      }
    }
  }
  if (Enable2 == 1) {
    if ((now.hour(), DEC) == OffHour2) {
      if ((now.minute(), DEC) == OffMinute2) {
        digitalWrite(Relay2, LOW);
      }
    }
  }

  switch (var) {                                                      //Main Switch Var
    case 1:
      //do something when var equals 1 display normally

      if (change == 1) {                                                    //QuickToggle
        ToggleA++;
        if (ToggleA > 1) {
          ToggleA = 0;
          digitalWrite(Relay1, HIGH);
        }
        else {
          digitalWrite(Relay1, LOW);
        }
      }

      if (change == -1) {
        ToggleB++;

        if (ToggleB > 1) {
          ToggleB = 0;
          digitalWrite(Relay2, HIGH);
        }
        else {
          digitalWrite(Relay2, LOW);
        }
      }

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
        lcd.print("-A=Off- ");
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
        lcd.print(" -B=Off-");
      }



      break;

    case 2:
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

      switch (SetA) {                                            //SetA
        case 1:
          //do something when var equals 1
          lcd.setCursor(4, 1);
          lcd.print("<");


          Enable1 = Enable1 + change;
          if (Enable1 > 1) {
            Enable1 = 1;
          }
          if (Enable1 < 0) {
            Enable1 = 0;
          }


          break;

        case 2:
          //do something when var equals 2
          lcd.setCursor(4, 1);
          lcd.print(">");

          OnHour1 = OnHour1 + change;

          break;

        case 3:
          //do something when var equals 3
          lcd.setCursor(7, 1);
          lcd.print(">");

          OnMinute1 = OnMinute1 + change;

          break;

        case 4:
          //do something when var equals 4
          lcd.setCursor(10, 1);
          lcd.print(">");

          OffHour1 = OffHour1 + change;

          break;
        case 5:
          //do something when var equals 5
          lcd.setCursor(13, 1);
          lcd.print(">");

          OffMinute1 = OffMinute1 + change;

          break;

        default:
          SetA = 0;
          break;
      }                                  // SetAEnd


      break;                                                    //Var =2 End

    case 3:                                                      // Var=3 Start
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

      switch (SetB) {                                            //SetB
        case 1:
          //do something when var equals 1
          lcd.setCursor(4, 1);
          lcd.print("<");

          Enable2 = Enable2 + change;
          if (Enable2 > 1) {
            Enable2 = 1;
          }
          if (Enable2 < 0) {
            Enable2 = 0;
          }


          break;

        case 2:
          //do something when var equals 2
          lcd.setCursor(4, 1);
          lcd.print(">");

          OnHour2 = OnHour2 + change;

          break;

        case 3:
          //do something when var equals 3
          lcd.setCursor(7, 1);
          lcd.print(">");

          OnMinute2 = OnMinute2 + change;

          break;

        case 4:
          //do something when var equals 4
          lcd.setCursor(10, 1);
          lcd.print(">");

          OffHour2 = OffHour2 + change;

          break;
        case 5:
          //do something when var equals 5
          lcd.setCursor(13, 1);
          lcd.print(">");

          OffMinute2 = OffMinute2 + change;

          break;

        default:
          SetB = 0;
          break;
      }                                  // SetBEnd

      break;

    case 4:
      lcd.setCursor(0, 1);
      lcd.print("A");
      if (PanEn1 == 1)
      {
        // action A
        lcd.print("*on|");
      }
      else
      {
        // action B
        lcd.print("off|");
      }

      if (PanWait1 < 100) {
        lcd.print("0");
        if (PanWait1 < 10) {
          lcd.print("0");

        }
      }
      lcd.print(PanWait1);

      lcd.print("|");
      if (PanOn1 < 100) {
        lcd.print("0");
        if (PanOn1 < 10) {
          lcd.print("0");

        }
      }
      lcd.print(PanOn1);

      lcd.print("|");
      if (PanOff1 < 100) {
        lcd.print("0");
        if (PanOff1 < 10) {
          lcd.print("0");

        }
      }
      lcd.print(PanOff1);

      switch (SetPanA) {                                            //SetPanA
        case 1:
          //do something when var equals 1
          lcd.setCursor(4, 1);
          lcd.print("<");


          PanEn1 = PanEn1 + change;
          if (PanEn1 > 1) {
            PanEn1 = 1;
          }
          if (PanEn1 < 0) {
            PanEn1 = 0;
          }


          break;

        case 2:
          //do something when var equals 2
          lcd.setCursor(4, 1);
          lcd.print(">");

          PanWait1 = PanWait1 + change;

          break;

        case 3:
          //do something when var equals 3
          lcd.setCursor(8, 1);
          lcd.print(">");

          PanOn1 = PanOn1 + change;

          break;

        case 4:
          //do something when var equals 4
          lcd.setCursor(12, 1);
          lcd.print(">");

          PanOff1 = PanOff1 + change;

          break;

        default:
          SetPanA = 0;
          break;
      }                                  // SetPanAEnd

      break;
    case 5:
      lcd.setCursor(0, 1);
      lcd.print("B");
      if (PanEn2 == 1)
      {
        // action A
        lcd.print("*on|");
      }
      else
      {
        // action B
        lcd.print("off|");
      }

      if (PanWait2 < 100) {
        lcd.print("0");
        if (PanWait2 < 10) {
          lcd.print("0");

        }
      }
      lcd.print(PanWait2);

      lcd.print("|");
      if (PanOn2 < 100) {
        lcd.print("0");
        if (PanOn2 < 10) {
          lcd.print("0");

        }
      }
      lcd.print(PanOn2);

      lcd.print("|");
      if (PanOff2 < 100) {
        lcd.print("0");
        if (PanOff2 < 10) {
          lcd.print("0");

        }
      }
      lcd.print(PanOff2);

      switch (SetPanB) {                                            //SetPanB
        case 1:
          //do something when var equals 1
          lcd.setCursor(4, 1);
          lcd.print("<");


          PanEn2 = PanEn2 + change;
          if (PanEn2 > 1) {
            PanEn2 = 1;
          }
          if (PanEn2 < 0) {
            PanEn2 = 0;
          }


          break;

        case 2:
          //do something when var equals 2
          lcd.setCursor(4, 1);
          lcd.print(">");

          PanWait2 = PanWait2 + change;

          break;

        case 3:
          //do something when var equals 3
          lcd.setCursor(8, 1);
          lcd.print(">");

          PanOn2 = PanOn2 + change;

          break;

        case 4:
          //do something when var equals 4
          lcd.setCursor(12, 1);
          lcd.print(">");

          PanOff2 = PanOff2 + change;

          break;

        default:
          SetPanB = 0;
          break;
      }                                  // SetPanBEnd

      break;

    case 6:
      TestEn = TestEn + change;
      if (TestEn > 1) {
        TestEn = 1;
      }
      if (TestEn < 0) {
        TestEn = 0;
      }


      if (TestEn == 1) {
        lcd.setCursor(0, 1);
        lcd.print("TEST ON         ");
        PanEn1 = 1;
        PanEn2 = 1;
        Enable1 = 1;
        Enable2 = 1;
        NowPanic = 1;
        SkipPanWait1 = 0;
        SkipPanWait2 = 0;
      }
      else {
        lcd.setCursor(0, 1);
        lcd.print("TEST Off       ");
        PanEn1 = 0;
        PanEn2 = 0;
        Enable1 = 0;
        Enable2 = 0;
        NowPanic = 0;
      }
      break;

    default:
      // if nothing else matches, do the default
      // default is optional
      var = 1;
      SetPanA = 0;
      SetPanB = 0;
      SetA = 0;
      SetB = 0;
      Serial.println("default var ed");
      break;
  }

}

