#include "display.h"
#include "mykeyboard.h"
#include "settings.h"
#include "gps.h"
#include <WiFi.h>


#if defined(CARDPUTER)  //Battery Calculation
  #include <driver/adc.h>
  #include <esp_adc_cal.h>
  #include <soc/soc_caps.h>
  #include <soc/adc_channel.h>
#endif

/***************************************************************************************
** Function name: resetTftDisplay
** Description:   set cursor to 0,0, screen and text to default color
***************************************************************************************/
void resetTftDisplay(int x, int y, uint16_t fc, int size, uint16_t bg, uint16_t screen) {
    tft.setCursor(x,y);
    tft.fillScreen(screen);
    tft.setTextSize(size);
    tft.setTextColor(fc,bg);
}

/***************************************************************************************
** Function name: setTftDisplay
** Description:   set cursor, font color, size and bg font color
***************************************************************************************/
void setTftDisplay(int x, int y, uint16_t fc, int size, uint16_t bg) {
    if (x>=0 && y<0)        tft.setCursor(x,tft.getCursorY());          // if -1 on x, sets only y
    else if (x<0 && y>=0)   tft.setCursor(tft.getCursorX(),y);          // if -1 on y, sets only x
    else if (x>=0 && y>=0)  tft.setCursor(x,y);                         // if x and y > 0, sets both
    tft.setTextSize(size);
    tft.setTextColor(fc,bg);
}

/***************************************************************************************
** Function name: displayRedStripe
** Description:   Display Red Stripe with information
***************************************************************************************/
void displayRedStripe(String text, uint16_t fgcolor, uint16_t bgcolor) {
    int size;
    if(text.length()*LW*FM<(tft.width()-2*FM*LW)) size = FM;
    else size = FP;
    tft.fillSmoothRoundRect(10,HEIGHT/2-13,WIDTH-20,26,7,bgcolor);
    tft.setTextColor(fgcolor,bgcolor);
    if(size==FM) { 
      tft.setTextSize(FM); 
      tft.setCursor(WIDTH/2 - FM*3*text.length(), HEIGHT/2-8);
    }
    else {
      tft.setTextSize(FP);
      tft.setCursor(WIDTH/2 - FP*3*text.length(), HEIGHT/2-8);
    } 
    tft.println(text);
}

void displayError(String txt)   { displayRedStripe(txt); }
void displayWarning(String txt) { displayRedStripe(txt, TFT_BLACK,TFT_YELLOW); }
void displayInfo(String txt)    { displayRedStripe(txt, TFT_WHITE, TFT_BLUE); }
void displaySuccess(String txt) { displayRedStripe(txt, TFT_WHITE, TFT_DARKGREEN); }

/*********************************************************************
**  Function: loopOptions                             
**  Where you choose among the options in menu
**********************************************************************/
void loopOptions(const std::vector<std::pair<std::string, std::function<void()>>>& options, bool bright, bool submenu, String subText){
  bool redraw = true;
  int index = 0;
  while(1){
    if (redraw) { 
      if(submenu) drawSubmenu(index, options, subText);
      else drawOptions(index, options, FGCOLOR, BGCOLOR);
      if(bright){
        #if !defined(STICK_C_PLUS)
        int bl = MINBRIGHT + round(((255 - MINBRIGHT) * (4 - index) * 0.25)); // 4 is the number of options
        analogWrite(BACKLIGHT, bl);
        #else
        axp192.ScreenBreath(100*(4 - index) * 0.25);  // 4 is the number of options
        #endif
      }
      redraw=false;
      delay(200); 
    }

    if(checkPrevPress()) {
    #ifdef CARDPUTER  
      if(index==0) index = options.size() - 1;
      else if(index>0) index--;
      redraw = true;
    #else
      break;
    #endif
    }
    /* DW Btn to next item */
    if(checkNextPress()) { 
      index++;
      if((index+1)>options.size()) index = 0;
      redraw = true;
    }

    /* Select and run function */
    if(checkSelPress()) { 
      options[index].second();
      break;
    }

    #ifdef CARDPUTER
    if(checkEscPress()) break;
    #endif
  }
  delay(200);
}

/***************************************************************************************
** Function name: progressHandler
** Description:   Função para manipular o progresso da atualização
** Dependencia: prog_handler =>>    0 - Flash, 1 - LittleFS
***************************************************************************************/
void progressHandler(int progress, size_t total) {
#ifndef STICK_C
  int barWidth = map(progress, 0, total, 0, 200);
  if(barWidth <3) {
    tft.fillRect(6, 27, WIDTH-12, HEIGHT-33, BGCOLOR);
    tft.drawRect(18, HEIGHT - 47, 204, 17, FGCOLOR);
    displayRedStripe("Running, Wait", TFT_WHITE, FGCOLOR);
  }
  tft.fillRect(20, HEIGHT - 45, barWidth, 13, FGCOLOR);
#else
  
  int barWidth = map(progress, 0, total, 0, 100);
  if(barWidth <2) {
    tft.fillRect(6, 6, WIDTH-12, HEIGHT-12, BGCOLOR);
    tft.drawRect(28, HEIGHT - 47, 104, 17, FGCOLOR);
    displayRedStripe("Wait",TFT_WHITE,FGCOLOR);
  }
  tft.fillRect(30, HEIGHT - 45, barWidth, 13, FGCOLOR);

#endif

}

