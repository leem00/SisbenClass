int pir = 18;
int status;
int led = 14;

void setup() {
  Serial.begin(115200);
  pinMode(pir, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  status = digitalRead(pir);

  if (status == 0){
    Serial.println("Tidak ada gerakan");
    digitalWrite(led, LOW);
  } else{
    Serial.println("Gerakan terdeteksi");
    digitalWrite(led, HIGH);
  }
  
  delay(500);
}