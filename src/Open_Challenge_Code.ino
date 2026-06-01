#include <Servo.h>

// ===== Debug Toggle =====
#define DEBUG_OUTPUT true

// ===== Pins =====
const int TRIG_LEFT  = 6;
const int ECHO_LEFT  = 5;
const int TRIG_RIGHT = 4;
const int ECHO_RIGHT = 2;
const int SERVO_PIN  = 9;

// ===== Motor Driver (Cytron MD13S) =====
const int PWM_PIN = 3;   // PWM pin for motor speed
const int DIR_PIN = 8;   // Direction pin

// ===== Speed Settings =====
const int MOTOR_SPEED = 35;

// ===== PD Control Settings =====
const float KP = 1.1;
const float KD = 1.1;

// ===== Steering Angles =====
const int CENTER_ANGLE = 90;
const int MIN_SERVO_ANGLE = 40;
const int MAX_SERVO_ANGLE = 150;

// ===== Globals =====
Servo steeringServo;
float lastError = 0;

void setup() {
  pinMode(TRIG_LEFT, OUTPUT);
  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, HIGH);  // Forward

  steeringServo.attach(SERVO_PIN);
  steeringServo.write(CENTER_ANGLE);

  if (DEBUG_OUTPUT) {
    Serial.begin(115200);
    Serial.println("=== PD Steering with Accurate Ultrasonic Readings ===");
    Serial.println("Left(cm)\tRight(cm)\tError\tServo");
  }
}

void loop() {
  float leftDist = getStableDistance(TRIG_LEFT, ECHO_LEFT);
  float rightDist = getStableDistance(TRIG_RIGHT, ECHO_RIGHT);

  // Fallback if one sensor fails
  if (leftDist < 0 && rightDist >= 0) leftDist = rightDist;
  if (rightDist < 0 && leftDist >= 0) rightDist = leftDist;
  if (leftDist < 0 && rightDist < 0) {
    runMotor(0, false);
    if (DEBUG_OUTPUT) Serial.println("Both sensors failed — stopping");
    delay(100);
    return;
  }

  // PD Controller
  float error = leftDist - rightDist;
  float derivative = error - lastError;
  float pdOutput = KP * error + KD * derivative;

  int servoAngle = constrain(CENTER_ANGLE + pdOutput, MIN_SERVO_ANGLE, MAX_SERVO_ANGLE);
  steeringServo.write(servoAngle);
  lastError = error;

  // Run Motor
  runMotor(MOTOR_SPEED, false);

  // Debug Output
  if (DEBUG_OUTPUT) {
    Serial.print(leftDist, 1);
    Serial.print("\t\t");
    Serial.print(rightDist, 1);
    Serial.print("\t\t");
    Serial.print(error, 1);
    Serial.print("\t");
    Serial.println(servoAngle);
  }

  delay(100); // Slightly longer for accurate ultrasonic reading
}

// ===== Stable Distance Function =====
float getStableDistance(int trigPin, int echoPin) {
  float sum = 0;
  int validCount = 0;

  // Take 3 readings and average (helps with stability)
  for (int i = 0; i < 3; i++) {
    float dist = getDistance(trigPin, echoPin);
    if (dist > 0) {
      sum += dist;
      validCount++;
    }
    delay(10);
  }

  if (validCount == 0) return -1;
  return sum / validCount;
}

// ===== Single Reading =====
float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 40000); // 40ms timeout
  if (duration == 0) return -1;
  return duration * 0.034 / 2.0;
}

// ===== Motor Run Function =====
void runMotor(int speed, bool reverse) {
  digitalWrite(DIR_PIN, reverse ? LOW : HIGH);
  analogWrite(PWM_PIN, speed);
}
