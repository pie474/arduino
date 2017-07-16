/* 
 * //////////////////////////////////////////////////
 * //making sense of the Parallax PIR sensor's output
 * //////////////////////////////////////////////////
 *
 * Switches a LED according to the state of the sensors output pin.
 * Determines the beginning and end of continuous motion sequences.
 *
 * @author: Kristian Gohlke / krigoo (_) gmail (_) com / http://krx.at
 * @date:   3. September 2006 
 *
 * kr1 (cleft) 2006 
 * released under a creative commons "Attribution-NonCommercial-ShareAlike 2.0" license
 * http://creativecommons.org/licenses/by-nc-sa/2.0/de/
 *
 *
 * The Parallax PIR Sensor is an easy to use digital infrared motion sensor module. 
 * (http://www.parallax.com/detail.asp?product_id=555-28027)
 *
 * The sensor's output pin goes to HIGH if motion is present.
 * However, even if motion is present it goes to LOW from time to time, 
 * which might give the impression no motion is present. 
 * This program deals with this issue by ignoring LOW-phases shorter than a given time, 
 * assuming continuous motion is present during these phases.
 *  
 */

/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
#include <SoftwareSerial.h>
#include <Servo.h>


#define rxPin   10  // Serial input (connects to Emic 2's SOUT pin)
#define txPin   11  // Serial output (connects to Emic 2's SIN pin)
#define ledPin  13  // Most Arduino boards have an on-board LED on this pin

#define relayPin 2

// set up a new serial port
SoftwareSerial emicSerial =  SoftwareSerial(rxPin, txPin);
boolean stringComplete = false;
boolean emicStringComplete = false;
int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime; 
int nerf_ready = 50;
int nerf_fire = 180;
boolean first_time = true;

int pirPin = 6;    //the digital pin connected to the PIR sensor's output

Servo myservo;
int pos = 0;

/////////////////////////////
//SETUP
void setup(){
  Serial.begin(9600);
  emicSerial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);
   // define pin modes
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  emicSerial.print('S');
  emicSerial.print("Motion sensing nerf gun defense system is now active.");
  emicSerial.print('\n');
  delay(50);
  emicSerial.print('\n');             // Send a CR in case the system is already up
  while (emicSerial.read() != ':');   // When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
  delay(10);                          // Short delay
  emicSerial.flush();
  emicSerial.print("V18\n");
  emicSerial.print("N1\n");
  emicSerial.print("W200\n");
  myservo.attach(5);
  myservo.write(nerf_ready);
}

void fire_nerf_gun(int num_fire) {
    if(first_time == true){
      first_time = false;
      return;
    }
    myservo.write(nerf_ready);
    emicSerial.write('S');
    emicSerial.write("Locked and loaded. Ready to fire.");
    emicSerial.write("\n");
    delay(2000);
    while (emicSerial.read() != ':');
    for(int j = 0;  j< num_fire; j++){
      myservo.write(nerf_fire);
      delay(300);
      myservo.write(nerf_ready);
      delay(300);
    }
  }
  

////////////////////////////
//LOOP
void loop(){

     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec");
         emicSerial.print("W300\n");
         emicSerial.print('S');
         emicSerial.print("HALT. ");
         emicSerial.print("Who,");
         emicSerial.print("GOES,");
         emicSerial.print("THERE.");
         emicSerial.print('\n');
         digitalWrite(relayPin, HIGH);
         while (emicSerial.read() != ':');
         delay(2000);
         fire_nerf_gun(3);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           digitalWrite(relayPin, LOW);
           delay(50);
           }
       }
  }

