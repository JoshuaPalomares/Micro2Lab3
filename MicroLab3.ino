#include <RTClib.h>
#include <LiquidCrystal.h>
#include <Wire.h>

LiquidCrystal lcd(10, 8, 6, 5, 4, 3);
RTC_DS1307 timer;

#define ctrlr 2
#define en 7
#define CW 11
#define CCW 12
int CurrControllerState; //use to detect input state of the button.
int LastControllerState = HIGH; //use to detect input state of the button
int integerState; //use for counting controller input
int fandir = LOW;
int lastSpeed = 0;
unsigned long time_increment = 0;
unsigned long prev_time_increment = 0;

void setup() 
{
  
  // put your setup code here, to run once:
 

  //Input Initialization
  pinMode(en, OUTPUT);
  pinMode(CW, OUTPUT);
  pinMode(CCW, OUTPUT);
  pinMode(ctrlr, INPUT);
  Serial.begin(9600); //Begin communication thru I2C Bus.
  Wire.begin(); //initialization of the single-wire based LED pixels
  timer.begin(); //initialization of time counter.
  //LCD-screen initialization
  lcd.begin(16, 2); //Initialize LCD process (16 col(width) & 2 row(height))
  // lcd.clear();
  
  //Button Initialization
  attachInterrupt(digitalPinToInterrupt(2), buttonControl, RISING);
  

}

void loop() 
{
  int crtlSetting = 0;
  DateTime time = timer.now();


  if (time.second() % 60 == 0)
  {
    //every 60-seconds, reset the display.
    //seconds would display as X-XX-X9 
    //every minute that passed. To Avoid this, check when/if the second value % 10 is 0. If so, clear the value to effectively reset it.
    // lcd.setCursor(0,0); //set cursor @ point (0,0) (highest-leftmost point on the LCD screen)
    // lcd.print(" "); //Output time to the first row of the 2-row LCD display.
    lcd.clear();
  }
  lcd.setCursor(0,0); //set cursor @ point (0,0) (highest-leftmost point on the LCD screen)
  lcd.print("Time: "); //Output time to the first row of the 2-row LCD display.
  lcd.print(time.hour());
  lcd.print(":");
  lcd.print(time.minute());
  lcd.print(":");
  lcd.print(time.second());
  // digitalWrite(en,HIGH);

  if(time.second() < 30) 
  {
    
    if (time.second() == 0)
    {
      
      lastSpeed++;
    }

      changeSP(lastSpeed);
  }
  else 
  {
    digitalWrite(en, LOW);
    digitalWrite(CCW, 0);
    digitalWrite(CW, 0);
    spOff();
  }

  delay(1000);
}

void changeSP(int lastSpeed)
{
    lcd.setCursor(0,1); //set cursor @ point (0,0) (highest-leftmost point on the LCD screen)
    if (lastSpeed == 1)
    {
      digitalWrite(en,HIGH);
      digitalWrite(CCW, !fandir);
      digitalWrite(CW, fandir);
      analogWrite(en, 80);
      lcd.print("Sp: 1/4");
      delay(2000);
    }
    else if (lastSpeed == 2)
    {
      digitalWrite(en,HIGH);
      digitalWrite(CCW, !fandir);
      digitalWrite(CW, fandir);
      analogWrite(en, 120);
      lcd.print("Sp: 1/2");
      delay(2000);
    }
    else if (lastSpeed == 3)
    {
      digitalWrite(en,HIGH);
      digitalWrite(CCW, !fandir);
      digitalWrite(CW, fandir);
      analogWrite(en, 180);
      lcd.print("Sp: 3/4");
      delay(2000);
    }
    else if (lastSpeed == 4)
    {
      digitalWrite(en,HIGH);
      digitalWrite(CCW, !fandir);
      digitalWrite(CW, fandir);
      lastSpeed = 0;
      analogWrite(en, 255);
      lcd.print("Sp: FULL");
      delay(2000);
    }
    else if (lastSpeed > 4)
    {
      lastSpeed = 1;
      changeSP(lastSpeed);
    }


}

void spOff()
{
  lcd.setCursor(0,1);
  lcd.print("Sp: 0 | Dir: N/A");  
}


void buttonControl()//https://projecthub.arduino.cc/ronbentley1/16d57fe0-986a-4629-a6cb-4e69f8be61c5
{
   time_increment = millis(); //returns the number of milliseconds since the start of the program
   //this value is scaling infinitely relative to how long the code has been executing for, so it is useful in perpetually changing the direction
  if (time_increment - prev_time_increment > 255) //255 as defined within spect
  {
    fandir = !(fandir); //adjust value of direction to produce change
   if (fandir == LOW) {
      lcd.clear();     
      lcd.setCursor(0, 1);
      lcd.print("Dir Change: CCW");
    }
  }    
    else {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Dir Change: CW");
    }

  prev_time_increment = time_increment;
}





