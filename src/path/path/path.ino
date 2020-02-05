#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <arduino.h>
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

const int DELAY_DURATION = 100;
static const char* const TRACK = "/track001.mp3"; 
int VOLUME = 0;
const int HALL_PIN = 5;

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

bool playing = false;
int hall = 1;

void setup() {
  Serial.begin(9600);
  pinMode(HALL_PIN, INPUT);

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
  musicPlayer.setVolume(VOLUME,VOLUME);
 
  delay(DELAY_DURATION);
}

void loop() {
  hall = digitalRead(HALL_PIN);
  //Serial.println(digitalRead(HALL_PIN));

  if(hall == 0 && playing == false){
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT); 
    musicPlayer.startPlayingFile(TRACK);
    playing = true;
   } else if (hall == 1 && playing == true){
    musicPlayer.stopPlaying();
    playing = false;
   }
  
  delay(DELAY_DURATION);
}