/***************************************************************************************
** Function name: drawOptions
** Description:   Função para desenhar e mostrar as opçoes de contexto
***************************************************************************************/
void drawOptions(int index,const std::vector<std::pair<std::string, std::function<void()>>>& options, uint16_t fgcolor, uint16_t bgcolor) {
    int menuSize = options.size();
    if(options.size()>MAX_MENU_SIZE) menuSize = MAX_MENU_SIZE;

    tft.fillRoundRect(WIDTH*0.15,HEIGHT/2-menuSize*(FM*8+4)/2 -5,WIDTH*0.7,(FM*8+4)*menuSize+10,5,bgcolor);
    
    tft.setTextColor(fgcolor,bgcolor);
    tft.setTextSize(FM);
    tft.setCursor(WIDTH*0.15+5,HEIGHT/2-menuSize*(FM*8+4)/2);

    int i=0;
    int init = 0;
    int cont = 1;
    menuSize = options.size();
    if(index>=MAX_MENU_SIZE) init=index-MAX_MENU_SIZE+1;
    for(i=0;i<menuSize;i++) {
      if(i>=init) {
        String text="";
        if(i==index) text+=">";
        else text +=" ";
        text += String(options[i].first.c_str());
        tft.setCursor(WIDTH*0.15+5,tft.getCursorY()+4);
        tft.println(text.substring(0,13));
        cont++;
      }
      if(cont>MAX_MENU_SIZE) goto Exit;
    }
    Exit:
    if(options.size()>MAX_MENU_SIZE) menuSize = MAX_MENU_SIZE;
    tft.drawRoundRect(WIDTH*0.15,HEIGHT/2-menuSize*(FM*8+4)/2 -5,WIDTH*0.7,(FM*8+4)*menuSize+10,5,fgcolor);
}

/***************************************************************************************
** Function name: drawOptions
** Description:   Função para desenhar e mostrar as opçoes de contexto
***************************************************************************************/
void drawSubmenu(int index,const std::vector<std::pair<std::string, std::function<void()>>>& options, String system) {
    int menuSize = options.size();
    drawMainBorder();
    tft.setTextColor(FGCOLOR,BGCOLOR);
    tft.fillRect(6,26,WIDTH-12,20,BGCOLOR);
    tft.fillRoundRect(6,26,WIDTH-12,HEIGHT-32,5,BGCOLOR);
    tft.setTextSize(FP);
    tft.setCursor(12,30);
    tft.setTextColor(FGCOLOR);
    tft.println(system);

    if (index-1>=0) {
      tft.setTextSize(FM);
      tft.setTextColor(FGCOLOR-0x2000);
      tft.drawCentreString(options[index-1].first.c_str(),WIDTH/2, 42,SMOOTH_FONT);
    } else {
      tft.setTextSize(FM);
      tft.setTextColor(FGCOLOR-0x2000);
      tft.drawCentreString(options[menuSize-1].first.c_str(),WIDTH/2, 42,SMOOTH_FONT);
    }
      tft.setTextSize(FG);
      tft.setTextColor(FGCOLOR);
      tft.drawCentreString(options[index].first.c_str(),WIDTH/2, 67,SMOOTH_FONT);

    if (index+1<menuSize) {
      tft.setTextSize(FM);
      tft.setTextColor(FGCOLOR-0x2000);
      tft.drawCentreString(options[index+1].first.c_str(),WIDTH/2, 102,SMOOTH_FONT);
    } else {
      tft.setTextSize(FM);
      tft.setTextColor(FGCOLOR-0x2000);
      tft.drawCentreString(options[0].first.c_str(),WIDTH/2, 102,SMOOTH_FONT);
    }

    tft.drawFastHLine(WIDTH/2 - options[index].first.size()*FG*LW/2, 67+FG*LH,options[index].first.size()*FG*LW,FGCOLOR);
    tft.fillRect(tft.width()-5,index*tft.height()/menuSize,5,tft.height()/menuSize,FGCOLOR);

}

void drawMainBorder() {
    tft.fillScreen(BGCOLOR);
    setTftDisplay(12, 12, FGCOLOR, 1, BGCOLOR);

    //if(wifiConnected) {tft.print(WiFi.localIP());} else {tft.print("Nao Conectado");}

    int i=0;
    if(wifiConnected) { drawWifiSmall(WIDTH - 90, 7); i++;}               //Draw Wifi Symbol beside battery
    
    tft.drawRoundRect(5, 5, WIDTH - 10, HEIGHT - 10, 5, FGCOLOR);
    tft.drawLine(5, 25, WIDTH - 6, 25, FGCOLOR);
    drawBatteryStatus();
}

void drawIntro(){
  drawMainBorder();
  drawWifi(80,27);
  tft.setTextSize(FM);
  tft.drawCentreString("Escanear Redes",tft.width()/2, tft.height()-(LH*FM+10), SMOOTH_FONT);
}

