#include <WiFi.h>
#include <WiFiUdp.h>


const char* ssid = "utexas-iot";
const char* password = "73023484672243220551";

const char* udpAddress = "10.159.65.96"; // host laptop IP, change on every new connection

WiFiUDP udp;
int keyboardPin = 3;
int mousePin = 4;
int personPin = 2;
int cellPhonePin = 23;

void setup() {

  
  // Set digital pins as input/outputs
  pinMode(personPin, OUTPUT);
  //pinMode(personPin, OUTPUT);

  Serial.begin(115200);
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  udp.begin(1234); // set the UDP port to listen on


}

void loop(){
  char packetBuffer[255];
  int packetSize = udp.parsePacket();
  
  if (packetSize) {
    // Read the packet into packetBuffer and get the remote IP address
    udp.read(packetBuffer, packetSize);
    IPAddress remoteIP = udp.remoteIP();
    Serial.print("Received packet from ");
    Serial.println(remoteIP);

    // Convert the received bytes to a string
    String packetString = "";
    for (int i = 0; i < packetSize; i++) {
      packetString += (char) packetBuffer[i];
    }

    // Determine which item was selected by object detector and turn the corresponding pin on or off based on input from MI
    if (packetString == "laptop") { 
      // Read input from MI
      readMI_Input(keyboardPin);
    } 
    else if (packetString == "keyboard") {
      // Read input from MI
      readMI_Input(keyboardPin);
    } 
    else if (packetString == "mouse") {
      // Read input from MI
      readMI_Input(mousePin);
    } 
    else if (packetString == "cell phone") {
      // Read input from MI
      readMI_Input(cellPhonePin);
    } 
    else if (packetString == "person") {
      // Read input from MI
      readMI_Input(personPin);
    } 
    else {
      Serial.println("Unknown input");
    }
  }
}


void readMI_Input(int pin) {
  // Wait for a packet from MI decoder
  while (true) {
    // Check if there is any incoming UDP packet
    int packetSize = udp.parsePacket();
    if (packetSize) {
      byte data[packetSize];
      int len = udp.read(data, packetSize);
      if (len > 0) {
        byte input = data[0];
        if (input == '0') {
          Serial.print("Received UDP: ");
          Serial.println((char)data[0]);
          
          if (pin != keyboardPin || pin !=  mousePin || pin != cellPhonePin) {
            digitalWrite(pin, LOW); // turn off device
          }
          break;
        }
        else if (input == '1') {
          Serial.print("Received UDP: ");
          Serial.println((char)data[0]);
          
          // keyboard control
          if (pin == keyboardPin) {
            Serial.println("Keyboard typed");
            udp.beginPacket(udpAddress, 1234);
            udp.print("type:hello world");
            udp.endPacket();
            delay(500);
          }
          // mouse control
          else if (pin == mousePin) {
            Serial.println("Mouse clicked");
            udp.beginPacket(udpAddress, 1234);
            udp.print("left_click");
            udp.endPacket();
            delay(500);
          }
          // phone control
          else if (pin == cellPhonePin ) {
            Serial.println("Text sent");
            delay(500);
          }
          // person 
          else {
            digitalWrite(pin, HIGH); // turn on device
          }
          break;
        }
      }
    }
  }
}
