#define BLYNK_TEMPLATE_ID "TMPL6ehGO3Dnj"
#define BLYNK_TEMPLATE_NAME "Intelligent Traffic Management System"
#define BLYNK_AUTH_TOKEN "U91eW2WYtec8TXwgyFu60vvMDcjpq5JX"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// Constants
#define IR_SENSOR_1_LANE_1 35 // IR Sensor 1 (Lane 1)
#define IR_SENSOR_2_LANE_1 34 // IR Sensor 2 (Lane 1)
#define IR_SENSOR_1_LANE_2 39 // IR Sensor 1 (Lane 2)
#define IR_SENSOR_2_LANE_2 36 // IR Sensor 2 (Lane 2)

#define CAM_TRIGGER_LANE_1 5  // Trigger pin for (Lane 1)
#define CAM_TRIGGER_LANE_2 19 // Trigger pin for (Lane 2)

// LED pins
const int redLED = 14;    
const int yellowLED = 25; 
const int greenLED = 13; 

// Servo pin
Servo gateServo;
const int servoPin = 4; // D4

#define DISTANCE_CM 50.0    // Distance in cm
#define SPEED_LIMIT_KMPH 15.0

// WiFi credentials
char ssid[] = "UIU-STUDENT";
char pass[] = "12345678";

// Variables
unsigned long entryTimeLane1 = 0, exitTimeLane1 = 0;
unsigned long entryTimeLane2 = 0, exitTimeLane2 = 0;

bool lane1Sensor1State = HIGH;
bool lane1Sensor2State = HIGH;
bool lane2Sensor1State = HIGH;
bool lane2Sensor2State = HIGH;

bool isGateLowered = false;

// Blynk virtual pins
#define RED_LED_VPIN V2
#define YELLOW_LED_VPIN V1
#define GREEN_LED_VPIN V0
#define GATE_VPIN V3

// Blynk handlers for LEDs
BLYNK_WRITE(RED_LED_VPIN) {
  digitalWrite(redLED, param.asInt()); // Turn Red LED on/off
}

BLYNK_WRITE(YELLOW_LED_VPIN) {
  digitalWrite(yellowLED, param.asInt()); // Turn Yellow LED on/off
}

BLYNK_WRITE(GREEN_LED_VPIN) {
  digitalWrite(greenLED, param.asInt()); // Turn Green LED on/off
}

// Blynk handler for the gate
BLYNK_WRITE(GATE_VPIN) {
  if (param.asInt() == 1) { // Button pressed
    isGateLowered = true;
    gateServo.write(90);  // Move servo to 90 degrees
    //digitalWrite(redLED, HIGH); // Turn Red LED on
    digitalWrite(redLED, LOW); // Turn Red LED off
    Serial.println("Gate lowered.");
  } else { // Button released
    isGateLowered = false;
    gateServo.write(0);   // Move servo back to 0 degrees
    //digitalWrite(redLED, LOW); // Turn Red LED off
    digitalWrite(redLED, HIGH); // Turn Red LED on
    Serial.println("Gate raised.");
  }
}

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);
  Serial.println("Setup started...");

  // Initialize LEDs as output
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Initialize IR sensor pins
  pinMode(IR_SENSOR_1_LANE_1, INPUT);
  pinMode(IR_SENSOR_2_LANE_1, INPUT);
  pinMode(IR_SENSOR_1_LANE_2, INPUT);
  pinMode(IR_SENSOR_2_LANE_2, INPUT);

  // Initialize CAM trigger pins
  pinMode(CAM_TRIGGER_LANE_1, OUTPUT);
  pinMode(CAM_TRIGGER_LANE_2, OUTPUT);
  digitalWrite(CAM_TRIGGER_LANE_1, LOW);
  digitalWrite(CAM_TRIGGER_LANE_2, LOW);

  // Attach servo
  gateServo.attach(servoPin);

  // Debug WiFi Connection
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Debug Blynk connection
  Serial.println("Connected to Blynk!");

  // Set initial state
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  gateServo.write(0); // Start with gate open

  Serial.println("Setup completed!");
}

void loop() {
  Blynk.run(); // Run Blynk

  // Lane 1: Detect state change for IR Sensor 1 and 2
  bool currentStateLane1Sensor1 = digitalRead(IR_SENSOR_1_LANE_1);
  bool currentStateLane1Sensor2 = digitalRead(IR_SENSOR_2_LANE_1);

  if (lane1Sensor1State == HIGH && currentStateLane1Sensor1 == LOW) {
    entryTimeLane1 = millis(); // Record entry time
    Serial.println("Lane 1 Sensor 1 triggered!");
  }

  if (lane1Sensor2State == HIGH && currentStateLane1Sensor2 == LOW) {
    exitTimeLane1 = millis(); // Record exit time
    Serial.println("Lane 1 Sensor 2 triggered!");
    calculateSpeedAndCheck(entryTimeLane1, exitTimeLane1, 1); // Calculate speed for Lane 1
  }

  lane1Sensor1State = currentStateLane1Sensor1;
  lane1Sensor2State = currentStateLane1Sensor2;

  // Lane 2: Detect state change for IR Sensor 1 and 2
  bool currentStateLane2Sensor1 = digitalRead(IR_SENSOR_1_LANE_2);
  bool currentStateLane2Sensor2 = digitalRead(IR_SENSOR_2_LANE_2);

  if (lane2Sensor1State == HIGH && currentStateLane2Sensor1 == LOW) {
    entryTimeLane2 = millis(); // Record entry time
    Serial.println("Lane 2 Sensor 1 triggered!");
  }

  if (lane2Sensor2State == HIGH && currentStateLane2Sensor2 == LOW) {
    exitTimeLane2 = millis(); // Record exit time
    Serial.println("Lane 2 Sensor 2 triggered!");
    calculateSpeedAndCheck(entryTimeLane2, exitTimeLane2, 2); // Calculate speed for Lane 2
  }

  lane2Sensor1State = currentStateLane2Sensor1;
  lane2Sensor2State = currentStateLane2Sensor2;
}

// Function to calculate speed and check against the speed limit
void calculateSpeedAndCheck(unsigned long entryTime, unsigned long exitTime, int lane) {
  if (entryTime == 0 || exitTime == 0) return; // Ensure valid timing data

  float timeTaken = (exitTime - entryTime) / 1000.0; // Time in seconds
  float speedMps = DISTANCE_CM / 100.0 / timeTaken; // Speed in meters per second
  float speedKmph = speedMps * 3.6; // Convert to km/h

  Serial.print("Lane ");
  Serial.print(lane);
  Serial.print(": Speed = ");
  Serial.print(speedKmph);
  Serial.println(" km/h");

  if (speedKmph > SPEED_LIMIT_KMPH) {
    Serial.print("Vehicle on Lane ");
    Serial.print(lane);
    Serial.println(" exceeded the speed limit!");

    // Trigger the respective ESP32-CAM
    if (lane == 1) {
      digitalWrite(CAM_TRIGGER_LANE_1, HIGH);
      delay(100); // Short pulse
      digitalWrite(CAM_TRIGGER_LANE_1, LOW);
    } else if (lane == 2) {
      digitalWrite(CAM_TRIGGER_LANE_2, HIGH);
      delay(100); // Short pulse
      digitalWrite(CAM_TRIGGER_LANE_2, LOW);
    }
  }
}
