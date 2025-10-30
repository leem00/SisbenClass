#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LiquidCrystal_I2C namaDisplay(alamat I2C, panjang LCD, lebar LCD);
LiquidCrystal_I2C displayESP(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  displayESP.init();

  displayESP.clear();
  displayESP.backlight();
} 

void loop() {
  displayESP.setCursor(0, 0);
  displayESP.print("Hello World");
  delay(1000);
  displayESP.setCursor(0, 1);
  displayESP.print("Saya Ucim");
  delay(1000);
  displayESP.clear();
  delay(1000);
}