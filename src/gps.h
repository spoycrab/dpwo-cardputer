#ifndef GPS_H
#define GPS_H

#include <WebServer.h>

extern double latitude;
extern double longitude;

extern WebServer server;

void setupGPS(String ssid, String password);
void handleLocation();

#endif