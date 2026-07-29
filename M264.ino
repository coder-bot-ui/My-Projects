#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS   22
#define TFT_RST  21
#define TFT_DC   20
#define TFT_MOSI 0
#define TFT_SCLK 1

#define ST7735_DARKGREY   0x39E7
#define ST7735_GREY       0x7BEF

#define VCC 2
#define GND 3
#define ep  4
#define bk  5
#define A   6
#define B   7
#define upd 8
#define cfg 9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

struct PostBox {
  int d1;
  int d2;
  int d3;
  const char* r1;
  const char* r2;
  String s1;
  String s2;
  String s3;
  bool value;
  bool b2;
};

PostBox mailbox_tty1 = {0, 0, 0, "0", "0", "0", "0", "0", false, false};
PostBox mailbox_tty2 = {0, 0, 0, "0", "0", "0", "0", "0", false, false};

const char* FOLDER = "System32"; 
int cmd = 0;

volatile bool tty1 = true;
volatile bool tty2 = false;
volatile bool system_ready = false; 

int seckey = 56886;
String kbd_buffer = "";
String kbd_buffer_2 = "";

int err = 1;

void kbd_logic();
void TTY1();
void TTY2();
void brick();
void Recovery();

void render_boot() {
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(3);
  tft.setCursor(34, 40);
  tft.print(" 8086");
  LoadingBar();
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(1);
}

void LoadingBar() {
  tft.fillRect(14, 100, 130, 10, ST7735_GREY);
  delay(2000);
  tft.fillRect(14, 100, 20, 10, ST7735_WHITE);
  delay(3000);
  tft.fillRect(14, 100, 30, 10, ST7735_WHITE);
  delay(4000);
  tft.fillRect(14, 100, 65, 10, ST7735_WHITE);
  delay(5000);
  tft.fillRect(14, 100, 70, 10, ST7735_WHITE);
  delay(2000);
  tft.fillRect(14, 100, 73, 10, ST7735_WHITE);
  delay(4000);
  tft.fillRect(14, 100, 75, 10, ST7735_WHITE);
  delay(9000);
  tft.fillRect(14, 100, 130, 10, ST7735_WHITE);
  delay(3000);
  tft.setTextColor(ST7735_GREEN);
  
}

void Recovery(){
  tft.setCursor(0, 0);
  tft.fillScreen(ST7735_BLACK);
  tft.print("Recovery Mode");
  tft.setCursor(0, 20);
  tft.print("Wipe Cache\n");
  tft.fillRect(0, 20, 150, 9, ST7735_WHITE);
  tft.setCursor(0, 20);
  tft.setTextColor(ST7735_BLACK);
  tft.print("Wipe Cache");
  tft.setCursor(0, 30);
  tft.setTextColor(ST7735_GREEN);

  tft.print("Factory Reset\n");
  tft.print("EMOTIONAL DAMAGE!\n");
  tft.print("Restart");

}
void deb() {
  delay(500); // Cleaned up into a single robust delay block
}

void BIOS() {
  render_boot();
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.print("Starting in Real Mode\n");
  delay(1000);
  tft.print("Preparing GDT 1\n");
  delay(500);
  tft.print("Preparing GDT 2\n");
  delay(1000);
  tft.print("Allocating RAM\n");
  delay(1500);
  tft.print("Proccessing\n");
  delay(1550);
  tft.print("Starting in 32-bit Mode\n");
  delay(1200);
  tft.print("Preparing headers\n");
  delay(450);
  tft.print("Alocating RAMDISK\n");
  delay(600);
  tft.print("Preparing 64-bit key\n");
  delay(300);
  
  tft.print("Ran Tests [OK]\n");
  delay(1000);
  tft.print("Starting in 64-bit Mode\n");
  delay(1000);
  tft.print("Allocating Disk\n");
  delay(1000);
  tft.print("Allocating RAMDISK\n");
  delay(1000);
  tft.print("Ran CMOS Check [OK]\n");
  delay(4000);
  tft.fillScreen(ST7735_BLUE);
  delay(3000);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.print("Starting Input Busses [\x1E]\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  delay(1000);
  tft.print("POST FAILED!\n");
  warning(80, 54);
  LoadingBar();
 
  
}

void warning(int x, int y){
  tft.fillScreen(ST7735_BLACK);
  tft.fillTriangle(x, y - 30, x - 30, y + 20, x + 30, y + 20, ST7735_YELLOW);
  
  tft.fillRect(x - 2, y - 12, 5, 18, ST7735_BLACK);
  tft.fillRect(x - 2, y + 10, 5, 5, ST7735_BLACK);

}

void update(){
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(34, 40);
  tft.setTextColor(ST7735_WHITE);
  explorer();
  delay(5000);
  tft.print("BIOS IS UPDATING");
  tft.fillRect(14, 80, 130, 15, ST7735_GREY);
  delay(2000);
  tft.fillRect(14, 80, 20, 15, ST7735_WHITE);
  delay(3000);
  tft.fillRect(14, 80, 30, 15, ST7735_WHITE);
  delay(4000);
  tft.fillRect(14, 80, 65, 15, ST7735_WHITE);
  delay(5000);
  tft.fillRect(14, 80, 70, 15, ST7735_WHITE);
  delay(2000);
  tft.fillRect(14, 80, 73, 15, ST7735_WHITE);
  delay(4000);
  tft.fillRect(14, 80, 75, 15, ST7735_WHITE);
  delay(9000);
  tft.fillRect(14, 80, 130, 15, ST7735_WHITE);
  delay(3000);
  tft.setTextColor(ST7735_GREEN);
  BIOS();

   
  tty1 = true; // Reset states instead of calling setup() recursively
  tty2 = false;
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
  tft.print("FLASH \x1A RAM\n"); 
  tft.print("POST ERROR\n");
  delay(3000);
  tft.fillScreen(ST7735_RED);
  tft.setCursor(0, 0);
  tft.print("Recovery Environment\n");
  delay(3000);
  tft.print("KERNEL PANIC !\n");
  delay(3000);
  warning(80, 40);
  tft.setCursor(12, 75);
  tft.print("This device is bricked\n");
  while(1) { delay(1000); } // Safe execution halt
}

void TTY1() {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_GREEN);
  tft.print("TTY1");
  tft.setCursor(0, 15);
  update();
  // FIX: Loop runs ONLY while tty1 is true. 
  // When buttons flip the booleans, this loop terminates cleanly!
  while (tty1) {
    kbd_logic(); 
    delay(10);    
  }
}

