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

// Jalur player: 0 = kiri, 1 = tengah, 2 = kanan
int lane = 1;
int laneX[3] = {25, 60, 95};

// Player
int playerY = 52;
int playerW = 12;
int playerH = 8;

// Musuh / balok jatuh
int enemyLane = 0;
int enemyX = 25;
int enemyY = 0;
int enemyW = 12;
int enemyH = 8;
int enemySpeed = 2;

// Game
int score = 0;
bool gameOver = false;

// Touch detection
bool lastTouchState = LOW;

void setup() {
  pinMode(TOUCH_PIN, INPUT);

  // OLED ESP32: SDA = 21, SCL = 22
  Wire.begin(SDA_PIN, SCL_PIN);

  // Mulai OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  randomSeed(analogRead(0));

  resetEnemy();
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
  updateEnemy();
  checkCollision();
  drawGame();

  delay(45);
}

void showStartScreen() {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(32, 8);
  display.println("DODGER BOX");

  display.setCursor(5, 25);
  display.println("Touch: pindah jalur");

  display.setCursor(10, 42);
  display.println("Hindari balok jatuh");

  display.display();
  delay(2000);
}

void readTouch() {
  bool touchState = digitalRead(TOUCH_PIN);

  // Pindah jalur hanya saat baru disentuh
  if (touchState == HIGH && lastTouchState == LOW) {
    lane++;

    if (lane > 2) {
      lane = 0;
    }

    delay(120);
  }

  lastTouchState = touchState;
}

void updateEnemy() {
  enemyY += enemySpeed;

  // Jika musuh sudah lewat bawah layar
  if (enemyY > SCREEN_HEIGHT) {
    score++;

    // Setiap skor kelipatan 5, musuh makin cepat
    if (score % 5 == 0 && enemySpeed < 7) {
      enemySpeed++;
    }

    resetEnemy();
  }
}

void resetEnemy() {
  enemyLane = random(0, 3);
  enemyX = laneX[enemyLane];
  enemyY = -enemyH;
}

void checkCollision() {
  int playerX = laneX[lane];

  bool hitX = playerX < enemyX + enemyW && playerX + playerW > enemyX;
  bool hitY = playerY < enemyY + enemyH && playerY + playerH > enemyY;

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

  // Garis jalur
  display.drawLine(43, 12, 43, 64, SSD1306_WHITE);
  display.drawLine(78, 12, 78, 64, SSD1306_WHITE);

  // Player
  display.fillRect(laneX[lane], playerY, playerW, playerH, SSD1306_WHITE);

  // Musuh
  display.fillRect(enemyX, enemyY, enemyW, enemyH, SSD1306_WHITE);

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
  lane = 1;
  score = 0;
  enemySpeed = 2;
  gameOver = false;
  resetEnemy();
}