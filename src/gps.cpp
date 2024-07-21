#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "gps.h"

WebServer server(80);
double latitude;
double longitude;

void handleLocation();

void setupGPS(String ssid, String password){
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

    latitude = doc["latitude"];
    longitude = doc["longitude"];

    Serial.print("Latitude: ");
    Serial.println(latitude);
    Serial.print("Longitude: ");
    Serial.println(longitude);

    server.send(200, "text/plain", "Location received");
}

