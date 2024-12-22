#include "MeshNetwork.h"



MeshNetwork mesh;

void MyMessage(String received_ID, String received_TO, String received_message, String received_messageID) {
  Serial.println("New Message:");
  Serial.println("    ID:        " + received_ID);
  Serial.println("    TO:        " + received_TO);
  Serial.println("    Message:   " + received_message);
  Serial.println("    MessageID: " + received_messageID);
}

void setup() {
  Serial.begin(9600);
  
  mesh.begin();
  mesh.setNodeID(10002);
  mesh.setDeviceRole(mesh.REPEATER);// REPEATER/CLIENT

  mesh.onReceive(MyMessage);

  mesh.sendTextMessage("hello:jeg:er:kul", 10001);
}

void loop() {
  mesh.update();
}















/*#include <HardwareSerial.h>

HardwareSerial HC12(1);

const int rxPin = 17;  // RX Pin (receive data from HC-12)
const int txPin = 16;  // TX Pin (send data to HC-12)

void setup() {
  Serial.begin(9600);  // Serial Monitor
  
  HC12.begin(1200, SERIAL_8N1, rxPin, txPin);  // HC-12 Serial
  
  Serial.println("ESP32 to HC-12 communication test");
}

void loop() {
  // Read from HC-12 and print to Serial Monitor
  if (HC12.available()) {
    String received = HC12.readStringUntil('\n');  // Read data until newline
    Serial.print("Received from HC-12: ");
    Serial.println(received);
  }

  // Read from Serial Monitor and send to HC-12
  if (Serial.available()) {
    String received = Serial.readStringUntil('\n');  // Read data from Serial Monitor
    HC12.println(received);  // Send to HC-12
  }
}
*/
