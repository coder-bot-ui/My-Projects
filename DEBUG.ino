#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS 22
#define TFT_RST 21
#define TFT_DC 20
#define TFT_MOSI 0
#define TFT_SCLK 1

struct config {
  int DEBUG;
  int UEFI;
  int id;
  int SHELL;
  int d1;
  int d2;
  int d3;
  int d4;
};

struct mem {
  char single_byte;
  int integer;
  int id;
};


int metadata = 91964;
int metadata1 = 0;
int metadata2 = 70573;
int metadata3 = 84057;
int BIOS = 1;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


config shell_config = { 1, 1, 5, 1, 1348, 1121, metadata, 79 };
mem _config = { 'A', 1345, 3 };
config sconfig = { 1, 1, 1, 1, 1, 1, 1, 1 };
String bk = "1";



void bootloop() {
  tft.fillScreen(ST7735_BLACK);
  delay(10000000);
}

void fw() {
  tft.fillScreen(ST7735_BLUE);
  tft.setCursor(0, 10);
  tft.setTextColor(ST7735_WHITE);
  tft.print("YOS Pico-BIOS\n");
  tft.print("\nPlease Wait..\n");
  delay(7000);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 10);
  tft.setTextColor(ST7735_GREEN);
  delay(5000);
  tft.print(" BIOS IS UPDATING \n");
  tft.print("[=========       ]\n");
  delay(3000);
  tft.print("BIOS UPDATE FAILED\n");
  BIOS = 0;
  delay(1000);
  tft.print("RESTARTING..\n");
  delay(2000);
  shell();
}


void debug() {
  tft.print("Starting DEBUG mode..\n");
  tft.print("[=                    ]");
  tft.println("");
  tft.println("[==                   ]");
  delay(2000);
  tft.println("[====                 ]");
  delay(1000);
  tft.println("[=======              ]");
  delay(3000);
  tft.println("[===========          ]");
  delay(2000);
  tft.println("[==================== ]");
  delay(4000);
  tft.println("[=====================]");

  tft.setCursor(0, 65);
}

void shell() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_GREEN);
  tft.print("Starting YOS shell...\n");
  delay(2000);
  tft.print("Loading Config..\n");


  const char* boot_msg = "";
  int debug_init = 0;
  if (BIOS) {
    metadata = 2377;

  } else {
    tft.print("BIOS SECURITY FAILED");
    delay(6567);
    bootloop();
  }
  if (shell_config.UEFI) {
    boot_msg = "YOS UEFI BIOS\n";
    metadata1 = 1000;
    metadata2 = 0;
  } else {
    boot_msg = "YOS LEGACY BIOS\n";
    metadata1 = 20;
    metadata2 = 1040;
  }

  if (shell_config.DEBUG) {
    debug_init = 1;
    metadata1 = 1120;
    metadata2 = 41;
  } else {
    debug_init = 0;
    metadata1 = 0;
    metadata2 = 0;
  }

  if (debug_init) {
    debug();
  } else {
    metadata1 = 1020;
  }


  tft.print("\n");
  tft.setCursor(0, 95);
  tft.print(boot_msg);
  tft.setCursor(0, 105);
  tft.print("Starting debug shell");
  delay(6000);
}

void setup() {
  Serial.begin(115200);
  if (bk != "1") {
    tft.fillScreen(ST7735_BLUE);
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(0, 0);
    tft.print("ERR:NO_CONFIG");
    delay(10000000);
  }
  shell();

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 10);
}

void loop() {
  if (Serial.available() > 0) {
    String userInput = Serial.readStringUntil(';');
    userInput.trim();

    tft.print(">>> ");
    tft.println(userInput);
  if (userInput == "setup") {
    tft.print("\nStarting setup..");
    delay(4000);
    tft.fillScreen(ST7735_BLUE);
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(0, 10);
    tft.print("YOS SETUP");
    tft.print("\nSetup is starting..");
    delay(2000);
    tft.print("\nSetup is Unpacking Files\n");
    delay(1500);
    tft.print("Formating disk\n");
    delay(4000);
    tft.print("> setup.exe -e\n");
    delay(3000);
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_GREEN);
    tft.setCursor(0, 10);



  }else {
      metadata1 = 0130;
  }
  if (userInput == "shell32") {

      tft.fillScreen(ST7735_BLACK);
      tft.setCursor(0, 10);
      tft.print("Starting Shell..\n");
      tft.print("metadata: ");
      tft.println(metadata1);
      tft.print("\n");
      tft.print("key: 7654\n");
      tft.print("example:\n");
      tft.print("7654 metadata");
      delay(8000);
      tft.fillScreen(ST7735_BLACK);
      tft.setCursor(0, 10);



    } else {
      int metadata = 2097;
    }

    if (userInput == "fw") {
      fw();
    } else {
      metadata1 = 000;
    }
    if (userInput == "long mode") {
      tft.print("Going to 64-bit Long mode\n");
      tft.print("(x64)[===========     ]\n");
      delay(4000);
      tft.fillScreen(ST7735_BLACK);
      tft.setCursor(0, 10);
      tft.print("Installing CPU microcode\n");
      delay(3000);
      tft.print("Installing drivers\n");
      delay(4000);
      tft.print("Please run fw\n");
      delay(4000);
      tft.fillScreen(ST7735_BLACK);
      tft.setCursor(0, 10);


    } else {
      metadata = 90;
    }
    if (userInput == "config") {
      tft.print("Starting config..");
      delay(4000);
      tft.fillScreen(ST7735_BLACK);
      tft.setCursor(0, 10);
      String bcfg = Serial.readStringUntil(';');
      bcfg.trim();
      
      tft.print("Please wait..");
      delay(5000);
      bk = bcfg;
      tft.print("OUT OF MEMORY!\n");
      delay(4000);
      tft.print("RESTARTING..");
      delay(6000);
      setup();
          
        
      
    }
    Serial.print("Executed: ");
    Serial.println(userInput);
  }
}
