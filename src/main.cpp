#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>

// Custom libs
#include "web_page.h"
void sendStatic();

#define SERIAL_FREQ 115200
#define SERVO_PIN 18
#define LEFT_PIN 22
#define RIGHT_PIN 23
#define POTENTIOMETER_PIN 32
#define ADC_MAX 4096
#define ESP_AP 1

int btnSpeed = 5;
int pos = 0;
int lastVal = 0;
int val = 0;

const char SSID[] = "ESP AP";
const char password[] = "#password";

//-------------------------------
// Main program Objects
Servo motor;
WebServer Server(80);
//-------------------------------

void setup()
{
    Serial.begin(SERIAL_FREQ);
    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    motor.setPeriodHertz(50);
    motor.attach(SERVO_PIN, 500, 2400);

    if (ESP_AP) {
        // Setting up ESP Access point and Server
        Serial.println();
        Serial.println();
        Serial.print("starting ");
        Serial.print(SSID);
        Serial.print(" Access Point.");

        WiFi.softAP(SSID, password);

        Serial.println("");
        Serial.print("IP address: ");
        Serial.println(WiFi.softAPIP());
    } else {
        // In case of connecting to local network
        WiFi.begin(SSID, password);
        while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(500);
        }
        WiFi.mode(WIFI_STA);
        Serial.println();
        Serial.print("Local IP: ");
        Serial.println(WiFi.localIP());
    }

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

    Server.on("/", sendStatic);
    Server.begin();
}

void loop()
{
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

    while (digitalRead(LEFT_PIN) == 1 && pos >= 0) {
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

    while (digitalRead(RIGHT_PIN) == 1 && pos <= 180) {
        delay(100);

        if (digitalRead(RIGHT_PIN) == 1) {
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

void sendStatic()  {
    webPageCode.replace("__posVal", String(pos));
    webPageCode.replace("__currSpeedVal", String(btnSpeed));

    Server.send(200, "text/html", webPageCode);
}