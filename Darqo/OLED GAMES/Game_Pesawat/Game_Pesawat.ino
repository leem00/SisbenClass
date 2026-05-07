#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Ukuran OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Pin OLED ESP32
#define SDA_PIN 21
#define SCL_PIN 22

// Pin touch sensor
#define TOUCH_PIN 4

// OLED reset
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pesawat
int shipX = 56;
int shipY = 54;
int shipW = 12;
int shipH = 8;

// Peluru
int bulletX = shipX + 5;
int bulletY = shipY;
bool bulletActive = true;

// Meteor
int meteorX = 30;
int meteorY = 0;
int meteorSize = 10;
int meteorSpeed = 2;

// Game
int score = 0;
bool gameOver = false;

// Touch
bool lastTouchState = LOW;

void setup() {
  pinMode(TOUCH_PIN, INPUT);

  // OLED ESP32: SDA = GPIO 21, SCL = GPIO 22
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  randomSeed(analogRead(0));

  showStartScreen();
}

void loop() {
  if (gameOver) {
    if (digitalRead(TOUCH_PIN) == HIGH) {
      resetGame();
      delay(300);
    }
    return;
  }

  readTouch();
  updateBullet();
  updateMeteor();
  checkBulletHit();
  checkShipHit();
  drawGame();

  delay(40);
}

void showStartScreen() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(28, 8);
  display.println("SPACE SHOOTER");

  display.setCursor(5, 26);
  display.println("Touch: geser pesawat");

  display.setCursor(8, 43);
  display.println("Tembak meteor!");

  display.display();
  delay(2000);
}

void readTouch() {
  bool touchState = digitalRead(TOUCH_PIN);

  // Setiap sentuhan, pesawat geser ke kanan
  if (touchState == HIGH && lastTouchState == LOW) {
    shipX += 16;

    // Kalau mentok kanan, balik ke kiri
    if (shipX > SCREEN_WIDTH - shipW) {
      shipX = 0;
    }

    delay(120);
  }

  lastTouchState = touchState;
}

void updateBullet() {
  if (bulletActive) {
    bulletY -= 5;

    if (bulletY < 0) {
      bulletY = shipY;
      bulletX = shipX + shipW / 2;
    }
  }
}

void updateMeteor() {
  meteorY += meteorSpeed;

  if (meteorY > SCREEN_HEIGHT) {
    resetMeteor();
  }
}

void checkBulletHit() {
  bool hitX = bulletX >= meteorX && bulletX <= meteorX + meteorSize;
  bool hitY = bulletY >= meteorY && bulletY <= meteorY + meteorSize;

  if (hitX && hitY) {
    score++;

    if (score % 5 == 0 && meteorSpeed < 6) {
      meteorSpeed++;
    }

    resetMeteor();

    bulletY = shipY;
    bulletX = shipX + shipW / 2;
  }
}

void checkShipHit() {
  bool hitX = shipX < meteorX + meteorSize && shipX + shipW > meteorX;
  bool hitY = shipY < meteorY + meteorSize && shipY + shipH > meteorY;

  if (hitX && hitY) {
    gameOver = true;
    showGameOver();
  }
}

void resetMeteor() {
  meteorX = random(0, SCREEN_WIDTH - meteorSize);
  meteorY = -meteorSize;
}

void drawGame() {
  display.clearDisplay();

  // Score
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);

  // Peluru
  display.drawLine(bulletX, bulletY, bulletX, bulletY - 4, SSD1306_WHITE);

  // Meteor
  display.fillCircle(meteorX + meteorSize / 2, meteorY + meteorSize / 2, meteorSize / 2, SSD1306_WHITE);

  // Pesawat sederhana
  display.fillTriangle(
    shipX + shipW / 2, shipY,
    shipX, shipY + shipH,
    shipX + shipW, shipY + shipH,
    SSD1306_WHITE
  );

  display.display();
}

void showGameOver() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(35, 15);
  display.println("GAME OVER");

  display.setCursor(35, 30);
  display.print("Score: ");
  display.print(score);

  display.setCursor(10, 48);
  display.println("Touch untuk ulang");

  display.display();
}

void resetGame() {
  shipX = 56;

  bulletX = shipX + shipW / 2;
  bulletY = shipY;

  meteorSpeed = 2;
  score = 0;
  gameOver = false;

  resetMeteor();
}