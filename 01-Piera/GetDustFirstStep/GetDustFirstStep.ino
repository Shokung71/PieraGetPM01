#include <Arduino.h>

#define RXPIN 26
#define TXPIN 33

String serial2Out;

void setup() {
  // เริ่มต้นการติดต่อกับ Serial Monitor และ Serial2
  Serial.begin(115200);  // ใช้ USB Serial
  delay(500);            // หน่วงเวลาเพื่อให้ Serial พร้อม
  Serial2.begin(115200, SERIAL_8N1, RXPIN, TXPIN);   // กำหนด Serial2 สำหรับอ่านฝุ่น

  Serial.println("Dust Sensor Setup Complete");
}

void loop() {
  // อ่านข้อมูลจาก Serial2 (ฝุ่น)
  if (Serial2.available()) {
    serial2Out = Serial2.readStringUntil('\n');
    Serial.println("Dust Sensor Data: ");
    Serial.println(serial2Out);  // ส่งข้อมูลฝุ่นไปยัง Serial Monitor
  }

  delay(1000);  // หน่วงเวลา 1 วินาทีระหว่างการอ่านข้อมูล
}