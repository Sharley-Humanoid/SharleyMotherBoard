// =================================
//  SHARLEY MOTHERBOARD 1 - MASTER
//  https://www.sharley.tech

//  IMPORTANT: CHANGE WIFI DETAILS WITH YOURS AND CHANGE SLAVE IP

//  RUN SLAVE MOTHERBOARD/MOTHERBOARD 2 FIRST TO OBTAIN IP
// ==================================

#include <WiFi.h>
#include <ESP32Servo.h>
#include <WebServer.h>
#include <HTTPClient.h>


const char* ssid = "SSID";          // SSID OF WIFI
const char* password = "PASSWORD";  // PASSWORD OF WIFI


String slaveIP = "SLAVE IP";  // SLAVE IP
WebServer server(80);


Servo servos[4];
Servo headServo;


int servoPins[4] = { 32, 33, 23, 12 };
int headPin = 27;
int servoPositions[4] = { 90, 90, 90, 90 };
int headPos = 90;


unsigned long lastWiFiCheck = 0;
const unsigned long wifiCheckInterval = 5000;


// ======================= SERVO FUNCTIONS =======================
void moveServosSmooth(int targetPositions[4], int speed = 15) {
  bool moving = true;
  while (moving) {
    moving = false;
    for (int i = 0; i < 4; i++) {
      if (servoPositions[i] != targetPositions[i]) {
        if (servoPositions[i] < targetPositions[i]) servoPositions[i]++;
        else if (servoPositions[i] > targetPositions[i]) servoPositions[i]--;
        servos[i].write(servoPositions[i]);
        moving = true;
      }
    }
    delay(speed);
  }
}


void moveHead(int target, int speed = 15) {
  while (headPos != target) {
    if (headPos < target) headPos++;
    else if (headPos > target) headPos--;
    headServo.write(headPos);
    delay(speed);
  }
}


// ======================= NETWORK FUNCTIONS =======================
String sendCommandToSlave(String path) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected, skipping slave command");
    return "";
  }


  HTTPClient http;
  String url = "http://" + slaveIP + path;
  http.begin(url);
  http.setTimeout(2000);


  int httpCode = http.GET();
  String payload = "";
  if (httpCode > 0) {
    payload = http.getString();
  } else {
    Serial.println("Failed to reach slave: " + url);
  }
  http.end();
  return payload;
}


void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);


  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }


  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi connection failed! Running in offline mode.");
  }
}


void ensureWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi Lost! Reconnecting...");
    connectToWiFi();
  }
}


void handleRoot() {
  String html = "<html><head><title>Sharley</title></head><body>";
  html += "<h1>Sharley Control Panel</h1>";
  html += "<p><b>Arms:</b></p>";
  html += "<button onclick=\"fetch('/open_arms')\">Open Arms</button>";
  html += "<button onclick=\"fetch('/close_arms')\">Close Arms</button><br><br>";
  html += "<p><b>Head:</b></p>";
  html += "<button onclick=\"fetch('/head_left')\">Left</button>";
  html += "<button onclick=\"fetch('/head_center')\">Center</button>";
  html += "<button onclick=\"fetch('/head_right')\">Right</button>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}


void handleOpenArms() {
  server.send(200, "text/plain", "open");
  String resp = sendCommandToSlave("/move_normal?action=open");
  if (resp == "OK" || resp == "") {
    int target[4] = { 0, 180, 0, 180 };
    moveServosSmooth(target);
    Serial.println("✅ Arms opened!");
  } else {
    Serial.println("❌ Slave failed to respond");
  }
}


void handleCloseArms() {
  server.send(200, "text/plain", "close");
  String resp = sendCommandToSlave("/move_normal?action=close");
  if (resp == "OK" || resp == "") {
    int target[4] = { 90, 90, 90, 90 };
    moveServosSmooth(target);
    Serial.println("✅ Arms closed!");
  } else {
    Serial.println("❌ Slave failed to respond");
  }
}


void handleHeadLeft() {
  moveHead(30);
  server.send(200, "text/plain", "Head left");
}


void handleHeadCenter() {
  moveHead(90);
  server.send(200, "text/plain", "Head centered");
}


void handleHeadRight() {
  moveHead(150);
  server.send(200, "text/plain", "Head right");
}


// ======================= SETUP & LOOP =======================
void setup() {
  Serial.begin(115200);
  Serial.println("BOOTING...");
  for (int i = 0; i < 4; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(servoPositions[i]);
  }
  headServo.attach(headPin);
  headServo.write(headPos);
  connectToWiFi();
  server.on("/", handleRoot);
  server.on("/open_arms", handleOpenArms);
  server.on("/close_arms", handleCloseArms);
  server.on("/head_left", handleHeadLeft);
  server.on("/head_center", handleHeadCenter);
  server.on("/head_right", handleHeadRight);
  server.begin();
}


void loop() {
  server.handleClient();
  if (millis() - lastWiFiCheck > wifiCheckInterval) {
    lastWiFiCheck = millis();
    ensureWiFi();
  }
}
