#include "displayM5.h"
M5Canvas canvas(&M5Cardputer.Display);
String data = "> ";
void drawTextBox(){

    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextSize(0.5);
    M5Cardputer.Display.drawRect(0, 0, M5Cardputer.Display.width(),M5Cardputer.Display.height() - 28, RED);
    M5Cardputer.Display.setTextFont(&fonts::FreeSerif18pt7b);

    M5Cardputer.Display.fillRect(0, M5Cardputer.Display.height() - 4, M5Cardputer.Display.width(), 4, GREEN);

    canvas.setTextFont(&fonts::FreeSerif18pt7b);
    canvas.setTextSize(0.5);
    canvas.createSprite(M5Cardputer.Display.width() - 8,
                        M5Cardputer.Display.height() - 36);
    canvas.setTextScroll(true);
    canvas.println("Press Key and Enter to Input Text");
    canvas.pushSprite(4, 4);
    canvas.println("");


    M5Cardputer.Display.drawString("> ", 4, M5Cardputer.Display.height() - 24);
}

void writeText(){
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

            for (auto i : status.word) {
                data += i;
            }

            if (status.del) {
                data.remove(data.length() - 1);
            }

            if (status.ctrl){
                Serial.println(data);
            }

            if (status.enter) {
                data.remove(0, 2);
                canvas.println(data);
                canvas.pushSprite(4, 4);
                data = "> ";
            }
        
            M5Cardputer.Display.fillRect(0, M5Cardputer.Display.height() - 28,
                                         M5Cardputer.Display.width(), 25,
                                         BLACK);

            M5Cardputer.Display.drawString(data, 4,
                                           M5Cardputer.Display.height() - 24);
        }
    }
}