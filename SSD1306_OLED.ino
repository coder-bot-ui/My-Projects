#include <Arduino.h>
#include <mbed.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET    -1 

// 1. FIXED: Explicitly map the I2C0 registers to pin p0 (GP0) and pin p1 (GP1)
// Using the native Mbed wrapper architecture
arduino::MbedI2C customWire(p0, p1);

// Pass our customWire object straight to the display driver
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &customWire, OLED_RESET);

rtos::Thread oled_thread;

// --- OLED CORE 1 THREAD WORKER ---
void oled_render_loop() {
  // Initialize our custom mapped I2C traces
  customWire.begin();

  // Initialize the driver chip (Standard address is 0x3C)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("KERNEL_ERR: Allocation failed at GP0/GP1!");
    while(true); 
  }

  display.clearDisplay();
  display.display();

  display.setTextSize(1);             
  // 2. FIXED: Swapped out GREEN for WHITE to pass the compiler logic check
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0);             
  display.println("--- PICO KERNEL v2 ---");
  display.println("C:\> Mbed             ");
  display.println("EMBEDDING FIRMWARE ...");
  display.setCursor(0, 45);
  display.print("C:/> ");

  display.display();

  int state_counter = 0;
  while(true) {
    display.fillRect(30, 45, 90, 10, SSD1306_BLACK); 
    display.setCursor(30, 45);
    display.print("RUN_TICK_");
    display.print(state_counter++);
    display.display();

    rtos::ThisThread::sleep_for(1000); 
  }
}

// --- MAIN RUNTIME LOOPS (CORE 0) ---
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Spin up our isolated display worker loop straight onto Core 1
  oled_thread.start(oled_render_loop);
  Serial.println("SYSTEM: Mbed kernel pin override locked.");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(800);
}
