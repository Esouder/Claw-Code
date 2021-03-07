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

enum clawState_e{
    open,
    closed
};

#define TRUE 1
#define FALSE 0

Servo servo;
#define US_MAX_DISTANCE 20 //max distance for the distance sensor to output
#define SERVO_MAX_ANGLE 180 //max angle for the servo to turn to
#define SERVO_MIN_ANGLE 50 //min angle for the servo to turn to
#define SERVO_OPEN 180 //angle of the servo when the claw is open
#define SERVO_CLOSED 50 //angle of the servo when the claw is closed
#define US_TRIGGER_ALT 0 // altitude to trigger opening/closing the claw

/*change DEBUG MODE to 0 for off and 1 for on*/
#define DEBUG_MODE 1
#define SERIAL_BAUDRATE 9600

clawState_e clawState = open;
double servoAngle = SERVO_OPEN;

int beenElevated = FALSE;

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

    
    if(distance <=US_TRIGGER_ALT){
        if(clawState==closed&&beenElevated ==TRUE){
            clawState=open;
            beenElevated = FALSE;
        }
        else if(clawState ==open&&beenElevated==TRUE){
            clawState=closed;
            beenElevated=FALSE;
        }
    }
    else{
        beenElevated = TRUE;
    }

    if(clawState==open){
        servoAngle=SERVO_OPEN;
    }
    else if (clawState==closed){
        servoAngle = SERVO_CLOSED;
    }

    /*actuate servo*/
    servoActuate(servoAngle);
    

    /*debug mode stuff, if applicable*/
    if(DEBUG_MODE){
        Serial.print("Distance: ");
        Serial.println(distance);
        Serial.print("Servo Angle: ");
        Serial.println(servoAngle);
        Serial.print("Claw State: ");
        Serial.println(clawState);
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
    
    if(distance>=500){
        distance=0;
    }
    else if(distance>US_MAX_DISTANCE){
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