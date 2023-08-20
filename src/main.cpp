#include <Arduino.h>
#include <ESP32Servo.h>

#define SERIAL_FREQ 115200
#define SERVO_PIN 18
#define LEFT_PIN 22
#define RIGHT_PIN 23
#define POTENTIOMETER_PIN 32
#define ADC_MAX 4096

int btnSpeed = 5;
int pos = 0;
int lastVal = 0;
int val = 0;

Servo motor;

void setup() {
    Serial.begin(SERIAL_FREQ);
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    motor.setPeriodHertz (50);
    motor.attach(SERVO_PIN, 500, 2400);

    // Lighting indicates code has begun
    pinMode(2, OUTPUT);
    for (int i = 0; i < 3; i++) { 
        digitalWrite(2, HIGH);
        delay(200);
        digitalWrite(2, LOW);
        delay(200);
    }

    // Read Potentiometer initial value
    val = analogRead(POTENTIOMETER_PIN);
    val = map(val, 0, ADC_MAX, 0, 180); 
    lastVal = val;

    // Sync servo position with the pot initial value
    motor.write(val);
    // Set current position to this value
    pos = val;

    pinMode(LEFT_PIN, INPUT);
    pinMode(RIGHT_PIN, INPUT); 
    delay(10);
}

void loop() {
    // Potentiometer control and Switch buttons
    val = analogRead(POTENTIOMETER_PIN);
    val = map(val, 0, ADC_MAX, 0, 180); 

    int diff = abs(val - lastVal);

    // Calculation of difference to put error space
    if (diff > 5) {
        lastVal = val;
        pos = val;
        motor.write(val);
        delay(20);
    }

    while (digitalRead(LEFT_PIN) == 1 && pos >= 0){ 
        delay(100);

        if (digitalRead(LEFT_PIN) == 1) {
            int stepSize = pos - btnSpeed;
            if (stepSize < 0) {
                pos = 0;
            } else {
                pos = stepSize;
            }
            motor.write(pos);
        }
    }

    while (digitalRead(RIGHT_PIN) == 1 && pos <= 180){ 
        delay(100);

        if (digitalRead(RIGHT_PIN) == 1){
            int stepSize = pos + btnSpeed;
            if (stepSize > 180) {
                pos = 180;
            } else {
                pos = stepSize;
            }
            motor.write(pos);
        }    
    }
}