void drawGPSInfo()
{
  tft.setTextSize(FM);
  tft.setCursor(30,42);tft.print("SSID: ");if(wifiConnected) {tft.print(WiFi.SSID());}else {tft.print("Nao Conectado");}
  tft.setCursor(30,42+(1*17));tft.print("Logintude: "+ String(longitude, 7));
  tft.setCursor(30,42+(2*17));tft.print("Latitude: "+ String(latitude, 7));
}



/***************************************************************************************
** Function name: getBattery()
** Description:   Delivers the battery value from 1-100
***************************************************************************************/
int getBattery() {
  int percent=0;
  #if defined(STICK_C_PLUS)
  float b = axp192.GetBatVoltage();
  percent = ((b - 3.0) / 1.2) * 100;
  
  #else
  
    #if defined(CARDPUTER)
      uint8_t _batAdcCh = ADC1_GPIO10_CHANNEL;
      uint8_t _batAdcUnit = 1;
    #elif defined(STICK_C_PLUS2)
      uint8_t _batAdcCh = ADC1_GPIO38_CHANNEL;
      uint8_t _batAdcUnit = 1;
    #endif
  
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten((adc1_channel_t)_batAdcCh, ADC_ATTEN_DB_11);
    static esp_adc_cal_characteristics_t* adc_chars = nullptr;
    static constexpr int BASE_VOLATAGE = 3600;
    adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize((adc_unit_t)_batAdcUnit, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, BASE_VOLATAGE, adc_chars);
    int raw;
    raw = adc1_get_raw((adc1_channel_t)_batAdcCh);
    uint32_t volt = esp_adc_cal_raw_to_voltage(raw, adc_chars);

    float mv = volt * 2;
    percent = (mv - 3300) * 100 / (float)(4150 - 3350);

  #endif
  return  (percent < 0) ? 0
        : (percent >= 100) ? 100
        :  percent;
}

/***************************************************************************************
** Function name: drawBatteryStatus()
** Description:   Delivers the battery value from 1-100
***************************************************************************************/
void drawBatteryStatus() {
    tft.drawRoundRect(WIDTH - 42, 7, 34, 17, 2, FGCOLOR);
    int bat = getBattery();
    tft.setTextSize(FP);
    tft.setTextColor(FGCOLOR, BGCOLOR);
    tft.drawRightString(String(bat) + "%", WIDTH - 45, 12, 1);
    tft.fillRoundRect(WIDTH - 40, 9, 30 * bat / 100, 13, 2, FGCOLOR);
    tft.drawLine(WIDTH - 30, 9, WIDTH - 30, 9 + 13, BGCOLOR);
    tft.drawLine(WIDTH - 20, 9, WIDTH - 20, 9 + 13, BGCOLOR);
}


/***************************************************************************************
** Function name: listFiles
** Description:   Função para desenhar e mostrar o menu principal
***************************************************************************************/
#define MAX_ITEMS 7
void listFiles(int index, String fileList[][3]) {
    tft.fillScreen(BGCOLOR);
    tft.drawRoundRect(5, 5, WIDTH - 10, HEIGHT - 10, 5, FGCOLOR);
    tft.setCursor(10,10);
    tft.setTextSize(FM);
    int arraySize = 0;
    while(fileList[arraySize][2]!="" && arraySize < MAXFILES) arraySize++;
    int i=0;
    int start=0;
    if(index>=MAX_ITEMS) {
        start=index-MAX_ITEMS+1;
        if(start<0) start=0;
    }
    
    while(i<arraySize) {
        if(i>=start && fileList[i][2]!="") {
            tft.setCursor(10,tft.getCursorY());
            if(fileList[i][2]=="folder") tft.setTextColor(FGCOLOR-0x1111);
            else if(fileList[i][2]=="operator") tft.setTextColor(ALCOLOR);
            else tft.setTextColor(FGCOLOR);

            if (index==i) tft.print(">");
            else tft.print(" ");
            tft.println(fileList[i][0].substring(0,17));
            
        }
        i++;
        if (i==(start+MAX_ITEMS) || fileList[i][2]=="") break;
    }
}

void drawWifiSmall(int x, int y) {
  draw.deleteSprite();
  draw.createSprite(17,17);
  draw.fillSprite(BGCOLOR);
  draw.fillCircle(9,14,2,FGCOLOR);
  draw.drawSmoothArc(9,14,5,7,130,230,FGCOLOR, BGCOLOR,true);
  draw.drawSmoothArc(9,14,11,13,130,230,FGCOLOR, BGCOLOR,true);
  draw.pushSprite(x,y);
  draw.deleteSprite();
}

void drawWifi(int x, int y) {
  tft.fillRect(x,y,80,80,BGCOLOR);
  tft.fillCircle(40+x,60+y,6,FGCOLOR);
  tft.drawSmoothArc(40+x,60+y,26,20,130,230,FGCOLOR, BGCOLOR,true);
  tft.drawSmoothArc(40+x,60+y,46,40,130,230,FGCOLOR, BGCOLOR,true);
}
