#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define WHITE 0x7

float cookies = 0;
float c_per_sec = 0.0;
int cookie_unit = 0;

int clickers = 0;
int clicker_cost = 15;
float clicker_speed = 0.1;


int grandmas = 0;
int grandma_cost = 100;
float grandma_speed = 0.5;

int farms = 0;
int farm_cost = 500;
float farm_speed = 1;

int factories = 0;
int factory_cost = 1000;
float factory_speed = 5;

int select_shop_item = 1;

float currtime = 0.0;

boolean leftButtonPressed = false;

void setup() {
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
  currtime = millis();
}

void loop() {
  uint8_t buttons = lcd.readButtons();
  
  //printing basic info (how many cookies and cookies per second)
  lcd.setCursor(0,0);
  lcd.print(cookies);
  lcd.setCursor(0,1);
  lcd.print(c_per_sec);
  
  
  //printing the shop section
  if (select_shop_item == 1){
    lcd.setCursor(8,0);
    lcd.print("Clicker ");
    lcd.setCursor(8,1);
    lcd.print(clicker_cost);
  }
  
  if (select_shop_item == 2){
    lcd.setCursor(8,0);
    lcd.print("Grandma ");
    lcd.setCursor(8,1);
    lcd.print(grandma_cost);
  }
  
  if (select_shop_item == 3){
    lcd.setCursor(8,0);
    lcd.print("Farm    ");
    lcd.setCursor(8,1);
    lcd.print(farm_cost);
  }
  
  if (select_shop_item == 4){
    lcd.setCursor(8,0);
    lcd.print("Factory ");
    lcd.setCursor(8,1);
    lcd.print(factory_cost);
  }
  
  
  //the part where you click to get cookies
  if ((buttons & BUTTON_LEFT) ){
    if(!leftButtonPressed ) {
    
      lcd.setCursor(0,0);
      lcd.print("        ");
      leftButtonPressed = true;
      cookies++;
    }
  } 
    else {
    leftButtonPressed = false;
  }
  
  
  //scrolling up in the shop
  if (buttons & BUTTON_UP) {
    if (select_shop_item == 4) {
      select_shop_item = 1;
      lcd.setCursor(8,1);
      lcd.print("       ");
    }
    else (select_shop_item < 4); {
      select_shop_item++;
      lcd.setCursor(8,1);
      lcd.print("       ");
    }
  }
  
 
  //scrolling down in the shop
  if (buttons & BUTTON_DOWN) {
    if (select_shop_item == 1) {
      select_shop_item = 4;
      lcd.setCursor(8,1);
      lcd.print("       ");
    }
    else (cookie_unit > 1); {
      select_shop_item--;
      lcd.setCursor(8,1);
      lcd.print("       ");
    }
  }
  
  
  //buying an item in the shop
  if (buttons & BUTTON_RIGHT) {
    
    if (select_shop_item == 1) {
      if (cookies >= clicker_cost) {
        clickers++;
        cookies = cookies - clicker_cost;
        c_per_sec = c_per_sec + clicker_speed;
      }
    }
    
    if (select_shop_item == 2) {
      if (cookies >= grandma_cost) {
        grandmas++;
        cookies = cookies - grandma_cost;
        c_per_sec = c_per_sec + grandma_speed;
      }
    }
    
    if (select_shop_item == 3) {
      if (cookies >= farm_cost) {
        farms++;
        cookies = cookies - farm_cost;
        c_per_sec = c_per_sec + farm_speed;
      }
    }
    
    if (select_shop_item == 4) {
      if (cookies >= factory_cost) {
        clickers++;
        cookies = cookies - factory_cost;
        c_per_sec = c_per_sec + factory_speed;
      }
    }
    
  }
  
  //increment cookies based on cookies per second
  if (millis() - currtime >= 1000) {
    cookies += c_per_sec;
    lcd.setCursor(0,0);
    lcd.print("        ");
    currtime = millis();
  }
  
}
