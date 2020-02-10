// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

const int DELAY_DURATION = 500;
const int MAX_VOLUME = 0;
const int MIN_VOLUME = 180;
const int VOLUME_MULTIPLICATOR = 3;
const int BOTTOM_Y_BOUNDERY = 345;
const int TOP_Y_BOUNDERY = 405;
const int TRACKS_COUNT = 9;

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

int y_axis;
static const char* const tracks [TRACKS_COUNT] = 
{
  "/track001.mp3", 
  "/track002.mp3", 
  "/track003.mp3", 
  "/track004.mp3", 
  "/track005.mp3", 
  "/track006.mp3", 
  "/track007.mp3", 
  "/track008.mp3", 
  "/track009.mp3"
};
int current_track_index;
bool playing = false;
int y_axis_diff;
int current_volume;

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053 Simple Test");

  if (!musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // Set volume for left, right channels. lower numbers == louder volume!
  resetVolume();
}

void loop() {
  y_axis = analogRead(A1);
 
  if (y_axis >= BOTTOM_Y_BOUNDERY) {
    Serial.println(" - Y - ist oben");
    setVolume(y_axis);
    if (playing == false){
      musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
      current_track_index = random(TRACKS_COUNT);
      musicPlayer.startPlayingFile(tracks[current_track_index]);
      playing = true;
     } else if (musicPlayer.stopped()) {
      musicPlayer.startPlayingFile(tracks[current_track_index]);
     }
  } else {
    Serial.println(" - Y - ist nicht oben");
    if (playing == true){
      resetVolume();
      musicPlayer.stopPlaying();
      playing = false;
    }
  }
  delay(DELAY_DURATION);
}

void resetVolume() {
  musicPlayer.setVolume(MIN_VOLUME, MIN_VOLUME);
}

void setVolume(int y_axis) {
  y_axis_diff = TOP_Y_BOUNDERY - y_axis;
  current_volume = (y_axis_diff < 0 ? 0 : y_axis_diff * VOLUME_MULTIPLICATOR);
  musicPlayer.setVolume(current_volume, current_volume);
}
