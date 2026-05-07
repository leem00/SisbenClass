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

// Player
int playerX = 50;
int playerY = 56;
int playerW = 20;
int playerH = 5;

// Kotak jatuh
int boxX = 30;
int boxY = 0;
int boxSize = 8;
int boxSpeed = 2;

// Game
int score = 0;
bool gameOver = false;

// Untuk mendeteksi sentuhan sekali saja
bool lastTouchState = LOW;

void setup() {
  pinMode(TOUCH_PIN, INPUT);

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
  updateBox();
  checkCatch();
  drawGame();

  delay(40);
}

void showStartScreen() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(18, 10);
  display.println("CATCH THE BOX");

  display.setCursor(10, 28);
  display.println("Touch: gerak kanan");

  display.setCursor(8, 45);
  display.println("Tangkap kotaknya!");

  display.display();
  delay(2000);
}

void readTouch() {
  bool touchState = digitalRead(TOUCH_PIN);

  // Gerak hanya saat baru disentuh
  if (touchState == HIGH && lastTouchState == LOW) {
    playerX += 15;

    // Kalau sudah mentok kanan, balik ke kiri
    if (playerX > SCREEN_WIDTH - playerW) {
      playerX = 0;
    }
  }

  lastTouchState = touchState;
}

void updateBox() {
  boxY += boxSpeed;

  // Kalau kotak jatuh sampai bawah
  if (boxY > SCREEN_HEIGHT) {
    gameOver = true;
    showGameOver();
  }
}

void checkCatch() {
  bool hitX = boxX + boxSize >= playerX && boxX <= playerX + playerW;
  bool hitY = boxY + boxSize >= playerY && boxY <= playerY + playerH;

  if (hitX && hitY) {
    score++;

    // Tambah kecepatan sedikit
    if (score % 3 == 0) {
      boxSpeed++;
    }

    resetBox();
  }
}

void resetBox() {
  boxX = random(0, SCREEN_WIDTH - boxSize);
  boxY = 0;
}

void drawGame() {
  display.clearDisplay();

  // Score
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);

  // Kotak jatuh
  display.fillRect(boxX, boxY, boxSize, boxSize, SSD1306_WHITE);

  // Player
  display.fillRect(playerX, playerY, playerW, playerH, SSD1306_WHITE);

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
  playerX = 50;
  boxX = random(0, SCREEN_WIDTH - boxSize);
  boxY = 0;
  boxSpeed = 2;
  score = 0;
  gameOver = false;
}