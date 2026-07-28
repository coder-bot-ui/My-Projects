#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <pico/bootrom.h>




bool bootsel_press() {

  return BOOTSEL;
}


int ch = 0;
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

struct bootutil {
  int BIOS;
  int KEY;
  int d1;
  const char* s1;
  const char* s2;
  const char* s3;
  const char* s4;
  int d2;
  int d3;
  int d4;
  uint64_t WoW;
};

const char* FOLDER = "System32";
int metadata = 91964;
int metadata1 = 0;
int metadata2 = 70573;
int metadata3 = 84057;
int BIOS = 1;
String args = "*args32";

String bk;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
bootutil SETTINGS {1, 5, 1, "YUVCORP CORPRATION", "Pico-Project 1", "Project 1970", "Raspberry Pi Pico", 1, 1970, 9807, 64317};


void ch_check() {
  if (ch) {
    tft.print("CRASH FALLBACK:\n");
    tft.print("FLASH \x1A RAM\n");
    
    tft.print("COLOR:\n");
    tft.print("BLUE");

  }else{
    tft.print("CRASH FALLBACK:\n");
    tft.print("FLASH \E RAM\n");
    
    tft.print("COLOR:\n");
    tft.print("BLUE");

  }
  ch = 0;
}

void color_func(){
  if (bootsel_press()) {
    tft.setCursor(0, 10);
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_GREEN);
    ch_check();
  }
}

void react() {
  if (bootsel_press()) {
    tft.setCursor(0, 10);
    tft.fillScreen(ST7735_RED);
    tft.setTextColor(ST7735_WHITE);
    tft.print("CRASH FALLBACK:\n");
    tft.print("FLASH \x1A RAM\n");
    ch = 1;
    tft.print("COLOR:\n");
    tft.print("BLUE");

  }
}

void update(){
  warning(60, 30);
  tft.setCursor(20, 70);
  tft.fillScreen(ST7735_RED);
  tft.setTextColor(ST7735_WHITE);
  tft.print("BIOS IS UPDATING\n");
  delay(5000);
  tft.print("rebooting..");
  delay(1000);
  setup();
  brick();

}

void warning(int x, int y){
  tft.fillScreen(ST7735_BLACK);
  tft.fillTriangle(x, y - 30, x - 30, y + 20, x + 30, y + 20, ST7735_YELLOW);
  tft.drawTriangle(x, y - 30, x - 30, y + 20, x + 30, y + 20, ST7735_BLACK);
  tft.fillRect(x - 2, y - 12, 5, 18, ST7735_BLACK);
  tft.fillRect(x - 2, y + 10, 5, 5, ST7735_BLACK);


}

void explorer(){
  tft.print("Starting explorer..");
  delay(1000);
  tft.fillScreen(ST7735_BLACK);
  FolderICON();
  tft.print("YOS\n");
  tft.setCursor(0, 75);
  tft.setTextColor(ST7735_WHITE);
  tft.fillRect(20, 80, 55, 35, ST7735_YELLOW);
  tft.fillRect(22, 75, 25, 4, ST7735_WHITE);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(20, 118);
  tft.print(FOLDER);
  tft.setTextColor(ST7735_GREEN);

}

void brick(){
  tft.fillScreen(ST7735_BLUE);
  tft.setCursor(5, 10);
  tft.setTextSize(4); 
  tft.setTextColor(ST7735_WHITE);
  tft.print(":(\n");
  tft.setTextSize(1);
  tft.setCursor(0, 60);
  tft.print("ERR: BAD_CONFIG\n\n");
  tft.print("FLASH \E RAM\n");
  tft.print("POST ERROR\n");
  delay(5000);
  tft.fillScreen(ST7735_RED);
  tft.setCursor(0, 0);
  tft.print("Recovery Environment\n");
  delay(6000);
  tft.print("KERNEL PANIC !\n");
  delay(5000);
  tft.fillScreen(ST7735_RED);
  warning(80, 40);
  tft.setCursor(12, 75);
  tft.setTextSize(1);
  tft.print("This device is bricked\n");
  
  delay(1000000);

  

  
}

