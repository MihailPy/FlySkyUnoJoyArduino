
#include "UnoJoy.h"
#include "SoftwareSerial.h"
#include "FlySkyIBus.h"

SoftwareSerial MS(2,3);

void setup(){
  setupPins();
  setupUnoJoy();
  MS.begin(115200);
  //Serial.begin(9600);
  IBus.begin(MS);
}

void loop(){
  
  // Always be getting fresh data
  dataForController_t controllerData = getControllerData();
  setControllerData(controllerData);
  //Serial.print(controllerData.leftStickX);
  //Serial.print(controllerData.leftStickY);
  //Serial.print(controllerData.rightStickX);
  //Serial.print(controllerData.rightStickY);
  //Serial.println();
}

void setupPins(void){
  // Set all the digital pins as inputs
  // with the pull-up enabled, except for the 
  // two serial line pins
  for (int i = 2; i <= 12; i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(A4, INPUT);
  digitalWrite(A4, HIGH);
  pinMode(A5, INPUT);
  digitalWrite(A5, HIGH);
}

dataForController_t getControllerData(void){
  IBus.loop();
  
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  values that were in those memory locations before
  dataForController_t controllerData = getBlankDataForController();
  // Since our buttons are all held high and
  //  pulled low when pressed, we use the "!"
  //  operator to invert the readings from the pins
  controllerData.triangleOn = map(IBus.readChannel(6), 1000, 2000, 0, 1);
  controllerData.circleOn = map(IBus.readChannel(7), 1000, 2000, 0, 1);
  if (IBus.readChannel(8) > 1500){controllerData.squareOn = 1;}else {controllerData.squareOn = 0;}
  if (IBus.readChannel(8) < 1500){controllerData.crossOn = 1;}else {controllerData.crossOn = 0;}
  controllerData.dpadUpOn = map(IBus.readChannel(9), 1000, 2000, 0, 1);
  controllerData.dpadDownOn = !digitalRead(7);
  controllerData.dpadLeftOn = !digitalRead(8);
  controllerData.dpadRightOn = !digitalRead(9);
  controllerData.l1On = !digitalRead(10);
  controllerData.r1On = !digitalRead(11);
  controllerData.selectOn = !digitalRead(12);
  controllerData.startOn = !digitalRead(A4);
  controllerData.homeOn = !digitalRead(A5);
  
  // Set the analog sticks
  //  Since analogRead(pin) returns a 10 bit value,
  //  we need to perform a bit shift operation to
  //  lose the 2 least significant bits and get an
  //  8 bit number that we can use  
  controllerData.leftStickX = (map(IBus.readChannel(0), 1000, 2000, 0, 255));
  controllerData.leftStickY = (map(IBus.readChannel(1), 1000, 2000, 0, 255));
  controllerData.rightStickX = (map(IBus.readChannel(3), 1000, 2000, 0, 255));
  controllerData.rightStickY = (map(IBus.readChannel(2), 1000, 2000, 0, 255));
  // And return the data!
  return controllerData;
}
//byte stickValue(int rcVal) {
//  return map( constrain(rcVal - 1000, 0, 1000), 0, 1000, 0, 255);
//}
