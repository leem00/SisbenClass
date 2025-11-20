#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Kalau punya OLED yang lebih gede, bisa diganti aja
#define SCREEN_WIDTH 128        // lebar display OLED (dalam px)
#define SCREEN_HEIGHT 64        // tinggi display OLED (dalam px)
#define OLED_RESET -1           // Reset pin (Ini biarin aja)
#define SCREEN_ADDRESS 0x3C     // kalau alamat I2C oled beda, tinggal diganti

// inisialisasi instance OLED
Adafruit_SSD1306 OLEDDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int counter = 0;

void setup() {
  // mulai make OLED
  OLEDDisplay.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  OLEDDisplay.clearDisplay();
}

void loop() {
  OLEDDisplay.setTextSize(3);         // Set ukuran teks pada OLED
  OLEDDisplay.setTextColor(WHITE);    // Set warna OLED (BLACK or WHITE) <Pada beberapa oled (YELLOW, BLUE)>
  OLEDDisplay.setCursor(0, 1);        // Set kursor (px kolom, px baris)
  OLEDDisplay.println("Hello");       // mau print apa dah?
  OLEDDisplay.display();              // abis itu display ke OLED
  delay(3000);
  OLEDDisplay.clearDisplay();
  
  OLEDDisplay.setTextSize(2);
  OLEDDisplay.setTextColor(WHITE);
  OLEDDisplay.setCursor(24, 46);
  OLEDDisplay.println("World");
  OLEDDisplay.display();
  delay(3000);
  OLEDDisplay.clearDisplay();

  OLEDDisplay.setTextSize(1);
  OLEDDisplay.setTextColor(BLACK, WHITE);
  OLEDDisplay.setCursor(0, 28);
  OLEDDisplay.printf("Hitungan ke-%d kali", counter);
  OLEDDisplay.display();
  delay(5000);
  OLEDDisplay.clearDisplay();
  counter++;
}
