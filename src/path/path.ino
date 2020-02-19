// include SPI, SD, Music Maker Shield and Arduino libraries
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
#include <arduino.h>

// Pins used for Music Maker Shield
#define SHIELD_RESET -1 // PIN for SHIELD_RESET unused
#define SHIELD_CS 7 // PIN 7 for SHIELD_CS
#define SHIELD_DCS 6 // PIN 6 for SHIELD_DCS
#define CARDCS 4 // PIN 4 for CARDCS
#define DREQ 3 // PIN 3 for DREQ

const int DELAY_DURATION = 100; // delay duration
int VOLUME = 0; // volume
const int HALL_PIN = 5; // hall pin
const int TRACKS_COUNT = 2;  // tracks count

static const char* const tracks [TRACKS_COUNT] =  { // all available tracks
  "/path001.mp3", 
  "/path002.mp3"
};
int current_track_index = 0;  // current index of the track which is playing
Adafruit_VS1053_FilePlayer player = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS); // initialise player
bool playing = false; // value if track is playing
int hall = 1; // hall value; 1 if no magnet is present else 0

/*
 * Setup for the main loop
 */
void setup() {
  // initalize the serial output
  Serial.begin(9600);

  // set up hall pin
  pinMode(HALL_PIN, INPUT);
  
  // start player
  if (!player.begin()) {
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);  // don't do anything more
  }
  
  // start SD card
  Serial.println(F("VS1053 found"));
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // set volume
  player.setVolume(VOLUME,VOLUME);

  // executes delay
  delay(DELAY_DURATION);
}

void loop() {

  // reads the current hall value
  hall = digitalRead(HALL_PIN);

  // checks if hall value and if track is playing
  if(hall == 0 && playing == false){

    // interrrupt player
    player.useInterrupt(VS1053_FILEPLAYER_PIN_INT);

    // define current track
    current_track_index = (current_track_index == 0 ? 1 : 0);

    // start current track
    player.startPlayingFile(tracks[current_track_index]);
    playing = true;
    
   } else if (hall == 1 && playing == true){

    // stop playing track
    player.stopPlaying();
    playing = false;
    
   }

  // executes delay
  delay(DELAY_DURATION);
}
