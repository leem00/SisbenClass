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
int birdX = 25;
int birdY = 30;
int birdSize = 6;
int velocityY = 0;

// Pipa
int pipeX = 128;
int pipeW = 12;
int gapY = 25;
int gapH = 22;

// Game
int score = 0;
bool gameOver = false;
bool passedPipe = false;

void setup() {
  pinMode(TOUCH_PIN, INPUT);

  // OLED ESP32
  Wire.begin(SDA_PIN, SCL_PIN);

  // Mulai OLED
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
  updateBird();
  updatePipe();
  checkCollision();
  drawGame();

  delay(45);
}

void showStartScreen() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(35, 10);
  display.println("FLAPPY BOX");

  display.setCursor(13, 28);
  display.println("Touch untuk naik");

  display.setCursor(12, 45);
  display.println("Lewati celah pipa");

  display.display();
  delay(2000);
}

void readTouch() {
  if (digitalRead(TOUCH_PIN) == HIGH) {
    velocityY = -4;
  }
}

void updateBird() {
  velocityY += 1;
  birdY += velocityY;

  if (birdY < 0) {
    birdY = 0;
    velocityY = 0;
  }

  if (birdY > SCREEN_HEIGHT - birdSize) {
    gameOver = true;
    showGameOver();
  }
}

void updatePipe() {
  pipeX -= 3;

  if (pipeX < -pipeW) {
    pipeX = SCREEN_WIDTH;
    gapY = random(12, 38);
    passedPipe = false;
  }

  if (!passedPipe && pipeX + pipeW < birdX) {
    score++;
    passedPipe = true;
  }
}

void checkCollision() {
  bool hitX = birdX + birdSize > pipeX && birdX < pipeX + pipeW;

  bool hitTopPipe = birdY < gapY;
  bool hitBottomPipe = birdY + birdSize > gapY + gapH;

  if (hitX && (hitTopPipe || hitBottomPipe)) {
    gameOver = true;
    showGameOver();
  }
}

void drawGame() {
  display.clearDisplay();

  // Score
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);

  // Player
  display.fillRect(birdX, birdY, birdSize, birdSize, SSD1306_WHITE);

  // Pipa atas
  display.fillRect(pipeX, 0, pipeW, gapY, SSD1306_WHITE);

  // Pipa bawah
  display.fillRect(pipeX, gapY + gapH, pipeW, SCREEN_HEIGHT - (gapY + gapH), SSD1306_WHITE);

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
  birdY = 30;
  velocityY = 0;

  pipeX = SCREEN_WIDTH;
  gapY = random(12, 38);

  score = 0;
  gameOver = false;
  passedPipe = false;
}