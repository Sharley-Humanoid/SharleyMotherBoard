// =================================
//  SHARLEY MOTHERBOARD 2 - SLAVE
//  https://www.sharley.tech

//  IMPORTANT: CHANGE WIFI DETAILS WITH YOURS

//  PUT IP OBTAINED BY SLAVE IN MOTHERBOARD'S CODE
//  IP WILL BE PRINTED IN SERIAL MONITOR AT 115200
// ==================================


#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

const char* ssid = "SSID";  // SSID OF WIFI
const char* password = "PASSWORD";  // PASSWORD OF WIFI
int normalServoPins[] = { 32, 33, 23, 12 };
const int moveSpeed = 20;

bool servoType[4] = { true, false, true, false };

Servo normalServos[4];

int normalCurrentPos[4] = { 90, 90, 90, 90 };
int normalTargetPos[4] = { 90, 90, 90, 90 };

WebServer server(80);
unsigned long lastMove = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Sharley");
  Serial.println("======================================");
  Serial.println("Booting Sharley");
  Serial.println();
  setupServos();
  connectToWiFi();
  setupWebServer();
  Serial.println("MOTHERBOARD 2 SETUP");
  Serial.println("Waiting for Master...");
  Serial.println("Status page: http://" + WiFi.localIP().toString());
  Serial.println("MASTER CODE: " + WiFi.localIP().toString());
  Serial.println("Servo Configuration:");
  for (int i = 0; i < 4; i++) {
    if (servoType[i]) {
      Serial.println("  Servo " + String(i + 5) + ": 0°/90° range");
    } else {
      Serial.println("  Servo " + String(i + 5) + ": 90°/180° range");
    }
  }
  Serial.println("======================================");
}

void loop() {
  server.handleClient();
  moveServosSmooth();
  delay(10);
}

void setupServos() {
  Serial.println("Setting up servos...");
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  for (int i = 0; i < 4; i++) {
    normalServos[i].setPeriodHertz(50);
    normalServos[i].attach(normalServoPins[i], 544, 2400);
    normalServos[i].write(normalCurrentPos[i]);
    Serial.println("  Normal Servo " + String(i + 5) + " on pin " + String(normalServoPins[i]));
  }
}

void connectToWiFi() {
  Serial.print("Connecting: " + String(ssid));
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi Connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("WiFi connection failed!");
    Serial.println("Check your WiFi password");
  }
}

void setupWebServer() {
  server.on("/move_normal", handleMoveNormal);
  server.on("/status", handleStatusRequest);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started on port 80");
}

void moveServosSmooth() {
  if (millis() - lastMove >= moveSpeed) {
    lastMove = millis();
    bool anyMoving = false;
    for (int i = 0; i < 4; i++) {
      if (normalCurrentPos[i] != normalTargetPos[i]) {
        anyMoving = true;

        if (normalCurrentPos[i] < normalTargetPos[i]) {
          normalCurrentPos[i]++;
        } else {
          normalCurrentPos[i]--;
        }

        normalServos[i].write(normalCurrentPos[i]);
      }
    }
    if (anyMoving) {
      Serial.print("Moving: ");
      for (int i = 0; i < 4; i++) {
        Serial.print("S" + String(i + 5) + ":" + String(normalCurrentPos[i]) + "° ");
      }
      Serial.println();
    }
  }
}

void handleMoveNormal() {
  if (server.hasArg("action")) {
    String action = server.arg("action");
    if (action == "open" || action == "close") {
      Serial.println("MASTER COMMAND: Move normal servos (5-8) to " + action + " position");

      for (int i = 0; i < 4; i++) {
        if (servoType[i]) {
          // Type 1: 0°/90° range
          if (action == "open") {
            normalTargetPos[i] = 0;
          } else if (action == "close") {
            normalTargetPos[i] = 90;
          }
        } else {
          if (action == "open") {
            normalTargetPos[i] = 180;
          } else if (action == "close") {
            normalTargetPos[i] = 90;
          }
        }
      }
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Normal servos: Only 'open' or 'close' actions allowed");
    }
  } else {
    server.send(400, "text/plain", "Missing action parameter");
  }
}

void handleStatusRequest() {
  String status = "";
  for (int i = 0; i < 4; i++) {
    String range = servoType[i] ? "(0°/90°)" : "(90°/180°)";
    status += "S" + String(i + 5) + ":" + String(normalCurrentPos[i]) + "°" + range + " ";
  }
  server.send(200, "text/plain", status);
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Sharley MB 2</title>";
  html += "<style>";
  html += "body { font-family: Arial; margin: 40px; background: #f0f8ff; }";
  html += "h1 { color: #333; text-align: center; }";
  html += ".info { background: white; padding: 20px; border-radius: 10px; margin: 20px 0; }";
  html += ".servo { margin: 10px 0; padding: 10px; background: #e6f3ff; border-radius: 5px; }";
  html += ".special { background: #ffe6f3; }";
  html += ".type1 { background: #e6ffe6; }";
  html += ".type2 { background: #ffe6cc; }";
  html += "</style></head><body>";
  html += "<h1>Sharley</h1>";
  html += "<div class='info'>";
  html += "<h3>Network Info:</h3>";
  html += "<p><strong>IP Address:</strong> " + WiFi.localIP().toString() + "</p>";
  html += "<p><strong>Status:</strong> Ready for Master commands</p>";
  html += "<p><strong>Master Control:</strong> Use Master interface</p>";
  html += "</div>";
  html += "<div class='info'>";
  html += "<h3>Normal Servos (5-8):</h3>";
  for (int i = 0; i < 4; i++) {
    String servoClass = servoType[i] ? "servo type1" : "servo type2";
    String range = servoType[i] ? "(0°/90°)" : "(90°/180°)";
    html += "<div class='" + servoClass + "'>Servo " + String(i + 5) + ": " + String(normalCurrentPos[i]) + "° " + range + "</div>";
  }
  html += "</div>";
  html += "<div class='info'>";
  html += "<h3>Instructions:</h3>";
  html += "<p>This page shows current servo positions</p>";
  html += "<p>Control all servos from Master interface</p>";
  html += "<p>Page auto-refreshes every 5 seconds</p>";
  html += "<p><strong>Master Code: </strong> " + WiFi.localIP().toString() + "</p>";
  html += "<p><strong>Green:</strong> Type 1 servos (0°/90° range)</p>";
  html += "<p><strong>Orange:</strong> Type 2 servos (90°/180° range)</p>";
  html += "</div>";
  html += "<script>setTimeout(function(){ location.reload(); }, 5000);</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}
