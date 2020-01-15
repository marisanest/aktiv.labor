
// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <stdlib.h>
#include <time.h>

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

int xA;
int yA;
int zA;
static const char* const tracks [] = {"/track003.mp3", "/track004.mp3", "/track005.mp3", "/track006.mp3"};
bool playing = false;
static const char* current_track = ""; 
 
void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);
  srand ( time(NULL) );
}
 
void loop() {
  yA = analogRead(A1);
 
  if (yA > 390) {
    Serial.println(" - Y - ist oben");
    if (playing == false){
      int random_index = rand() % 4;
      current_track = tracks[random_index];
      Serial.println("Start playing "); //  + current_track
      musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT); 
      musicPlayer.startPlayingFile(current_track);
      playing = true;
     }
     Serial.println("Playing "); //  + current_track
  } else {
    Serial.println(" - Y - ist nicht oben");
    if (playing == true){
      playing = false;
      Serial.println("Stop playing "); // + current_track
      musicPlayer.stopPlaying();
    }
    Serial.println("Nothing playing");
  }
 
  delay(1000);
}
