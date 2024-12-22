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

  mesh.sendTextMessage("hello", 10001);
}

void loop() {
  mesh.update();
}
