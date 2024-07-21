/*
https://docs.m5stack.com/en/arduino/m5gfx/m5gfx_functions
*/
#include <Arduino.h>
#include "globals.h"
#include "displayM5.h"
// #include <M5Cardputer.h>
// #include "M5GFX.h"

// M5Canvas canvas(&M5Cardputer.Display);

#define BITMAP M5Cardputer.Display.drawBmp(NEMOMatrix, 97338)
void setup(){
    Serial.begin(115200);
    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextSize(0.5);
    drawTextBox();
    
}

void loop(){
    M5Cardputer.update();
    writeText();
}
