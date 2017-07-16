/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

  boolean jump = false;
  boolean right = 0;
  boolean left = 0;
  int x = 0;
  int y = 1;
  int delayTime = 500;
  long timeNow=0; 
  int ox = 16; 
  long timeNowo=0;
  boolean stomp = false;
  boolean collision = false;
  char* mario = "M";
  long score = 0;
  long oldScore = 0;
  int coin1x = 0;
  int coin1y = 0;
  boolean coin1Alive = true;
  int timeNowc = 0;
  
  
void setup() {
  
  
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  // Print a message to the LCD. We track how long it takes since
  // this library has been optimized a bit and we're proud of it :)
  
  //Serial.print("Took "); Serial.print(time); Serial.println(" ms");
  lcd.setBacklight(WHITE);
  randomSeed(millis());
  coin1x = random(0,15);
  coin1y = random(0,1);
}

uint8_t i=0;
void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 0);
  // print the number of seconds since reset:
  //lcd.print(millis()/1000);

  uint8_t buttons = lcd.readButtons();
  lcd.setCursor(x,y);
  lcd.print(mario);
  
  if (((millis()-timeNowo) > 500)) {
    
      
      lcd.setCursor(ox,1);
      if (stomp == false) {
        lcd.print("O ");
      }
      if (stomp == true) {
        lcd.print("_ ");
        delay(1000);
        lcd.print(" ");
        if (abs(ox-x) > 2) stomp=false;
      }
      lcd.setBacklight(WHITE);
      if (ox==0) {
        lcd.setCursor(0,1);
        lcd.print(" ");
        ox=16;
      }
      if (ox !=0) {
        ox--;
      }
      if (collision) {
        
        mario = "m";
        oldScore = score;
        if (abs(ox-x)>2) {
          collision = false;
        }
      }
      lcd.setCursor(0,0);
      lcd.print("Score=");
      lcd.print(score);
      timeNowo=millis();
  }
      if (millis()-timeNowc >10000) {
        coin1Alive = true;
      }
  
  
  if (jump == true && ((millis()-timeNow) > 1000)) {
      
      lcd.clear();
      y=1;
      lcd.setCursor(x,y);
      lcd.print("M");
      lcd.setBacklight(WHITE);
      if (ox==x && y==1 && stomp == false) {
        stomp = true;
        score+=10;
      }
      jump = false;
  }
  if (buttons & BUTTON_UP) {
    lcd.clear();
    y=0;
    lcd.setCursor(x,y);
    timeNow = millis();
    lcd.print("M");
    lcd.setBacklight(WHITE);
    jump = true;
    //delay(delayTime);
    
  }
  if (buttons & BUTTON_DOWN) {
    
    
  }
  if (buttons & BUTTON_LEFT) {
    lcd.clear();
    if (x==0) {
      x = 16;
      lcd.setCursor(x,y);
      lcd.print("M");
    }
    if (x !=0) {
      x--;
      lcd.setCursor(x,y);
      lcd.print("M");
    }
  }
  if (buttons & BUTTON_RIGHT) {
    lcd.clear();
    if (x==16) {
      x = 0;
      lcd.setCursor(x,y);
      lcd.print("M");
    }
    if (x !=16) {
      x++;
      lcd.setCursor(x,y);
      lcd.print("M");
    }
  }
  if (ox==x && y==1 && stomp==false && collision==false) {
    collision=true;
    score-=10;
  }

  if (score == oldScore + 10) {
     collision=false;
     mario = "M";
  } 
  lcd.setCursor(12,1);
  if (coin1Alive == true) {
    lcd.print("o");
    if (x==12 && y == 1) {
      score+=5;
      coin1Alive = false;
      timeNowc = millis();
    }
  }
}
