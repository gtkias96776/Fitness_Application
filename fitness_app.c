 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

BlynkTimer timer;
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "WIFI_PASSWORD";

Adafruit_MPU6050 mpu;


int stepCount = 0;
bool stepDetected = false;
float previousZ = 0;
float stepThreshold = 1.0; // Step detection threshold
int dailyStepTarget = 1000; // Daily step goal

int squatCount = 0;
bool squatDetected = false;
int dailySquatTarget = 50;

int jumpingJackCount = 0;
bool jumpingJackDetected = false;
int dailyJumpingJackTarget = 50;

int pushUpCount = 0;
bool pushUpDetected = false;
int dailyPushUpTarget = 50;

int sitUpCount = 0;
bool sitUpDetected = false;
int dailySitUpTarget = 50;

unsigned long lastMessageTime = 0;  // Last message display time
const unsigned long messageDisplayDelay = 1000;  // Delay of 1 second

void setup() {
  Serial.begin(9600); // Adjusted baud rate for ESP8266
  while (!Serial)
    delay(10);

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, sendsensor);
  Serial.println("");
  delay(100);
}

void updateExerciseMessage(String message) {
  unsigned long currentTime = millis();
  if (currentTime - lastMessageTime >= messageDisplayDelay) {
    Blynk.virtualWrite(V12, message);
    lastMessageTime = currentTime;
  }
}

void detectStep(float accelerationZ) {
  if (abs(accelerationZ - previousZ) > stepThreshold && !stepDetected) {
    stepCount++;
    stepDetected = true;
    Serial.print("Step detected! Total steps: ");
    Serial.println(stepCount);
    updateExerciseMessage("Step detected!");
  }
  if (abs(accelerationZ - previousZ) < (stepThreshold / 2)) {
    stepDetected = false;
  }
  previousZ = accelerationZ;

  if (stepCount >= dailyStepTarget) {
    Blynk.virtualWrite(V13, 255);  // Turn on LED
  }
}

void detectSquat(float accelerationZ) {
  if (accelerationZ < -8 && !squatDetected) {
    squatCount++;
    squatDetected = true;
    Serial.print("Squat detected! Total squats: ");
    Serial.println(squatCount);
    updateExerciseMessage("Squat detected!");
  }
  if (accelerationZ > -3) {
    squatDetected = false;
  }

  if (squatCount >= dailySquatTarget) {
    Blynk.virtualWrite(V13, 255);  // Turn on LED
  }
}

void detectJumpingJack(float accelerationX, float accelerationZ) {
  if (accelerationZ > 8 && abs(accelerationX) > 2 && !jumpingJackDetected) {
    jumpingJackCount++;
    jumpingJackDetected = true;
    Serial.print("Jumping Jack detected! Total jumping jacks: ");
    Serial.println(jumpingJackCount);
    updateExerciseMessage("Jumping Jack detected!");
  }
  if (accelerationZ < 5) {
    jumpingJackDetected = false;
  }

  if (jumpingJackCount >= dailyJumpingJackTarget) {
    Blynk.virtualWrite(V13, 255);  // Turn on LED
  }
}

void detectPushUp(float gyroX, float gyroY) {
  if (gyroY > 2 && !pushUpDetected) {
    pushUpCount++;
    pushUpDetected = true;
    Serial.print("Push-up detected! Total push-ups: ");
    Serial.println(pushUpCount);
    updateExerciseMessage("Push-up detected!");
  }
  if (gyroY < 1) {
    pushUpDetected = false;
  }

  if (pushUpCount >= dailyPushUpTarget) {
    Blynk.virtualWrite(V13, 255);  // Turn on LED
  }
}

void detectSitUp(float gyroX, float accelerationZ) {
  if (gyroX > 5 && accelerationZ < 0 && !sitUpDetected) {
    sitUpCount++;
    sitUpDetected = true;
    Serial.print("Sit-up detected! Total sit-ups: ");
    Serial.println(sitUpCount);
    updateExerciseMessage("Sit-up detected!");
  }
  if (gyroX < 1) {
    sitUpDetected = false;
  }

  if (sitUpCount >= dailySitUpTarget) {
    Blynk.virtualWrite(V13, 255);  // Turn on LED
  }
}

void sendsensor() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float accelerationX = a.acceleration.x;
  float accelerationY = a.acceleration.y;
  float accelerationZ = a.acceleration.z;
  float gyroX = g.gyro.x;
  float gyroY = g.gyro.y;
  float gyroZ = g.gyro.z;

  detectStep(accelerationZ);
  detectSquat(accelerationZ);
  detectJumpingJack(accelerationX, accelerationZ);
  detectPushUp(gyroX, gyroY);
  detectSitUp(gyroX, accelerationZ);

  Serial.print("Acceleration X: ");
  Serial.print(accelerationX);
  Serial.print(", Y: ");
  Serial.print(accelerationY);
  Serial.print(", Z: ");
  Serial.println(accelerationZ);

  Serial.print("Rotation X: ");
  Serial.print(gyroX);
  Serial.print(", Y: ");
  Serial.print(gyroY);
  Serial.print(", Z: ");
  Serial.println(gyroZ);

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Blynk.virtualWrite(V0, accelerationX);
  Blynk.virtualWrite(V1, gyroX);
  Blynk.virtualWrite(V2, temp.temperature);
  Blynk.virtualWrite(V3, accelerationZ);
  Blynk.virtualWrite(V4, accelerationY);
  Blynk.virtualWrite(V5, gyroY);
  Blynk.virtualWrite(V6, stepCount);
  Blynk.virtualWrite(V7, squatCount);
  Blynk.virtualWrite(V8, jumpingJackCount);
  Blynk.virtualWrite(V9, pushUpCount);
  Blynk.virtualWrite(V10, sitUpCount);
  Blynk.virtualWrite(V11, gyroZ);

  Serial.println("");
  delay(500);
}

void loop() {
  Blynk.run();
  timer.run();
}
