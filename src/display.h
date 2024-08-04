// display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include "globals.h"

void initDisplay(int i = 0); // Início da função e mostra bootscreen

//Funções para economizar linhas nas outras funções
void resetTftDisplay(int x = 0, int y = 0, uint16_t fc = FGCOLOR, int size = FM, uint16_t bg = BGCOLOR, uint16_t screen = BGCOLOR);
void setTftDisplay(int x = 0, int y = 0, uint16_t fc = tft.textcolor, int size = tft.textsize, uint16_t bg = tft.textbgcolor);

void displayRedStripe(String text, uint16_t fgcolor = TFT_WHITE, uint16_t bgcolor = TFT_RED);

void displayError(String txt);  // Faixa vermelha
void displayWarning(String txt);// Faixa amarela
void displayInfo(String txt);   // Faixa Azul
void displaySuccess(String txt);// Faixa Verde

void listFiles(int index, String fileList[][3]);
void loopOptions(const std::vector<std::pair<std::string, std::function<void()>>>& options, bool bright = false, bool submenu = false, String subText = "");

void drawOptions(int index,const std::vector<std::pair<std::string, std::function<void()>>>& options, uint16_t fgcolor, uint16_t bgcolor);

void drawSubmenu(int index,const std::vector<std::pair<std::string, std::function<void()>>>& options, String system);

void drawMainBorder();
void drawMainMenu(int index = 0);

int getBattery();

void drawBatteryStatus();

void drawWifiSmall(int x, int y);

void drawWifi(int x, int y);


void drawIntro();
void drawGPSInfo();

#endif
