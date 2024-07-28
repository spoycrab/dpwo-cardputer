/*
https://docs.m5stack.com/en/arduino/m5gfx/m5gfx_functions
*/
#include <Arduino.h>
#include "globals.h"

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
TFT_eSprite sprite = TFT_eSprite(&tft);
TFT_eSprite draw = TFT_eSprite(&tft);

Keyboard_Class Keyboard = Keyboard_Class();

std::vector<std::pair<std::string, std::function<void()>>> options;

// Public Globals Variables
int prog_handler;
int rotation;
bool sdcardMounted;
bool wifiConnected;
bool BLEConnected;
bool returnToMenu;
char timeStr[10];
time_t localTime;
struct tm* timeInfo;
ESP32Time rtc;
bool clock_set = false;
String ssid;
String pwd;


#include "mykeyboard.h"
#include "wifi_common.h"
#include "display.h"
#include "settings.h"
#include "Wire.h"
#include "gps.h"

void setup()
{
    Serial.begin(115200);
    // declare variables
    prog_handler = 0;
    sdcardMounted = false;
    wifiConnected = false;
    BLEConnected = false;

    latitude = 0.0;
    longitude = 0.0;

    Keyboard.begin();
    tft.init();

    rotation = gsetRotation();
    tft.setRotation(rotation);
    resetTftDisplay();

    getBrightness();
    setBrightness(10);
}

void loop()
{
    int index = 0;
    bool redraw = true;
    int opt = 6;
    while (1)
    {
        // if(wifiConnected){
        //     server.handleClient();
        // }
        // switch (index){
        //     case 0:                  
        //         if(redraw){drawIntro();redraw = false;}
        //         Keyboard.update();
        //         if (Keyboard.isChange())
        //         {
        //             if (Keyboard.isKeyPressed(KEY_ENTER))
        //             {
        //                 wifiConnectMenu();
        //                 setupGPS(ssid, pwd);
        //                 wifiConnected = true;
        //                 delay(200);
        //                 index++; // pular para proxima tela
        //                 redraw = true;
        //                 Serial.println(ssid);
        //                 Serial.println(pwd);
        //             }
        //         }
        //         break;
        //     case 1:
        //         if(redraw){drawGPSInfo();redraw = false;}
        //         Keyboard.update();
        //         if (Keyboard.isChange())
        //         {
        //             if (Keyboard.isKeyPressed(KEY_ENTER))
        //             {
        //                 tft.fillScreen(BGCOLOR);
        //                 redraw = true;
        //                 index++;
        //             }
        //         }
        //         break;
        //     case 2: //teste
        //         if(redraw){drawDPWOinfo();redraw = false;}
        //         Keyboard.update();
        //         if (Keyboard.isChange())
        //         {
        //             if (Keyboard.isKeyPressed(KEY_ENTER))
        //             {
        //                 tft.fillScreen(BGCOLOR);
        //                 redraw = true;
        //                 index--;
        //             }
        //         }
        //         break;
        // }


        // switch (index){
        //     case 0:
        //         if(redraw)drawIntro();
        //         redraw = false;
        //         break;
        //     default:
        //         if(redraw)drawMainBorder();
        //         drawGPSInfo();
        //         redraw = false;
        //         break;
        // }
        // delay(200);
        
        // Keyboard.update();
        // if(Keyboard.isChange()){
        //     if(Keyboard.isKeyPressed(KEY_ENTER)){
        //         wifiConnectMenu();
        //         setupGPS(ssid,pwd);
        //         wifiConnected = true;
        //         delay(200);
        //         index++;//pular para proxima tela
        //         redraw = true;
        //         Serial.println(ssid);
        //         Serial.println(pwd);
        //     }
        // }

        Keyboard.update();
        if(Keyboard.isChange()){
            if(Keyboard.isKeyPressed(KEY_ENTER)){
                Serial.println("dpwo started");
                dpwo_setup();
            }
        }
    }
}
