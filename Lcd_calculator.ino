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

boolean selectPressed = false;
  int equationState = 0;
  int arg1 = 0;
  int arg2 = 0;
  int oper = 0; //0=+, 1=-, 2=*, 3=/, 4=%, 5=^
  
void setup() {
  
  
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  // Print a message to the LCD. We track how long it takes since
  // this library has been optimized a bit and we're proud of it :)
  int time = millis();
  lcd.print("e = m * c^2!");
  time = millis() - time;
  Serial.print("Took "); Serial.print(time); Serial.println(" ms");
  lcd.setBacklight(WHITE);
}

uint8_t i=0;
void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis()/1000);

  uint8_t buttons = lcd.readButtons();
  if (equationState == 0) {
    lcd.setCursor(0,0);
    lcd.print("Select first num");
    lcd.setBacklight(WHITE);
    lcd.setCursor(0,1);
    
  } else if (equationState == 1) {
    lcd.setCursor(0,0);
    lcd.print("Select operation");
    lcd.setBacklight(WHITE);
    lcd.setCursor(4,1);
   
  } else if(equationState == 2) {
    lcd.setCursor(0,0);
    lcd.print("Select second num");
    lcd.setBacklight(WHITE);
    lcd.setCursor(7,1);
    
  } else if (equationState == 3) {
    // compute and print the answer
    lcd.setCursor(0,0);
    lcd.print("Answer          ");
    lcd.setBacklight(WHITE);
    lcd.setCursor(10,1);
    lcd.print("=");
    lcd.setCursor(12,1);
    int answer = getAnswer();
    lcd.print(answer);
  }
    
  if (buttons) {
    //lcd.clear();
    
    if (buttons & BUTTON_UP) {
      
      if (equationState==0) {
        // then dealing with arg1
        arg1++;
        
        lcd.print(arg1);
        lcd.setBacklight(WHITE);
      } else if (equationState == 1) {
       // dealing with operation 
        oper = (oper + 1) % 6;
        printOper();
      } else if (equationState == 2) {
        // delaing with arg2
        arg2++;
        lcd.print(arg2);
        lcd.setBacklight(WHITE);
      }
    }
    if (buttons & BUTTON_DOWN) {
      
      if (equationState==0) {
        // then dealing with arg1
        arg1--;
        lcd.print(arg1);
        lcd.print(" ");
        lcd.setBacklight(WHITE);
      } else if (equationState == 1) {
        oper = (oper-1) % 6;
        printOper();
      } else if (equationState == 2) {
        // delaing with arg2
        arg2--;
        lcd.print(arg2);
        lcd.print(" ");
        lcd.setBacklight(WHITE);
      }
    }
    if (buttons & BUTTON_LEFT) {
      lcd.print("LEFT ");
      lcd.setBacklight(WHITE);
    }
    if (buttons & BUTTON_RIGHT) {
      lcd.print("RIGHT ");
      lcd.setBacklight(WHITE);
    }
    if (buttons & BUTTON_SELECT) {
      // go to next equation state
      equationState = (equationState+1) % 4;
      if (equationState==0) {
        lcd.clear();
      }
    }
  }
}
  
  int getAnswer() {
    int ans = 0;
    switch (oper) {
      case 0: //+
        ans = arg1 + arg2;
        break;
      case 1://-
        ans = arg1 - arg2;
        break;
      case 2://*
        ans = arg1 * arg2;
        break;
      case 3:// /
        if (arg2 != 0) {
          ans = arg1 / arg2;
        } else
          ans = 99999;
        break;
      case 4: // %
        ans = arg1 % arg2;
        break;
      case 5: // ^
        ans = arg1 ^ arg2;
        break;
      
    }
    return ans;
  }
  
  void printOper() {
    switch (oper) {
      case 0:
        lcd.print("+");
        break;
      case 1:
        lcd.print("-");
        break;
      case 2:
        lcd.print("*");
        break;
      case 3:
        lcd.print("/");
        break;
      case 4:
        lcd.print("%");
        break;
      case 5:
        lcd.print("^");
        break;
    }
    lcd.setBacklight(WHITE);
  }
