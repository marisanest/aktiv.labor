#include <TM1637Display.h>  //  Einbinden der Bibliothekt zum ansteuern des Displays
#include<SoftwareSerial.h>  //   header file of software serial port
 
#define CLK 4  //  PIN 4 für den CLK PIN
#define DIO 5  //  PIN 5 für den DIO 
#define push(d) stack[stackptr++] = d

const int TOP_X_GESTURE_BOUNDRY = 400;
const int MIN_DISPLAYABLE_DISTANCE = 50;
const int MAX_DISPLAYABLE_DISTANCE = 1200;
const int DELAY_DURATION = 100;
const int BLANK_DISPAY_DELAY_DURATION = 3000;
const int HEADER = 0x59;  //  frame header of data package
const int GAME_MODE_DISTANCE = 0;
const int GAME_MODE_RANDOM_DISTANCE = 1;
const int MAX_STACK_SIZE = 50;

TM1637Display display(CLK, DIO);
SoftwareSerial lidarSerial (2,3);  //  define software serial port name asSerial1 and define pin2 as RX and pin3 as TX

// LiDAR
int dist;  //  actual distance measurements of LiDAR
int check;  //  save check value
int i; // index
int uart[9];  //  save data measured by LiDAR
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff }; // Setzt die Anzahl der möglichen Segmente.
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

// Gyro
int xaxis;
int stack[MAX_STACK_SIZE];
int stackptr = 0;
bool filled = false;
float stackavg;
float stacklen;

// Game
int gamemode = 0;

void setup() {
 Serial.begin(9600); 
 lidarSerial.begin(115200);

 display.setBrightness(8);
 display.setSegments(data);
 
 delay(DELAY_DURATION);
}

void loop() {
  if (stackptr >= MAX_STACK_SIZE){
    stackptr = 0; 
    filled = true;
  }

  xaxis = analogRead(A0);
 
  push(xaxis);
  stacklen = (filled == true ? MAX_STACK_SIZE : stackptr);
  stackavg = avg(stack, stacklen);
  
  if(stackavg >= TOP_X_GESTURE_BOUNDRY){
    display.setSegments(blank);
    delay(BLANK_DISPAY_DELAY_DURATION);
    gamemode = (gamemode == 1 ? 0 : 1);

    if(gamemode == GAME_MODE_RANDOM_DISTANCE)
      diplayRandomDistance();
    else if (gamemode == GAME_MODE_DISTANCE)
      displayDistance();
  } else if (gamemode == GAME_MODE_DISTANCE) {
    displayDistance();
  }
}

float avg(int *array, int len) {
  int sum = 0;
  for (int i = 0 ; i < len ; i++)
    sum += array[i];
  return  ((float) sum) / len;
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
