#include <HardwareSerial.h>



// ID:   TO:   message:                                                       messageID
// 34566:64254:OKNDVKNDKNKJDFKNDFKNDFKNIEHGLSDMDVPDMEOLGVMDPJLDLDKWOKDPOSKJVL:6547


class MeshNetwork {
private:
  HardwareSerial HC12{1};

  int NodeID = 0;
  int DeviceRole = 0;

  const int rxPin = 17;  // RX Pin (receive data from HC-12)
  const int txPin = 16;  // TX Pin (send data to HC-12)

  const int SerialSpeed = 1200;

  int MessageIDListSize = 10;
  int MessageIDList[10];


  using MessageCallback = void (*)(String, String, String, String);
  MessageCallback onReceiveCallback;
  
public:
  const int CLIENT = 0;
  const int REPEATER = 1;
  
  
  
  
  void begin() {
      HC12.begin(SerialSpeed, SERIAL_8N1, rxPin, txPin);
  }
  
  

  void setNodeID(int n) {
    NodeID = n;
  }
  


  void setDeviceRole(int set_var) {
    DeviceRole = set_var;
  }
  
  

  void sendTextMessage(String message, int destinationNodeID) {
    randomSeed(analogRead(A0));
    String messageID = String(random(10, 10000));
    
    HC12.println(String(NodeID) + ":" + destinationNodeID + ":" + message + ":" + messageID);
    // Serial.println(String(NodeID) + ":" + destinationNodeID + ":" + message + ":" + messageID);
  }



  void onReceive(MessageCallback callback) {
    onReceiveCallback = callback;
  }



  void AddToList(int number) {
    for (int i = MessageIDListSize-1; i > 0; i--) {
      MessageIDList[i] = MessageIDList[i-1];
    }
    MessageIDList[0] = number;
  }

  

  bool IsNumberInList(int number) {
    for (int i = 0; i < MessageIDListSize; i++) {
      if (MessageIDList[i] == number) {
        return true;
      }
    }
    return false;
  }



  void received_CLIENT(String rec_ID, String rec_TO, String rec_message, String rec_messageID) {
    int number = rec_messageID.toInt();
    
    if (!IsNumberInList(number)) {
      AddToList(number);
      if (rec_TO.toInt() == NodeID) {
        onReceiveCallback(rec_ID, rec_TO, rec_message, rec_messageID);
      }
    }
  }



  void received_REPEATER(String rec_ID, String rec_TO, String rec_message, String rec_messageID) {
    int number = rec_messageID.toInt();
    
    if (!IsNumberInList(number)) {
      AddToList(number);
      if (rec_TO.toInt() == NodeID) {
        onReceiveCallback(rec_ID, rec_TO, rec_message, rec_messageID);
      } else {
        HC12.println(rec_ID + ":" + rec_TO + ":" + rec_message + ":" + rec_messageID);
      }
    }
  }


  

  void update() {
    if (HC12.available()) {
      String received = HC12.readStringUntil('\n');
      // Serial.println(received);


      String received_ID;
      String received_TO;
      String received_message;
      String received_messageID;
      
      int firstColon = received.indexOf(':');
      int secondColon = received.indexOf(':', firstColon + 1);
      int lastColon = received.lastIndexOf(':');
    
      if (firstColon != -1 && secondColon != -1 && lastColon != -1) {
        received_ID = received.substring(0, firstColon);
        received_TO = received.substring(firstColon + 1, secondColon);
        received_message = received.substring(secondColon + 1, lastColon);
        received_messageID = received.substring(lastColon + 1);


        
        
        if (DeviceRole == CLIENT) {
          received_CLIENT(received_ID, received_TO, received_message, received_messageID);
        } else if (DeviceRole == REPEATER) {
          received_REPEATER(received_ID, received_TO, received_message, received_messageID);
        }
      }
    }
  }
};