void TTY2() {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_GREEN);
  tft.print("TTY2");
  tft.setCursor(0, 15);
  
  while (tty2) {
    kbd_logic(); 
    delay(10);    
  }
}

void explorer(){
  tft.setTextColor(ST7735_WHITE);
  
  tft.fillRect(20, 20, 60, 35, ST7735_YELLOW);
  tft.fillRect(22, 15, 35, 5, ST7735_WHITE);
  tft.fillRect(25, 16, 15, 3, ST7735_GREY);
  tft.setCursor(20, 80);
  tft.print("YOS\n");
  delay(5000);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(34, 40);
}

void kbd_logic() {
  if (digitalRead(ep) == LOW) {
    explorer();
    while(digitalRead(ep) == LOW) { delay(5); } 
  }

  if (digitalRead(bk) == LOW) {
    BIOS(); 
    while(digitalRead(bk) == LOW) { delay(5); }
    // Force a redraw of the active environment after exiting BIOS view
    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0, 0);
    tft.print(tty1 ? "TTY1" : "TTY2");
  }
  
  // FIX: Buttons change states instead of calling functions recursively!
  if (digitalRead(A) == LOW) {
    if (!tty1) { // Only switch if we aren't already here
      deb();
      tty2 = false;
      tty1 = true;
    }
    while(digitalRead(A) == LOW) { delay(5); }
  }
  
  if (digitalRead(B) == LOW) {
    if (!tty2) {
      deb();
      tty1 = false;
      tty2 = true;
    }
    while(digitalRead(B) == LOW) { delay(5); }
  }
 
  if (digitalRead(upd) == LOW) {
    update();
    while(digitalRead(upd) == LOW) { delay(5); }
  }
  
  if (digitalRead(cfg) == LOW) {
    brick();
  }
}

// ==========================================================
// CORE 0: INTERFACE & INITIALIZATION ENGINE
// ==========================================================
void setup() {
  Serial.begin(115200);
  if (err){
    warning(80, 64);
    
  }
  
  // Clean power rails setup
  pinMode(VCC, OUTPUT);  digitalWrite(VCC, HIGH);
  pinMode(GND, OUTPUT);  digitalWrite(GND, LOW);
  delay(100); // Give hardware power a tiny moment to settle
  
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  
  pinMode(ep, INPUT_PULLUP);   pinMode(bk, INPUT_PULLUP);
  pinMode(A, INPUT_PULLUP);    pinMode(B, INPUT_PULLUP);
  pinMode(upd, INPUT_PULLUP);  pinMode(cfg, INPUT_PULLUP);
  
  delay(200);
  system_ready = true; // RELEASE INTERLOCK: Core 1 can now safely run
}

void loop() {
  // Core 0 handles TTY1 environment
  if (tty1) {
    cmd = 5;
    TTY1();
  }
  delay(20); // Keep core stable when idle
}

// ==========================================================
// CORE 1: AUXILIARY PROCESSOR (FreeRTOS SMP Core)
// ==========================================================
void setup1() {
  // FIX: Wait until Core 0 finishes initializing the display hardware
  while (!system_ready) {
    delay(10);
  }
}

void loop1() {
  // Core 1 handles TTY2 environment exclusively
  if (tty2) {
    cmd = 4;
    TTY2();
  }
  delay(20);
}