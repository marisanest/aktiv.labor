#include <Arduino.h>  //  Einbinden der Arduino Bibliothekt
#include <TM1637Display.h>  //  Einbinden der Bibliothekt zum ansteuern des Displays
#include <Chrono.h>
#include<SoftwareSerial.h>  //   header file of software serial port
 
#define CLK 4  //  PIN 4 für den CLK PIN
#define DIO 5  //  PIN 5 für den DIO 

const int MAX_GESTURE_DURATION = 5000;
const int TOP_X_GESTURE_BOUNDRY = 300; //280
const int BOTTOM_X_GESTURE_BOUNDRY = 380; //400
const int MIN_DISPLAYABLE_DISTANCE = 50;
const int MAX_DISPLAYABLE_DISTANCE = 1200;
const int DELAY_DURATION = 500;
const int HEADER = 0x59;  //  frame header of data package
const int GAME_MODE_DISTANCE = 0;
const int GAME_MODE_RANDOM_DISTNACE = 1;

Chrono gestureChrono; 
TM1637Display display(CLK, DIO);
SoftwareSerial lidarSerial (2,3);  //  define software serial port name asSerial1 and define pin2 as RX and pin3 as TX

// LiDAR
int dist;  //  actual distance measurements of LiDAR
int check;  //  save check value
int i; // index
int uart[9];  //  save data measured by LiDAR
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff }; //Setzt die Anzahl der möglichen Segmente.
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

// Gyro
int xAxis;

// Game
int gameGestureCounter = 0;
int gameMode = 0;

void setup() {
 Serial.begin(9600); 
 lidarSerial.begin(115200);

 display.setBrightness(8);
 display.setSegments(data);
 
 gestureChrono.restart();

 delay(DELAY_DURATION);
}

void loop() {
  
  if(gestureDetected()) {
    if(gameMode == GAME_MODE_RANDOM_DISTNACE) {
      display.setSegments(blank);
      delay(DELAY_DURATION*4);
      diplayRandomDistance();
    } else if (gameMode == GAME_MODE_DISTANCE) {
      display.setSegments(blank);
      delay(DELAY_DURATION*4);
      displayDistance();
    }
    delay(DELAY_DURATION*2);
  } else if (gameMode == GAME_MODE_DISTANCE) {
    displayDistance();
  }
  
  delay(DELAY_DURATION);
}

bool gestureDetected() {
  xAxis = analogRead(A0);
  
  if (xAxis <= TOP_X_GESTURE_BOUNDRY){
    Serial.println("TOP");
    if (gestureChrono.hasPassed(MAX_GESTURE_DURATION)) {
      gameGestureCounter = 0;
      gestureChrono.restart();
    } else if (gameGestureCounter == 0){
      Serial.println(1);
      gameGestureCounter = 1;
      gestureChrono.restart();
    } else if (gameGestureCounter == 2){
      Serial.println(3);
      gameGestureCounter = 0;
      gameMode = (gameMode == 0 ? 1 : 0);
      return true;
    }
  } else if (xAxis >= BOTTOM_X_GESTURE_BOUNDRY){
    Serial.println("BOTTOM");
    if (gestureChrono.hasPassed(MAX_GESTURE_DURATION)) {
      gameGestureCounter = 0;
      gestureChrono.restart();
    } else if (gameGestureCounter == 1){
      Serial.println(2);
      gameGestureCounter = 2;  
    }
  }
  return false;
}

void diplayRandomDistance() {
  display.showNumberDec(round(random(MIN_DISPLAYABLE_DISTANCE, MAX_DISPLAYABLE_DISTANCE + 1)/10)*10, false, 4, 4);
}

void displayDistance() {
  if (lidarSerial.available()) {
    if(lidarSerial.read()==HEADER) {
      uart[0]=HEADER;
      
      if(lidarSerial.read()==HEADER) {
        uart[1]=HEADER;

        for(i=2;i<9;i++) {
          uart[i]=lidarSerial.read();
        }
          
        check=uart[0]+uart[1]+uart[2]+uart[3]+uart[4]+uart[5]+uart[6]+uart[7];
          
        if(uart[8]==(check&0xff)) {   //verify the received data as per protocol
          display.showNumberDec(uart[2]+uart[3]*256, false, 4, 4); //calculate distance value and display it
        }
      }
    }
  }  
}
