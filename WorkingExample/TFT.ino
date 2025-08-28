
#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
#include <TFT_eSPI.h>
#include <FS.h>

// TFT Display Configuration (ST7735 80x160) - Uses default VSPI bus
TFT_eSPI tft = TFT_eSPI();

// SD Card Configuration - Uses separate HSPI bus to avoid conflicts
#define SD_MISO 16
#define SD_MOSI 18  
#define SD_SCK 17
#define SD_CS 47

// Create separate SPI bus for SD card (HSPI)
SPIClass sdSPI(HSPI);

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
  
  // Initialize SD card on separate HSPI bus
  Serial.println("Initializing SD card on HSPI bus...");
  Serial.printf("SD pins: MISO=%d, MOSI=%d, SCK=%d, CS=%d\n", 
                SD_MISO, SD_MOSI, SD_SCK, SD_CS);
  
  // Configure SD CS pin
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);  // Start with CS high (unselected)
  
  // Initialize HSPI bus for SD card
  sdSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  
  // Try to initialize SD card on HSPI bus
  bool sdInitialized = false;
  if (SD.begin(SD_CS, sdSPI, 4000000)) {
    Serial.println("*** SD CARD INITIALIZED ON HSPI BUS! ***");
    
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    
    Serial.println("Listing files from SD card:");
    listDir(SD, "/", 0);
    
    sdInitialized = true;
  } else {
    Serial.println("SD card initialization failed on HSPI bus!");
    Serial.println("Check connections and card insertion");
  }

  if (sdInitialized) {
    Serial.println("\n=== DUAL SPI BUS SETUP SUCCESSFUL ===");
    Serial.println("TFT: VSPI bus (default pins)");
    Serial.printf("SD:  HSPI bus (MISO:%d MOSI:%d SCK:%d CS:%d)\n", 
                  SD_MISO, SD_MOSI, SD_SCK, SD_CS);
    
    tft.fillScreen(BLACK);
    tft.setCursor(5, 5);
    tft.setTextColor(GREEN, BLACK);
    tft.println("Dual SPI OK!");
    
    // Show SD card info
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    tft.setCursor(5, 20);
    tft.setTextColor(WHITE, BLACK);
    tft.printf("SD: %lluMB", cardSize);
    
    // Show bus configuration
    tft.setCursor(5, 35);
    tft.setTextSize(1);
    tft.println("TFT: VSPI bus");
    tft.setCursor(5, 50);
    tft.println("SD:  HSPI bus");
    
    tft.setCursor(5, 65);
    tft.printf("MISO:%d MOSI:%d", SD_MISO, SD_MOSI);
    tft.setCursor(5, 80);
    tft.printf("SCK:%d  CS:%d", SD_SCK, SD_CS);
    
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
    tft.println("- HSPI wiring");
  }
}

void listFiles() {
  // No need to manually control CS - the separate SPI bus handles this
  
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(YELLOW, BLACK);
  //tft.println("Files on SD:");
  //tft.println("(HSPI Bus)");
  //tft.println("----------");
  
  Serial.println("\n=== Files on SD Card (HSPI Bus) ===");
  
  // Open root directory fresh each time
  File rootDir = SD.open("/");
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
  // No need to manually control CS - separate SPI buses prevent conflicts
  
  // Refresh file list every 10 seconds
  delay(10000);
  Serial.println("Refreshing file list on HSPI bus...");
  listFiles();
}