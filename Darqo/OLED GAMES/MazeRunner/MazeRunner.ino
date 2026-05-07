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
int playerX = 10;
int playerY = 10;
int playerSize = 5;

// Arah gerak
// 0 = kanan, 1 = bawah, 2 = kiri, 3 = atas
int direction = 0;

// Target
int targetX = 100;
int targetY = 45;
int targetSize = 5;

// Game
int score = 0;
bool gameOver = false;

// Touch
bool lastTouchState = LOW;

// Kecepatan game
unsigned long lastMoveTime = 0;
int moveDelay = 120;

void setup() {
  pinMode(TOUCH_PIN, INPUT);

  // OLED ESP32: SDA = 21, SCL = 22
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

  if (millis() - lastMoveTime > moveDelay) {
    movePlayer();
    checkCollisionWall();
    checkTarget();
    lastMoveTime = millis();
  }

  drawGame();
}

void showStartScreen() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(30, 8);
  display.println("MAZE RUNNER");

  display.setCursor(5, 25);
  display.println("Touch: ganti arah");

  display.setCursor(8, 42);
  display.println("Ambil target!");

  display.display();
  delay(2000);
}

void readTouch() {
  bool touchState = digitalRead(TOUCH_PIN);

  if (touchState == HIGH && lastTouchState == LOW) {
    direction++;

    if (direction > 3) {
      direction = 0;
    }

    delay(120);
  }

  lastTouchState = touchState;
}

void movePlayer() {
  if (direction == 0) {
    playerX += 4;
  } 
  else if (direction == 1) {
    playerY += 4;
  } 
  else if (direction == 2) {
    playerX -= 4;
  } 
  else if (direction == 3) {
    playerY -= 4;
  }
}

void checkCollisionWall() {
  // Batas layar
  if (playerX < 0 || playerX > SCREEN_WIDTH - playerSize ||
      playerY < 10 || playerY > SCREEN_HEIGHT - playerSize) {
    gameOver = true;
    showGameOver();
  }

  // Dinding 1
  if (hitBox(playerX, playerY, playerSize, playerSize, 35, 18, 8, 35)) {
    gameOver = true;
    showGameOver();
  }

  // Dinding 2
  if (hitBox(playerX, playerY, playerSize, playerSize, 70, 10, 8, 35)) {
    gameOver = true;
    showGameOver();
  }

  // Dinding 3
  if (hitBox(playerX, playerY, playerSize, playerSize, 95, 30, 8, 30)) {
    gameOver = true;
    showGameOver();
  }
}

void checkTarget() {
  if (hitBox(playerX, playerY, playerSize, playerSize,
             targetX, targetY, targetSize, targetSize)) {
    score++;

    targetX = random(5, 118);
    targetY = random(15, 55);

    // Makin lama makin cepat
    if (score % 3 == 0 && moveDelay > 60) {
      moveDelay -= 10;
    }
  }
}

bool hitBox(int x1, int y1, int w1, int h1,
            int x2, int y2, int w2, int h2) {
  return x1 < x2 + w2 &&
         x1 + w1 > x2 &&
         y1 < y2 + h2 &&
         y1 + h1 > y2;
}

void drawGame() {
  display.clearDisplay();

  // Score
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Score:");
  display.print(score);

  // Area batas atas
  display.drawLine(0, 9, 127, 9, SSD1306_WHITE);

  // Dinding maze
  display.fillRect(35, 18, 8, 35, SSD1306_WHITE);
  display.fillRect(70, 10, 8, 35, SSD1306_WHITE);
  display.fillRect(95, 30, 8, 30, SSD1306_WHITE);

  // Target
  drawStar(targetX, targetY);

  // Player
  display.fillRect(playerX, playerY, playerSize, playerSize, SSD1306_WHITE);

  display.display();
}

void drawStar(int x, int y) {
  display.drawPixel(x + 2, y, SSD1306_WHITE);
  display.drawPixel(x + 2, y + 1, SSD1306_WHITE);
  display.drawPixel(x, y + 2, SSD1306_WHITE);
  display.drawPixel(x + 1, y + 2, SSD1306_WHITE);
  display.drawPixel(x + 2, y + 2, SSD1306_WHITE);
  display.drawPixel(x + 3, y + 2, SSD1306_WHITE);
  display.drawPixel(x + 4, y + 2, SSD1306_WHITE);
  display.drawPixel(x + 2, y + 3, SSD1306_WHITE);
  display.drawPixel(x + 2, y + 4, SSD1306_WHITE);
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
  playerX = 10;
  playerY = 10;
  direction = 0;
  targetX = 100;
  targetY = 45;
  score = 0;
  moveDelay = 120;
  gameOver = false;
  lastMoveTime = millis();
}