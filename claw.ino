/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <Servo.h>

/*******************************************************************************
 * Pinouts
 ******************************************************************************/

/*Servo Pins*/
#define SERVO_PIN 9

/*Ultrasonic Sensor Pins*/
#define US_VCC_PIN 13
#define US_TRIGGER_PIN 12
#define US_ECHO_PIN 11
#define US_GROUND_PIN 10


/*******************************************************************************
 * Definitions, Objects, and Globals
 ******************************************************************************/

Servo servo;
#define US_MAX_DISTANCE 20
#define SERVO_MAX_ANGLE 180
#define SERVO_MIN_ANGLE 0

/*change DEBUG MODE to 0 for off and 1 for on*/
#define DEBUG_MODE 1
#define SERIAL_BAUDRATE 9600

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/

double senseDistance(void);

void servoActuate(double angle);

/*******************************************************************************
 * Arduino Setup
 ******************************************************************************/
void setup(){
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
 * Main Loop
 ******************************************************************************/
void loop(){
    /*sense distance*/
    double distance =senseDistance();

    /*calc and actuate servo*/
    double servoAngle = distance/20*180;
    servoActuate(servoAngle);

    /*debug mode stuff, if applicable*/
    if(DEBUG_MODE){
        Serial.print("Distance: ");
        Serial.println(distance);
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

/**
 * Moves the servo to the specified angle within the defined range
 * @param angle the angle the servo is to be set to. 
 */
void servoActuate(double angle){
    if(angle<SERVO_MIN_ANGLE){
        angle=SERVO_MIN_ANGLE;
    }
    else if(angle>SERVO_MAX_ANGLE){
        angle=SERVO_MAX_ANGLE;
    }

    servo.write(angle);
}