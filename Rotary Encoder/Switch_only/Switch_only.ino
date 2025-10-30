int button = 13;
int BUILT = 2;
int bStatus = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(BUILT, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  bStatus = digitalRead(button);

  if (bStatus == 0){
    digitalWrite(BUILT, HIGH);
  } else {
    digitalWrite(BUILT, LOW);
  }
  Serial.println(bStatus);
  delay(100);
}
