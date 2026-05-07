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
#define TOUCH_LEFT 4
#define TOUCH_RIGHT 5

// Reset OLED
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Player
int playerX = 56;
int playerY = 54;
int playerW = 14;
int playerH = 8;
int playerSpeed = 4;

// Meteor
int meteorX = 40;
int meteorY = 0;
int meteorSize = 10;
int meteorSpeed = 2;

// Game
int score = 0;
bool gameOver = false;

void setup() {
  pinMode(TOUCH_LEFT, INPUT);
  pinMode(TOUCH_RIGHT, INPUT);

  // OLED ESP32: SDA = GPIO 21, SCL = GPIO 22
  Wire.begin(SDA_PIN, SCL_PIN);

  // Mulai OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  randomSeed(analogRead(0));

  showStartScreen();
  resetMeteor();
}

void loop() {
  if (gameOver) {
    if (digitalRead(TOUCH_LEFT) == HIGH || digitalRead(TOUCH_RIGHT) == HIGH) {
      resetGame();
      delay(300);
    }
    return;
  }

  readTouch();
  updateMeteor();
  checkCollision();
  drawGame();

  delay(40);
}

void showStartScreen() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(28, 8);
  display.println("DODGE METEOR");

  display.setCursor(5, 25);
  display.println("Kiri  = Touch 1");

  display.setCursor(5, 38);
  display.println("Kanan = Touch 2");

  display.setCursor(10, 52);
  display.println("Hindari meteor!");

  display.display();
  delay(2500);
}

void readTouch() {
  int leftState = digitalRead(TOUCH_LEFT);
  int rightState = digitalRead(TOUCH_RIGHT);

  if (leftState == HIGH) {
    playerX -= playerSpeed;
  }

  if (rightState == HIGH) {
    playerX += playerSpeed;
  }

  // Batas kiri layar
  if (playerX < 0) {
    playerX = 0;
  }

  // Batas kanan layar
  if (playerX > SCREEN_WIDTH - playerW) {
    playerX = SCREEN_WIDTH - playerW;
  }
}

void updateMeteor() {
  meteorY += meteorSpeed;

  if (meteorY > SCREEN_HEIGHT) {
    score++;

    if (score % 5 == 0 && meteorSpeed < 7) {
      meteorSpeed++;
    }

    resetMeteor();
  }
}

void resetMeteor() {
  meteorX = random(0, SCREEN_WIDTH - meteorSize);
  meteorY = -meteorSize;
}

void checkCollision() {
  bool hitX = playerX < meteorX + meteorSize && playerX + playerW > meteorX;
  bool hitY = playerY < meteorY + meteorSize && playerY + playerH > meteorY;

  if (hitX && hitY) {
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

  // Meteor
  display.fillCircle(
    meteorX + meteorSize / 2,
    meteorY + meteorSize / 2,
    meteorSize / 2,
    SSD1306_WHITE
  );

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

  display.setCursor(5, 48);
  display.println("Sentuh utk ulang");

  display.display();
}

void resetGame() {
  playerX = 56;
  meteorSpeed = 2;
  score = 0;
  gameOver = false;
  resetMeteor();
}