#include <Servo.h>

/*Creator of the ultrasonic code I am using..*/
/*
 * created by Rui Santos, https://randomnerdtutorials.com
 * 
 * Complete Guide for Ultrasonic Sensor HC-SR04
 *
    Ultrasonic sensor Pins:
        VCC: +5VDC
        Trig : Trigger (INPUT) - Pin11
        Echo: Echo (OUTPUT) - Pin 12
        GND: GND
 */

/*
 * created by Matthew Braniff
 * 
 * This is my first Arduino project as part of University of Illinois in Chicago's IEEE Tinker program
 * 
 * Date: 11/16/2018
 * 
 * The purpose of this code is to play a small game of finding a random distance
 * between 1 and 38 inches. This game uses an ultrasonic sensor to measure distance 
 * and a servo motor to display how close you are to the mystery distance.
 * The closer you are to the distance the closer the servo is to 90 degrees. If you are
 * too close your angle is under 90 degrees and if you are over the angle is over 90 degrees,
 * depending on how far away you are, the larger the angle. When you find the distance it will
 * flip a boolean and make the servo do a little sprinkler movement, then creates a new random distance
 * and starts the game over again.
 * 
 */
 
int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo
long duration, distance, inches;

Servo servo;
int turn = 0;
bool distFound = false;
 
void setup() {
  //Setting a new random seed so distances are not predictable
  randomSeed(analogRead(0));
  
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Set servo input to pin 8 this is the yellow wire on the servo
  servo.attach(8);
  servo.write(0); //Initial servo angle 0
  delay(2000);

  //create a random Distance between 1 and 38 inches
  distance = random(37)+1; 
  Serial.print(distance);
  Serial.println();
}
 
void loop() {
  if(!distFound){ //This if blocks runs if you have no found the distance
    /***** The following is NOT my code, any code that has to do with the ultrasonic sensor is not mine!*****/
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
   
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
   
    // Convert the time into a distance
    inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
    
    Serial.print(inches);
    Serial.print("in");
    Serial.println();

    /***** Now this is my own code -Matt*****/

    //The first if checks if we are over the desired distance but also not if we are too far over
    //to avoid the servo constantly moving when the ultrasonic sensor gives a bad distance like 800in.
    if (inches > distance && !(inches > distance + 30)){ 
      turn = 180-(90*((double)distance/inches)); //Make the servo at an angle over 90 degrees
    }else if(inches < distance){ //If we are under the desired distance
      turn = (90*((double)inches/distance)); //Make the servo at an angle under 90 degrees
    }
    if(abs(inches-distance) < .001){ //If we are close enough (within .001in) to the desired distance
      distFound = true; //Flip the distFound boolean
    }
  
    servo.write(turn); //Set the servo angle to what ever turn is.
    
    delay(150);
  }else{ //This runs if the distFound boolean is true
    int val = 180;
    
    Serial.print("Distance Found!");
    Serial.println();
    
    servo.write(0); //Set servo angle to 0
    delay(1000);
    servo.write(180); //Set servo angle to 180
    
    //This loop subtracts 5 degrees from 180 and writes it to the servo until the angle is 0
    for(int i=0;i<36;i++){
      servo.write(val);
      delay(50);
      val -= 5;
    }//This is the sprinkler effect

    //Set a new mystery distance, print it to the serial monitor and flip the distFound boolean to false again
    distance = random(37)+1;
    Serial.print(distance);
    Serial.println();
    distFound = false;
  }
}
