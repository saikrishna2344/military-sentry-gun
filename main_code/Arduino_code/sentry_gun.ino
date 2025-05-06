#include <Servo.h>
#include <Ultrasonic.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Pin Definitions ---
#define PIR_PIN 2
#define FIRE_SERVO_PIN 3
#define GRIPPER_SERVO_PIN 4
#define IR_SENSOR_LEFT 5
#define IR_SENSOR_RIGHT 6
#define TRIG_PIN 7
#define ECHO_PIN 8
#define BUZZER_PIN 9
#define LED_PIN 10
#define MINE_SENSOR_PIN A0
#define MOTOR_LEFT_FORWARD 11
#define MOTOR_LEFT_BACKWARD 12
#define MOTOR_RIGHT_FORWARD A1
#define MOTOR_RIGHT_BACKWARD A2

// --- LCD Display Setup ---
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Servo and Ultrasonic Setup ---
Servo fireServo;
Servo gripperServo;
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

// --- System Variables ---
unsigned long lastDetectionTime = 0;
bool enemyDetected = false;
int systemMode = 0;
bool mineDetected = false;
int mineValue = 0;

// --- Additional Variables ---
int enemyCountdown = 10;
int obstacleDetected = 0;
bool obstacleAvoided = false;

void setup() {
  Serial.begin(9600);

  // Initialize pins
  pinMode(PIR_PIN, INPUT);
  pinMode(IR_SENSOR_LEFT, INPUT);
  pinMode(IR_SENSOR_RIGHT, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_LEFT_FORWARD, OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD, OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD, OUTPUT);

  // Attach servos
  fireServo.attach(FIRE_SERVO_PIN);
  gripperServo.attach(GRIPPER_SERVO_PIN);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setBacklight(1);

  // Start system in patrol mode
  fireServo.write(0);
  gripperServo.write(0);

  // Print to Serial Monitor
  Serial.println("[SYSTEM] Military Sentry Gun Initialized");
  lcd.print("Sentry Gun Online");
}

void loop() {
  // Check system mode and decide actions
  switch (systemMode) {
    case 0:
      patrolMode();
      break;
    case 1:
      defenseMode();
      break;
    case 2:
      mineDetectionMode();
      break;
    case 3:
      enemyAlertMode();
      break;
    default:
      patrolMode();
      break;
  }

  // Update system status on LCD
  updateLCD();

  delay(100);
}

void patrolMode() {
  int irLeft = digitalRead(IR_SENSOR_LEFT);
  int irRight = digitalRead(IR_SENSOR_RIGHT);
  long distance = ultrasonic.read();
  mineValue = analogRead(MINE_SENSOR_PIN);
  int pirState = digitalRead(PIR_PIN);

  // Detect enemy presence using PIR sensor
  if (pirState == HIGH) {
    enemyDetected = true;
    lastDetectionTime = millis();
    Serial.println("[PATROL] Enemy detected!");
    systemMode = 3;
    return;
  }

  // Detect landmine using analog sensor
  if (mineValue > 500) {
    mineDetected = true;
    systemMode = 2;
    return;
  }

  // If no enemy detected, move forward
  if (irLeft == LOW || irRight == LOW || distance < 15) {
    obstacleDetected = 1;
    avoidObstacle(irLeft, irRight);
  } else {
    obstacleDetected = 0;
    moveForward();
  }
}

void defenseMode() {
  alertResponse();
  sprayWater();
  delay(3000);
  systemMode = 0;
}

void mineDetectionMode() {
  stopMovement();
  detectAndRemoveMine();
  delay(3000);
  systemMode = 0;
}

void enemyAlertMode() {
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  systemMode = 0;
}

void alertResponse() {
  lcd.clear();
  lcd.print("Enemy Alert!");
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  delay(500);
}

void sprayWater() {
  lcd.clear();
  lcd.print("Spraying Water...");
  for (int angle = 0; angle <= 90; angle += 5) {
    fireServo.write(angle);
    delay(100);
  }
  for (int angle = 90; angle >= 0; angle -= 5) {
    fireServo.write(angle);
    delay(100);
  }
  Serial.println("[DEFENSE] Water sprayed");
}

void detectAndRemoveMine() {
  lcd.clear();
  lcd.print("Removing Mine...");
  for (int pos = 0; pos <= 45; pos++) {
    gripperServo.write(pos);
    delay(50);
  }
  for (int pos = 45; pos >= 0; pos--) {
    gripperServo.write(pos);
    delay(50);
  }
  Serial.println("[MINE] Gripper action complete");
}

void moveForward() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  Serial.println("[MOVE] Forward");
}

void avoidObstacle(int left, int right) {
  stopMovement();
  if (left == LOW && right == HIGH) {
    turnRight();
  } else if (right == LOW && left == HIGH) {
    turnLeft();
  } else {
    reverse();
  }
}

void turnLeft() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  Serial.println("[TURN] Left");
  delay(600);
}

void turnRight() {
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  Serial.println("[TURN] Right");
  delay(600);
}

void reverse() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, HIGH);
  Serial.println("[MOVE] Reverse");
  delay(800);
}

void stopMovement() {
  digitalWrite(MOTOR_LEFT_FORWARD, LOW);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);
  digitalWrite(MOTOR_RIGHT_FORWARD, LOW);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  Serial.println("[MOVE] Stop");
}

void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Mode: ");
  switch (systemMode) {
    case 0:
      lcd.print("Patrol");
      break;
    case 1:
      lcd.print("Defense");
      break;
    case 2:
      lcd.print("Mine");
      break;
    case 3:
      lcd.print("Enemy Alert");
      break;
    default:
      lcd.print("Unknown");
      break;
  }

  lcd.setCursor(0, 1);
  lcd.print("Status: ");
  if (enemyDetected) {
    lcd.print("Enemy Detected");
  } else {
    lcd.print("Normal");
  }
}
