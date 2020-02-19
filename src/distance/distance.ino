// include Lidar ans display libraries
#include <TM1637Display.h>
#include <SoftwareSerial.h>

// Pins used for Display
#define CLK 4 // PIN 4 for CLK
#define DIO 5 // PIN 5 for DIO 

// Pins used for Lidar
#define RX 2 // PIN 2 for RX
#define TX 3 // PIN 3 for TX

// predefined push method for stack
#define push(value) lidar_distance_stack[lidar_distance_stack_pointer++] = value

const int TOP_X_BOUNDARY = 400; // top x boundary
const int MIN_DISTANCE = 50; // minimum diststance lidar can measure
const int MAX_DISPLAYABLE_DISTANCE = 1200; // maximum diststance lidar can measure
const int DELAY_DURATION = 100; // delay duration
const int BLANK_DISPAY_DELAY_DURATION = 3000; // blank display delay duration
const int HEADER = 0x59;  // frame header of data package
const int GAME_MODE_DISTANCE = 0; // identifier for game mode = distance
const int GAME_MODE_RANDOM_DISTANCE = 1; // identifier for game mode = random distance
const int LIDAR_DISTANCE_STACK_SIZE = 50; // stack size
const int DISPLAY_BRIGHTNESS = 8; // display brightness

int lidar_data_fields[9];  // transmitted data by lidar
int lidar_check;  // check value for transmitted data by lidar
int lidar_distance;  //  measured distance by lidar
int lidar_distance_stack[LIDAR_DISTANCE_STACK_SIZE]; // stack for cached measured distance by lidar
int lidar_distance_stack_pointer = 0; // stack pointer
bool lidar_distance_stack_filled = false; // value if stack is filled
float lidar_distance_stack_avg; // stack average
float lidar_distance_stack_length; // stack length
uint8_t diplay_segments[] = { 0xff, 0xff, 0xff, 0xff }; // display segments
uint8_t diplay_blank_segments[] = { 0x00, 0x00, 0x00, 0x00 }; // display blank segments
int x_axis; // x axis value
int game_mode = 0; // game mode value
TM1637Display display(CLK, DIO); // initialise display
SoftwareSerial lidar (RX,TX);  // initialise lidar

/*
 * Setup for the main loop
 */
void setup() {
  // initalize the serial output
  Serial.begin(9600); 

  // initalize the lidar serial output
  lidar.begin(115200);

  // set the display brightness
  display.setBrightness(DISPLAY_BRIGHTNESS);

  // initalize the displayable display segments
  display.setSegments(diplay_segments);

  // executes delay
  delay(DELAY_DURATION);
}

/*
 * Executes the main loop
 */
void loop() {

  // checks if pointer is at the end of stack
  if (lidar_distance_stack_pointer >= LIDAR_DISTANCE_STACK_SIZE){
    
    // reset pointer
    lidar_distance_stack_pointer = 0;
    
    // set stack to filled
    lidar_distance_stack_filled = true;
  }

  // reads the current x axis value
  x_axis = analogRead(A0);

  // pushes the current x axis value to the stack
  push(x_axis);

  // calculates current stack length and average of stack values
  lidar_distance_stack_length = (lidar_distance_stack_filled == true ? LIDAR_DISTANCE_STACK_SIZE : lidar_distance_stack_pointer);
  lidar_distance_stack_avg = avg(lidar_distance_stack, lidar_distance_stack_length);

  // checks if x axis boundary is exceeded
  if(lidar_distance_stack_avg >= TOP_X_BOUNDARY){

    Serial.println("Top x axis boundary exceeded");

    // displays blank and executes delay
    display.setSegments(diplay_blank_segments);
    delay(BLANK_DISPAY_DELAY_DURATION);

    // changes game mode
    game_mode = (game_mode == 1 ? 0 : 1);

    // deplay depending on game mode
    if(game_mode == GAME_MODE_RANDOM_DISTANCE)
      diplayRandomDistance();
    else if (game_mode == GAME_MODE_DISTANCE)
      displayDistance();
      
  } else if (game_mode == GAME_MODE_DISTANCE) {
    displayDistance();
  }
}

/*
 * Calculates the average value of an integer array
 */
float avg(int *data, int len) {
  int sum = 0;
  
  for (int i = 0 ; i < len ; i++)
    sum += data[i];
  
  return  ((float) sum) / len;
}

/*
 * Displays a random distance on the display
 */
void diplayRandomDistance() {
  display.showNumberDec(round(random(MIN_DISTANCE, MAX_DISPLAYABLE_DISTANCE + 1)/10)*10, false, 4, 4);
}

/*
 * Displays the measured distance on the display
 */
void displayDistance() {
  if (lidar.available()) {
    if(lidar.read()==HEADER) {
      
      // saves first lidar data field
      lidar_data_fields[0]=HEADER;
      
      if(lidar.read()==HEADER) {
        
        // saves second lidar data field
        lidar_data_fields[1]=HEADER;

        // reads and saves the rest lidar data field
        for(int i = 2; i < 9 ; i++) {
          lidar_data_fields[i]=lidar.read();
        }

        // prepares verification of the received data as per protocol
        lidar_check = lidar_data_fields[0] + lidar_data_fields[1] + lidar_data_fields[2] +
                lidar_data_fields[3] + lidar_data_fields[4] + lidar_data_fields[5] +
                lidar_data_fields[6] + lidar_data_fields[7]; 
          
        if(lidar_data_fields[8] == (lidar_check & 0xff)) {   // verify the received data as per protocol
          display.showNumberDec(lidar_data_fields[2] + lidar_data_fields[3] * 256, false, 4, 4); // calculate distance value and display it
        }
      }
    }
  }  
}
