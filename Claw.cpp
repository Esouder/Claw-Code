/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <Arduino.h>
#include <Servo.h>
#include "Claw.h"

/*******************************************************************************
 * Pins
 ******************************************************************************/

/*Servo*/
#define SERVO_PIN 9

/* Ultrasonic Sensor*/
#define US_TRIGGER_PIN 12
#define US_VCC_PIN 13
#define US_ECHO_PIN 11
#define US_GROUND_PIN 10

/*******************************************************************************
 * Other definitions, globals, and objects
 ******************************************************************************/
Servo servo;
#define US_MAX_DISTANCE 20

/*change DEBUG MODE to 0 for off and 1 for on*/
#define DEBUG_MODE 1
#define SERIAL_BAUDRATE 9600

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
double senseDistance(void);

/*******************************************************************************
 * Arduino Setup
 ******************************************************************************/
void Claw::setup(){
    /*Servo setup*/
    servo.attach(SERVO_PIN);

    /*debug mode setup*/
    if(DEBUG_MODE){
        Serial.begin(SERIAL_BAUDRATE);
    }

    /*Ultrasonic Sensor Setup*/
    pinMode(US_ECHO_PIN, INPUT);
    pinMode(US_TRIGGER_PIN, OUTPUT);
    pinMode(US_GROUND_PIN, OUTPUT);
    pinMode(US_VCC_PIN, OUTPUT);
    digitalWrite(US_GROUND_PIN,LOW);
    digitalWrite(US_VCC_PIN,HIGH);

}

/*******************************************************************************
 * Arduino Loop
 ******************************************************************************/
void Claw::loop(){
    /*sense distance*/
    double distance =senseDistance();

    /*calc and actuate servo*/
    double servoAngle = distance/20*180;
    servo.write(servoAngle);

    /*debug mode stuff, if applicable*/
    if(DEBUG_MODE){
        Serial.print("Distance: fart");
        //Serial.println(distance);
        Serial.print("Servo Angle: ");
        Serial.println(servoAngle);
    }
    
    /*main loop execution delay*/
    delay(15);

}

/*******************************************************************************
 * Function Implementations 
 ******************************************************************************/

/**
 * gets a reading from the HC-SRO4 sensor, calculates and returns the distance
 * @return distance in cm read by sensor
 */
double senseDistance(void){
    digitalWrite(US_TRIGGER_PIN,LOW);
    delayMicroseconds(2);
    digitalWrite(US_TRIGGER_PIN,HIGH);
    delayMicroseconds(20);
    digitalWrite(US_TRIGGER_PIN, LOW);

    double duration = pulseIn(US_ECHO_PIN,HIGH);
    double distance = duration*0.034/2;
    
    if(distance>US_MAX_DISTANCE){
        distance = US_MAX_DISTANCE;
    }

    return distance;
}