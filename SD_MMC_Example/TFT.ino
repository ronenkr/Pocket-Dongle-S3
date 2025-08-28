
#include "Arduino.h"
#include <SPI.h>
#include <SD_MMC.h>
#include <TFT_eSPI.h>
#include <FS.h>

// TFT Display Configuration (ST7735 80x160) - Uses default VSPI bus
TFT_eSPI tft = TFT_eSPI();

// SD card pinout for pocket-dongle-s3 (MMC mode)
#define SD_CLK_PIN 17
#define SD_CMD_PIN 18
#define SD_D0_PIN 16
#define SD_D1_PIN 15
#define SD_D2_PIN 48
#define SD_D3_PIN 47

// Colors
#define BLACK       0x0000
#define WHITE       0xFFFF
#define RED         0xF800
#define GREEN       0x07E0
#define BLUE        0x001F
#define YELLOW      0xFFE0
#define CYAN        0x07FF
#define MAGENTA     0xF81F


String name[150];
String tmp;
int numTabs=0;
int number=1;
File root;
uint32_t cardSize;


void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  root = fs.open("/","r");
  root.rewindDirectory();
      
  while (true)
  {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }
   
    tmp = entry.name();
    name[numTabs] = "/" + tmp;
    Serial.println(entry.name());
    entry.close();
    numTabs++;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== SD Card File Browser with Separate SPI Buses ===");
  
  // Initialize TFT on default VSPI bus
  Serial.println("Initializing TFT on VSPI bus...");
  tft.init();
  tft.setRotation(1); // Portrait mode for 80x160
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(1);
  
  // Show startup message
  tft.setCursor(5, 5);
  tft.println("SD File Browser");
  tft.setCursor(5, 20);
  tft.println("Dual SPI Bus");
  tft.setCursor(5, 35);
  tft.println("Initializing...");
  
  Serial.println("TFT initialized on VSPI");
  
  // Initialize SD card in MMC mode
  Serial.println("Initializing SD card in MMC mode...");
  Serial.printf("SD MMC pins: CLK=%d, CMD=%d, D0=%d, D1=%d, D2=%d, D3=%d\n", 
                SD_CLK_PIN, SD_CMD_PIN, SD_D0_PIN, SD_D1_PIN, SD_D2_PIN, SD_D3_PIN);
  
  // Set custom pins for SD/MMC peripheral
  if (!SD_MMC.setPins(SD_CLK_PIN, SD_CMD_PIN, SD_D0_PIN, SD_D1_PIN, SD_D2_PIN, SD_D3_PIN)) {
    Serial.println("Failed to set SD_MMC pins!");
    return;
  }
  Serial.println("SD_MMC pins configured successfully");
  
  // Initialize SD card in 4-bit MMC mode
  bool sdInitialized = false;
  if (SD_MMC.begin("/sdcard", false, false, SDMMC_FREQ_DEFAULT, 5)) {
    Serial.println("*** SD CARD INITIALIZED IN MMC MODE (4-bit)! ***");
    
    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    
    Serial.println("Listing files from SD card:");
    listDir(SD_MMC, "/", 0);
    
    sdInitialized = true;
  } else {
    Serial.println("SD card initialization failed in MMC mode!");
  }

  if (sdInitialized) {
    Serial.println("\n=== MMC MODE SETUP SUCCESSFUL ===");
    Serial.println("TFT: VSPI bus (default pins)");
    Serial.printf("SD:  MMC mode (CLK:%d CMD:%d D0:%d D1:%d D2:%d D3:%d)\n", 
                  SD_CLK_PIN, SD_CMD_PIN, SD_D0_PIN, SD_D1_PIN, SD_D2_PIN, SD_D3_PIN);
    
    tft.fillScreen(BLACK);
    tft.setCursor(5, 5);
    tft.setTextColor(GREEN, BLACK);
    tft.println("MMC Mode OK!");
    
    // Show SD card info
    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    tft.setCursor(5, 20);
    tft.setTextColor(WHITE, BLACK);
    tft.printf("SD: %lluMB", cardSize);
    
    // Show bus configuration
    tft.setCursor(5, 35);
    tft.setTextSize(1);
    tft.println("TFT: VSPI bus");
    tft.setCursor(5, 50);
    tft.println("SD:  MMC mode");
    
    tft.setCursor(5, 65);
    tft.printf("CLK:%d CMD:%d", SD_CLK_PIN, SD_CMD_PIN);
    tft.setCursor(5, 80);
    tft.printf("D0:%d  D1:%d", SD_D0_PIN, SD_D1_PIN);
    
    delay(3000);
    
    // Start file listing
    listFiles();
  } else {
    Serial.println("\n=== SD CARD INITIALIZATION FAILED ===");
    
    tft.fillScreen(BLACK);
    tft.setCursor(5, 5);
    tft.setTextColor(RED, BLACK);
    tft.println("SD Card");
    tft.setCursor(5, 20);
    tft.println("FAILED");
    tft.setCursor(5, 40);
    tft.setTextColor(WHITE, BLACK);
    tft.println("Check:");
    tft.setCursor(5, 55);
    tft.println("- Card inserted");
    tft.setCursor(5, 70);
    tft.println("- MMC wiring");
  }
}

