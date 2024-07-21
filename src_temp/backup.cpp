#include <Arduino.h>
#include <M5Cardputer.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "SILVANA_2G";
const char* password = "pingopitoco";

WebServer server(80);

void handleLocation() {
    if (server.hasArg("plain") == false) {
        server.send(400, "text/plain", "Body not received");
        return;
    }

    String body = server.arg("plain");
    Serial.println("Received body: " + body);

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, body);

    if (error) {
        server.send(400, "text/plain", "Invalid JSON");
        return;
    }

    double latitude = doc["latitude"];
    double longitude = doc["longitude"];

    Serial.print("Latitude: ");
    Serial.println(latitude);
    Serial.print("Longitude: ");
    Serial.println(longitude);

    server.send(200, "text/plain", "Location received");
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());

    server.on("/location", HTTP_POST, handleLocation);
    server.begin();
}

void loop() {
    server.handleClient();
}