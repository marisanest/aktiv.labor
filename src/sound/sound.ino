// include SPI, SD and Music Maker Shield libraries
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

// Pins used for Music Maker Shield
#define SHIELD_RESET -1 // PIN for SHIELD_RESET unused
#define SHIELD_CS 7 // PIN 7 for SHIELD_CS
#define SHIELD_DCS 6 // PIN 6 for SHIELD_DCS
#define CARDCS 4 // PIN 4 for CARDCS
#define DREQ 3 // PIN 3 for DREQ

const int DELAY_DURATION = 500; // delay duration
const int MAX_VOLUME = 0; // maximum volume
const int MIN_VOLUME = 60; // minimum volume
const int VOLUME_MULTIPLICATOR = 1; // volume multiplicator for control
const int BOTTOM_Y_AXIS_BOUNDARY = 345; // bottom y axis boundary
const int TOP_Y_AXIS_BOUNDARY = 405; // top y axis boundary
const int TRACKS_COUNT = 6; // tracks count

int y_axis; // y axis value
int y_axis_diff; // differance between y axis value and TOP_Y_AXIS_BOUNDARY
static const char* const tracks [TRACKS_COUNT] = { // all available tracks
  "/track001.mp3", "/track002.mp3", "/track003.mp3", 
  "/track004.mp3", "/track005.mp3", "/track006.mp3"
  };
int current_track_index; // current index of the track which is playing
bool playing = false; // value if track is playing
int current_volume; // current volume
Adafruit_VS1053_FilePlayer player = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS); // initialise player

/*
 * Setup for the main loop
 */
void setup() {
  // initalize the serial output
  Serial.begin(9600);

  // start player
  if (!player.begin()) {
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1); // don't do anything more
  }
  
  Serial.println(F("VS1053 found"));

  // start SD card
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // set inital volume
  resetVolume();

  // executes delay
  delay(DELAY_DURATION);
}

/*
 * Executes the main loop
 */
void loop() {

  // reads the current y axis value
  y_axis = analogRead(A1);

  // check if y axis boundary is exceeded
  if (y_axis >= BOTTOM_Y_AXIS_BOUNDARY) {
    
    Serial.println("Bottom y axis boundary exceeded");

    // set volume
    setVolume(y_axis);

    if (playing == false){

      // interrrupt player
      player.useInterrupt(VS1053_FILEPLAYER_PIN_INT);

      // define current track
      current_track_index = random(TRACKS_COUNT);

      // start current track
      player.startPlayingFile(tracks[current_track_index]);
      playing = true;
     
     } else if (player.stopped()) {
      // restart current track
      player.startPlayingFile(tracks[current_track_index]);
     }
  } else {
    
    Serial.println("Fell below bottom y axis boundary");
    
    if (playing == true){

      // reset volume
      resetVolume();

      // stop playing track
      player.stopPlaying();
      playing = false;
      
    }
  }
  
  // executes delay
  delay(DELAY_DURATION);
}

/*
 * Resets the volume to the minimum
 */
void resetVolume() {
  player.setVolume(MIN_VOLUME, MIN_VOLUME);
}

/*
 * Seta the volume depending on the y axis value
 */
void setVolume(int y_axis) {
  
  // caluculates differance between y axis value and TOP_Y_AXIS_BOUNDARY
  y_axis_diff = TOP_Y_AXIS_BOUNDARY - y_axis;
  
  // calculates current volume
  current_volume = (y_axis_diff < 0 ? 0 : y_axis_diff * VOLUME_MULTIPLICATOR);

  // set volume
  player.setVolume(current_volume, current_volume);
}
