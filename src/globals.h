// Globals.h

#define FGCOLOR TFT_DARKGREEN
#define ALCOLOR TFT_RED
#define BGCOLOR TFT_BLACK

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <functional>
#include <vector>
#include <SPIFFS.h>

#include <ESP32Time.h>
#include <EEPROM.h>
#include <dpwo.h>

#if defined(CARDPUTER)
  #include <Keyboard.h>
  extern Keyboard_Class Keyboard;
#endif

// Declaração dos objetos TFT
extern TFT_eSPI tft;
extern TFT_eSprite sprite;
extern TFT_eSprite draw;

extern int prog_handler;    // 0 - Flash, 1 - LittleFS, 2 - Download

extern bool sdcardMounted;  // informa se o cartão está montado ou não, sem precisar chamar a função setupSdCard

extern bool wifiConnected;  // informa se o wifi está ativo ou não

extern bool BLEConnected;  // informa se o BLE está ativo ou não

extern std::vector<std::pair<std::string, std::function<void()>>> options;

extern  String ssid;

extern  String pwd;

extern String fileToCopy;

extern int rotation;

extern uint8_t buff[4096];

extern const int bufSize;

extern bool returnToMenu; // variável para verificação e quebrar os loops

void backToMenu();
