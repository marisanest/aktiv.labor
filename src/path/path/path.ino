#include <SD.h>
#include <SPI.h>
#include <arduino.h>
#include <MusicPlayer.h>

const int HALL_PIN = 4;

void setup() {
  Serial.begin(9600);
  //pinMode(HALL_PIN, INPUT);
  //player.beginInMidiFmt();
  player.begin()
  //player.scanAndPlayAll();
}
 
void loop() {
  //Serial.println(digitalRead(HALL_PIN));
  //player.midiDemoPlayer();
  player.play()
  delay(1000);
}
