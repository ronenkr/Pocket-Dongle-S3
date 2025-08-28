#include "TFT_eSPI.h"
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <vector>
#include "JpegFunc.h"
#include "FS.h"
#include "SD_MMC.h"
#include <JPEGDecoder.h>

//Micro SD Card PIN
#define SD_MISO_PIN                     16//5
#define SD_MOSI_PIN                     18//6
#define SD_SCLK_PIN                     17//7
#define SD_CS_PIN                       47//42

#define SCALE_SIZE 4
bool flag = 1;

char str1[20];
uint32_t cardSize;
TFT_eSPI tft= TFT_eSPI(80,160);
TFT_eSprite sprite = TFT_eSprite(&tft);
static int16_t x, y;


String name[150];
String tmp;
int numTabs=0;
int number=1;
File root;



static int jpegDrawCallback(JPEGDRAW *pDraw)
{
    tft.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight,pDraw->pPixels);
    return 1;
}



void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
 root = fs.open("/","r");
 root.rewindDirectory();
      
  while (true)
  {
    File entry =  root.openNextFile();
    if (! entry)
    {break;}
   
    tmp=entry.name();
    name[numTabs]="/"+tmp;
    Serial.println(entry.name());
    entry.close();
    numTabs++;
  }

}




void setup() {

  Serial.begin(115200);
  tft.begin();
  
  tft.setRotation(3);
  // tft.invertDisplay(1);
  //===========================================================
  SD_MMC.setPins(SD_SCLK_PIN, SD_MOSI_PIN,SD_MISO_PIN);
  if (!SD_MMC.begin("/sdcard", true, true,4000000)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD_MMC card attached");
    return;
  }
  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
  //===========================================================
  listDir(SD_MMC, "/", 0); // Change the root directory as needed

}

bool AT = 1;
void loop() {

  if(AT == 1){
    tft.fillScreen(TFT_RED);
    delay(1000);
    // Some example procedures showing how to display to the pixels:
    tft.fillScreen(TFT_GREEN);
    delay(1000);
    tft.fillScreen(TFT_BLUE);
    delay(1000);
    AT=0;
  }
  for(number=0;number<numTabs;number++){
      jpegDraw(name[number].c_str(), jpegDrawCallback, true, 0, 0, 80, 160);
      delay(1000);
    }
}
