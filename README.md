# AKT!V LABOR

This project was developed in the frame of a multidisciplinary semester course with [Fu Berlin and KHB Berlin](https://www.fu-berlin.de) and [KHB Berlin](https://www.kh-berlin.de). The goal of this course was to design a neo-analogue product embedded in the context of "Smart City".

## CHALLANGE

To address this topic we defined the following challenge for our project:

> How can we sensitize the senses of school children through playful learning to strengthen their knowledge of mobility and skills?

## PRODUCTS

In response to this challenge, we developed three tool: Höribus, Orienta and Distanzo.

**Höribus** conveys the ability of directional hearing by simulating familiar street sounds inside the safe space of the school. Sounds sampled from the environment help children to practice identifying the direction and nature of the sounds and to serve as a basis for communicating and discussing potential dangers.

**Distanzo** is designed to train the children’s perception of distance, and at a later stage can be used to teach them an awareness of speed and braking distance. The tool facilitates a playful game in which distances can be estimated and checked immediately: kids find it easy and fun to use, either individually or as a team, thanks to the intuitive neo-analog interaction. 

**Orienta** assists the children to train their navigation and orientation skills by practicing paths, like their route to the school, while in a safe space. Digitally enhanced milestones are scattered around the school, communicating with the Orienta device to help kids learn the way.

Watch our product [video]() to get a better insight, how these tools work.

## HARDWARE & SOFTWARE

All tools content Ardunio hardware. To be able to run the code on these tools, the follwing hardware is needed for each tool.

#### Höribus

* Arduino Uno
* Adafruit Music Maker MP3 Shield
* Adafruit Mono Amplifier
* 3-axis Accelerometer
* SD card
* 9V Battery
* Speaker

#### Orienta

* Arduino Uno
* Adafruit Music Maker MP3 Shield
* Adafruit Mono Amplifier
* Hall sensor
* SD card
* 9V Battery
* Speaker
* Magnet

#### Distanzo

* Arduino Mini
* Lidar sensor
* 3-axis Accelerometer
* 4 digit display
* Switch
* 9V Battery

Watch our functional [video](https://vimeo.com/392459920) to better understand, how the hardware is tide together.

## INSTALLATION

All tools are programmed with Arduino. 

You first need to install the [Arduino IDE](https://www.arduino.cc/en/Main/Software) on your computer. Then you connect each tool with your computer and run the respective source code on them. For Höribus use the sound.ino, for Orienta the path.ino and for Distanzo the distance.ino file you find in the equally named folders.

## TEAM 

* Jacob Sasse (Product Design)
* Katharina Bellinger (Product Design)
* Mattis Obermann (Product Design)
* Marisa Nest (Computer Science)

Contact us via aktiv.labor@gmail.org.