void listFiles() {
  // Using MMC mode - no manual CS control needed
  
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(YELLOW, BLACK);
  //tft.println("Files on SD:");
  //tft.println("(MMC Mode)");
  //tft.println("----------");
  
  Serial.println("\n=== Files on SD Card (MMC Mode) ===");
  
  // Open root directory fresh each time
  File rootDir = SD_MMC.open("/");
  if (!rootDir) {
    tft.setTextColor(RED, BLACK);
    tft.println("Failed to open");
    tft.println("root directory");
    Serial.println("Failed to open root directory");
    return;
  }
  
  int fileCount = 0;
  int yPos = 0; // Start lower to account for extra header line
  
  File file = rootDir.openNextFile();
  while (file && fileCount < 10) { // Reduced to fit screen with extra header
    String fileName = file.name();
    uint32_t fileSize = file.size();
    
    // Skip hidden files
    if (fileName.startsWith(".")) {
      file.close();
      file = rootDir.openNextFile();
      continue;
    }
    
    fileCount++;
    
    // Display on LCD
    tft.setCursor(2, yPos);
    
    if (file.isDirectory()) {
      tft.setTextColor(CYAN, BLACK);
      tft.printf("[%s]", fileName.c_str());
      Serial.printf("DIR:  %s\n", fileName.c_str());
    } else {
      tft.setTextColor(WHITE, BLACK);
      // Truncate long filenames for display
      if (fileName.length() > 12) {
        fileName = fileName.substring(0, 9) + "...";
      }
      
      if (fileSize < 1024) {
        tft.printf("%s %uB", fileName.c_str(), fileSize);
        Serial.printf("FILE: %s (%u bytes)\n", file.name(), fileSize);
      } else if (fileSize < 1024*1024) {
        tft.printf("%s %uK", fileName.c_str(), fileSize/1024);
        Serial.printf("FILE: %s (%u KB)\n", file.name(), fileSize/1024);
      } else {
        tft.printf("%s %uM", fileName.c_str(), fileSize/(1024*1024));
        Serial.printf("FILE: %s (%u MB)\n", file.name(), fileSize/(1024*1024));
      }
    }
    
    yPos += 11; // Move to next line
    if (yPos > 150) break; // Don't go off screen
    
    file.close();
    file = rootDir.openNextFile();
  }
  
  rootDir.close();
  
  if (fileCount == 0) {
    tft.setTextColor(YELLOW, BLACK);
    tft.setCursor(2, 40);
    tft.println("No files found");
    Serial.println("No files found");
  } else {
    // Show file count at bottom
    tft.setCursor(2, 150);
    tft.setTextColor(GREEN, BLACK);
    tft.printf("Total: %d files", fileCount);
    Serial.printf("\nTotal files shown: %d\n", fileCount);
  }
}

void loop() {
  // Using MMC mode - no manual bus control needed
  
  // Refresh file list every 10 seconds
  delay(10000);
  Serial.println("Refreshing file list in MMC mode...");
  listFiles();
}