void SHELL(){
  tft.fillScreen(ST7735_BLACK);
  tft.fillRect(0, 118, 10, 5, ST7735_GREEN);
  tft.fillRect(20, 118, 90, 5, ST7735_GREEN);
  tft.drawRect(10, 20, 60, 60, ST7735_GREEN);

  tft.setCursor(0, 70);
  tft.setTextColor(ST7735_GREEN);
  tft.print(">>> ");

}

void FolderICON() {
  tft.fillRect(20, 20, 55, 35, ST7735_YELLOW);
  tft.fillRect(22, 15, 25, 4, ST7735_WHITE);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(34, 60);
}


void shell(){
  if (Serial.available() > 0) {
    String userInput = Serial.readStringUntil(';');
    userInput.trim();

    tft.print("A:/> ");
    tft.println(userInput);
    if (userInput == "setup") {
      tft.print("\nStarting setup..");
      delay(1000);
      tft.fillScreen(ST7735_BLUE);
      tft.setTextColor(ST7735_WHITE);
      tft.setCursor(0, 10);
      tft.print("YOS SETUP");
      tft.print("\nSetup is starting..");
      delay(2000);
      tft.print("\nSetup is Unpacking Files\n");
      delay(1000);
      tft.print("Formating disk\n");
      delay(1000);
      tft.print("> setup.exe -e\n");
      delay(500);
      tft.fillScreen(ST7735_BLACK);
      tft.setTextColor(ST7735_GREEN);
      tft.setCursor(0, 10);
    } else {
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
      tft.print("7654 metadata\n");
      
      
      
    } else {
      int metadata = 2097;
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
      tft.print("Please run config\n");
      
      
      
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

      tft.print("Please wait..\n");
      delay(5000);
      bk = bcfg;
      tft.print("OUT OF MEMORY!\n");
      delay(4000);
      tft.print("RESTARTING..");
      delay(6000);
      brick();
      delay(5000);
      setup();
    }

    if (userInput == "explorer") {
      tft.print("Starting explorer..");
      delay(1000);
      tft.fillScreen(ST7735_BLACK);
      FolderICON();
      tft.print("YOS\n");
      tft.setCursor(0, 75);
      tft.setTextColor(ST7735_WHITE);
      tft.fillRect(20, 80, 55, 35, ST7735_YELLOW);
      tft.fillRect(22, 75, 25, 4, ST7735_WHITE);
      tft.setTextColor(ST7735_WHITE);
      tft.setCursor(20, 118);
      tft.print(FOLDER);
      tft.setTextColor(ST7735_GREEN);



      
     

    }
    if (userInput == "cls") {
      tft.fillScreen(ST7735_BLACK);
      tft.setCursor(0, 10);
    }
    if (userInput == "SHELL"){
      SHELL();
    }
    if (userInput == "update") {
      update();
    }
    if (userInput == "bios") {
      setup1();
    }
    

    Serial.print("Executed: ");
    Serial.println(userInput);
  } 
}

void setup() {
  Serial.begin(115200);
  args.reserve(512);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_GREEN);
  tft.print("Starting..\n");
  delay(1000);
  if (args == "explorer") {
    tft.print("bootargs is calling ep\n");
    delay(1000);
    explorer();
    delay(4000);
  }
  if (args == "config") {
    brick();
  }
  if (args == "update") {
    update();
  }

  delay(500);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  setup1();
}

void setup1() {
  if (bootsel_press()) {
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_GREEN);
    tft.setCursor(0, 0);
    tft.print("Starting BIOS Setup");
    delay(4000);
    tft.println("\n");
    tft.println("Please wait..");
    delay(1000);
    tft.println("BIOS FOR YOS SYSTEM");
    delay(2000);
    tft.fillScreen(ST7735_RED);
    tft.setCursor(0, 0);
    tft.setTextColor(ST7735_WHITE);
    tft.print("CRASH FALLBACK:\n");
    tft.print("FLASH \E RAM\n");
    int value = 5;
    tft.print("COLOR:\n");
    tft.print("BLUE");
    while (value > 0) {
      delay(500);
      react();
      delay(500);
      color_func();

      delay(500);
      react();
    }
    
    
    
    

  }

}

void loop1(){

  
}

void loop(){
  shell();
}