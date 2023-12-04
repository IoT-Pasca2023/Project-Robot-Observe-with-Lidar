#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <ArduinoJson.h>

#define MOTOR_RIGHT 9
#define MOTOR_LEFT 8

#define analogVin A0          // Analog voltage input to A0

Adafruit_MCP4725 MCP4725;

int PWMvalue = 0;

void setup() {
  Serial.begin(115200);
  MCP4725.begin(0x60);

  pinMode(MOTOR_LEFT, OUTPUT);
  pinMode(MOTOR_RIGHT, OUTPUT);
}

void loop() {
  unsigned long t1, t2;
  // Data Reading
  if (Serial.available() > 0) {
    String data = Serial.readString();
    data.trim();
    // Serial.println(data);
    t1 = millis();

    // voltage conversion
    uint32_t MCP4725_value;
    int adcValueRead = 0;
    float voltageRead = 0;
    float MCP4725_expected_output;

    // MCP4725_value = 615; //0.75 volt
    // MCP4725_value = 410; // 0.5 volt
    MCP4725_value = 2048;

      
    MCP4725_expected_output = (5.0/4096.0) * MCP4725_value;
    MCP4725.setVoltage(MCP4725_value, false);
    delay(250);
    adcValueRead = analogRead(analogVin);
    voltageRead = (adcValueRead * 5.0 )/ 1024.0;

    // motor speed condition
    if (data == "forward") {
      t2 = millis();
      // Hitung response time
      unsigned long responseTime = t2 - t1;

      // Tampilkan response time
      Serial.println(responseTime);
      digitalWrite(MOTOR_LEFT, voltageRead);
      digitalWrite(MOTOR_RIGHT, voltageRead);
    } 
    else if (data == "stop") {
      t2 = millis();
      // Hitung response time
      unsigned long responseTime = t2 - t1;

      // Tampilkan response time
      Serial.println(responseTime);
      digitalWrite(MOTOR_LEFT, LOW);
      digitalWrite(MOTOR_RIGHT, LOW);
    } else if (data == "left") {
      t2 = millis();
      // Hitung response time
      unsigned long responseTime = t2 - t1;

      // Tampilkan response time
      Serial.println(responseTime);
      digitalWrite(MOTOR_LEFT, LOW);
      digitalWrite(MOTOR_RIGHT, voltageRead);
    } else if (data == "right") {
      t2 = millis();
      // Hitung response time
      unsigned long responseTime = t2 - t1;

      // Tampilkan response time
      Serial.println(responseTime);
      digitalWrite(MOTOR_LEFT, voltageRead);
      digitalWrite(MOTOR_RIGHT, LOW);
    }

  }
}
