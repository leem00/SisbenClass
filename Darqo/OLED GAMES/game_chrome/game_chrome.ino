#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Ukuran OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

// Pin touch sensor
#define TOUCH_PIN 4

// OLED reset pin, -1 karena tidak memakai pin reset
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Posisi player
int playerX = 15;
int playerY = 48;
int playerSize = 10;

// Lompat
bool isJumping = false;
int velocityY = 0;
int gravity = 1;

// Rintangan
int obstacleX = 128;
int obstacleY = 50;
int obstacleW = 8;
int obstacleH = 10;

// Game
int score = 0;
bool gameOver = false;

void setup() {
  pinMode(TOUCH_PIN, INPUT);

  // OLED ESP32
  // SDA = GPIO 21
  // SCL = GPIO 22
  Wire.begin(21, 22);

  // Memulai OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

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
  updatePlayer();
  updateObstacle();
  checkCollision();
  drawGame();

  delay(40);
}

void showStartScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20, 15);
  display.println("GAME OLED");
  display.setCursor(16, 30);
  display.println("Touch = Lompat");
  display.setCursor(20, 45);
  display.println("Hindari balok");
  display.display();
  delay(2000);
}

void readTouch() {
  int touchState = digitalRead(TOUCH_PIN);

  if (touchState == HIGH && !isJumping) {
    isJumping = true;
    velocityY = -10;
  }
}

void updatePlayer() {
  if (isJumping) {
    playerY += velocityY;
    velocityY += gravity;

    if (playerY >= 48) {
      playerY = 48;
      isJumping = false;
      velocityY = 0;
    }
  }
}

void updateObstacle() {
  obstacleX -= 5;

  if (obstacleX < -obstacleW) {
    obstacleX = 128;
    score++;
  }
}

void checkCollision() {
  bool hitX = playerX < obstacleX + obstacleW && playerX + playerSize > obstacleX;
  bool hitY = playerY < obstacleY + obstacleH && playerY + playerSize > obstacleY;

  if (hitX && hitY) {
    gameOver = true;
    showGameOver();
  }
}

void drawGame() {
  display.clearDisplay();

  // Tanah
  display.drawLine(0, 60, 128, 60, SSD1306_WHITE);

  // Player
  display.fillRect(playerX, playerY, playerSize, playerSize, SSD1306_WHITE);

  // Rintangan
  display.fillRect(obstacleX, obstacleY, obstacleW, obstacleH, SSD1306_WHITE);

  // Score
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);

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

  display.setCursor(13, 48);
  display.println("Touch untuk ulang");

  display.display();
}

void resetGame() {
  playerY = 48;
  isJumping = false;
  velocityY = 0;

  obstacleX = 128;
  score = 0;
  gameOver = false;

  display.clearDisplay();
}