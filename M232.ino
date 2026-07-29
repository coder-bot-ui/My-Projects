#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define TFT_CS   22
#define TFT_RST  21
#define TFT_DC   20
#define TFT_MOSI 0
#define TFT_SCLK 1

#define VCC 2
#define GND 3
#define ep  4
#define bk  5
#define A   6
#define B   7
#define upd 8
#define cfg 9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// 1. FIXED: Moved struct blueprint to the absolute top of the file!
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

// Now the compiler safely understands how to allocate your mail boxes
PostBox mailbox_tty1 = {0, 0, 0, "0", "0", "0", "0", "0", false, false};
PostBox mailbox_tty2 = {0, 0, 0, "0", "0", "0", "0", "0", false, false};

const char* FOLDER = "System32"; // Restored missing folder variable marker
int cmd = 0;
volatile bool tty1 = true;
volatile bool tty2 = false;

String kbd_buffer = "";
String kbd_buffer_2 = "";

// Forward declarations to unblock the compiler's downward parsing view
void TTY1();
void TTY2();
void kbd_logic();
void brick();
void setup();

void deb() {
  delay(200);
  delay(100);
  delay(200);
}

void BIOS() {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(12, 20);
  tft.print("BIOS IS LOADING");
  tft.setCursor(80, 40);
  tft.print("\xB2");
  delay(2000);
  tft.println("\xB2\xB2");
  delay(1000);
  tft.print("\xB2\xB2\xB2");
  delay(1000);
  tft.println("\xB2\xB2\xB2\xB2");
  delay(2000);
  tft.print("\xB2\xB2\xB2\xB2\xB2");
}

void TTY2() {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.print("TTY2");
  tft.setCursor(0, 15);
  int RUN = 1;
  while (RUN) {
    kbd_logic(); // FIXED: Matched spelling with underscore
    delay(5);    // Keeps loop stable
  }
}

void warning(int x, int y){
  tft.fillScreen(ST7735_BLACK);
  tft.fillTriangle(x, y - 30, x - 30, y + 20, x + 30, y + 20, ST7735_YELLOW);
  tft.drawTriangle(x, y - 30, x - 30, y + 20, x + 30, y + 20, ST7735_BLACK);
  tft.fillRect(x - 2, y - 12, 5, 18, ST7735_BLACK);
  tft.fillRect(x - 2, y + 10, 5, 5, ST7735_BLACK);
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

void brick(){
  tft.fillScreen(ST7735_BLUE);
  tft.setCursor(5, 10);
  tft.setTextSize(4); 
  tft.setTextColor(ST7735_WHITE);
  tft.print(":(\n");
  tft.setTextSize(1);
  tft.setCursor(0, 60);
  tft.print("ERR: BAD_CONFIG\n\n");
  tft.print("FLASH \x1A RAM\n"); // Using right arrow byte sequence
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

void sesman(){
  if (tty1) {
    deb();
    TTY1();
    cmd = 5;
  } else {
    deb();
    TTY2();
    cmd = 4;
  }
}

void TTY1() {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.print("TTY1");
  tft.setCursor(0, 15);
  int RUN1 = 1;
  while (RUN1) {
    kbd_logic(); // FIXED: Matched spelling with underscore
    delay(5);    // Keeps loop stable
  }
}

void explorer(){
  tft.setTextColor(ST7735_WHITE);
  tft.print("YOS\n");
  tft.setCursor(0, 75);
  tft.fillRect(20, 80, 55, 35, ST7735_YELLOW);
  tft.fillRect(22, 75, 25, 4, ST7735_WHITE);
  tft.setCursor(20, 118);
  tft.print(FOLDER);
  tft.setTextColor(ST7735_GREEN);
}

void kbd_logic() {
  if (digitalRead(ep) == LOW) {
    explorer();
    while(digitalRead(ep) == LOW) { delay(5); } // Anti-bounce optimization
  }

  if (digitalRead(bk) == LOW) {
    BIOS(); // FIXED: Added missing semicolon
    while(digitalRead(bk) == LOW) { delay(5); }
  }
  
  if (digitalRead(A) == LOW) {
    tty1 = false;
    tty2 = false;
    deb();
    tft.fillScreen(ST7735_BLACK);
    TTY1();
    while(digitalRead(A) == LOW) { delay(5); }
  }
  
  if (digitalRead(B) == LOW) {
    tty1 = false;
    tty2 = false;
    deb();
    tft.fillScreen(ST7735_BLACK);
    TTY2();
    while(digitalRead(B) == LOW) { delay(5); }
  }
 
  if (digitalRead(upd) == LOW) {
    update();
    while(digitalRead(upd) == LOW) { delay(5); }
  }
  
  if (digitalRead(cfg) == LOW) {
    brick();
    while(digitalRead(cfg) == LOW) { delay(5); }
  }
}

void setup1() {
  sesman();
  deb();
}

void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_GREEN);
  
  pinMode(VCC, OUTPUT);  digitalWrite(VCC, HIGH);
  pinMode(GND, OUTPUT);  digitalWrite(GND, LOW);
  
  pinMode(ep, INPUT_PULLUP);   pinMode(bk, INPUT_PULLUP);
  pinMode(A, INPUT_PULLUP);    pinMode(B, INPUT_PULLUP);
  pinMode(upd, INPUT_PULLUP);  pinMode(cfg, INPUT_PULLUP);
  
  delay(500);
  deb();
}

void loop1() {
  
  
}

void loop() {
  sesman();
  delay(10);
}
